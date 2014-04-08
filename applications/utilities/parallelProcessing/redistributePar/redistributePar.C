/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    redistributePar

Description
    Redistributes existing decomposed mesh and fields according to the current
    settings in the decomposeParDict file.

    Must be run on maximum number of source and destination processors.
    Balances mesh and writes new mesh to new time directory.

Usage

    - redistributePar [OPTION]

    \param -region regionName \n
    Distribute named region.

    \param -decompose \n
    Remove any existing \a processor subdirectories and decomposes the
    geometry. Equivalent to running without processor subdirectories.

    \param -reconstruct \n
    Reconstruct geometry (like reconstructParMesh). Equivalent to setting
    numberOfSubdomains 1 in the decomposeParDict

    \param -cellDist \n
    Write the cell distribution as a labelList, for use with 'manual'
    decomposition method or as a volScalarField for post-processing.

\*---------------------------------------------------------------------------*/

#include "fvMeshDistribute.H"
#include "fvMesh.H"
#include "decompositionMethod.H"
#include "PstreamReduceOps.H"
#include "fvCFD.H"
#include "IOmapDistributePolyMesh.H"
#include "IOobjectList.H"
#include "globalIndex.H"
#include "loadOrCreateMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Tolerance (as fraction of the bounding box). Needs to be fairly lax since
// usually meshes get written with limited precision (6 digits)
static const scalar defaultMergeTol = 1e-6;


// Get merging distance when matching face centres
scalar getMergeDistance
(
    const argList& args,
    const Time& runTime,
    const boundBox& bb
)
{
    scalar mergeTol = defaultMergeTol;
    args.optionReadIfPresent("mergeTol", mergeTol);

    scalar writeTol =
        Foam::pow(scalar(10.0), -scalar(IOstream::defaultPrecision()));

    Info<< "Merge tolerance : " << mergeTol << nl
        << "Write tolerance : " << writeTol << endl;

    if (runTime.writeFormat() == IOstream::ASCII && mergeTol < writeTol)
    {
        FatalErrorIn("getMergeDistance")
            << "Your current settings specify ASCII writing with "
            << IOstream::defaultPrecision() << " digits precision." << endl
            << "Your merging tolerance (" << mergeTol << ") is finer than this."
            << endl
            << "Please change your writeFormat to binary"
            << " or increase the writePrecision" << endl
            << "or adjust the merge tolerance (-mergeTol)."
            << exit(FatalError);
    }

    scalar mergeDist = mergeTol * bb.mag();

    Info<< "Overall meshes bounding box : " << bb << nl
        << "Relative tolerance          : " << mergeTol << nl
        << "Absolute matching distance  : " << mergeDist << nl
        << endl;

    return mergeDist;
}


void printMeshData(const polyMesh& mesh)
{
    // Collect all data on master

    globalIndex globalCells(mesh.nCells());
    labelListList patchNeiProcNo(Pstream::nProcs());
    labelListList patchSize(Pstream::nProcs());
    const labelList& pPatches = mesh.globalData().processorPatches();
    patchNeiProcNo[Pstream::myProcNo()].setSize(pPatches.size());
    patchSize[Pstream::myProcNo()].setSize(pPatches.size());
    forAll(pPatches, i)
    {
        const processorPolyPatch& ppp = refCast<const processorPolyPatch>
        (
            mesh.boundaryMesh()[pPatches[i]]
        );
        patchNeiProcNo[Pstream::myProcNo()][i] = ppp.neighbProcNo();
        patchSize[Pstream::myProcNo()][i] = ppp.size();
    }
    Pstream::gatherList(patchNeiProcNo);
    Pstream::gatherList(patchSize);


    // Print stats

    globalIndex globalBoundaryFaces(mesh.nFaces()-mesh.nInternalFaces());

    label maxProcCells = 0;
    label totProcFaces = 0;
    label maxProcPatches = 0;
    label totProcPatches = 0;
    label maxProcFaces = 0;

    for (label procI = 0; procI < Pstream::nProcs(); procI++)
    {
        Info<< endl
            << "Processor " << procI << nl
            << "    Number of cells = " << globalCells.localSize(procI)
            << endl;

        label nProcFaces = 0;

        const labelList& nei = patchNeiProcNo[procI];

        forAll(patchNeiProcNo[procI], i)
        {
            Info<< "    Number of faces shared with processor "
                << patchNeiProcNo[procI][i] << " = " << patchSize[procI][i]
                << endl;

            nProcFaces += patchSize[procI][i];
        }

        Info<< "    Number of processor patches = " << nei.size() << nl
            << "    Number of processor faces = " << nProcFaces << nl
            << "    Number of boundary faces = "
            << globalBoundaryFaces.localSize(procI)-nProcFaces << endl;

        maxProcCells = max(maxProcCells, globalCells.localSize(procI));
        totProcFaces += nProcFaces;
        totProcPatches += nei.size();
        maxProcPatches = max(maxProcPatches, nei.size());
        maxProcFaces = max(maxProcFaces, nProcFaces);
    }

    // Stats

    scalar avgProcCells = scalar(globalCells.size())/Pstream::nProcs();
    scalar avgProcPatches = scalar(totProcPatches)/Pstream::nProcs();
    scalar avgProcFaces = scalar(totProcFaces)/Pstream::nProcs();

    // In case of all faces on one processor. Just to avoid division by 0.
    if (totProcPatches == 0)
    {
        avgProcPatches = 1;
    }
    if (totProcFaces == 0)
    {
        avgProcFaces = 1;
    }

    Info<< nl
        << "Number of processor faces = " << totProcFaces/2 << nl
        << "Max number of cells = " << maxProcCells
        << " (" << 100.0*(maxProcCells-avgProcCells)/avgProcCells
        << "% above average " << avgProcCells << ")" << nl
        << "Max number of processor patches = " << maxProcPatches
        << " (" << 100.0*(maxProcPatches-avgProcPatches)/avgProcPatches
        << "% above average " << avgProcPatches << ")" << nl
        << "Max number of faces between processors = " << maxProcFaces
        << " (" << 100.0*(maxProcFaces-avgProcFaces)/avgProcFaces
        << "% above average " << avgProcFaces << ")" << nl
        << endl;
}


// Debugging: write volScalarField with decomposition for post processing.
void writeDecomposition
(
    const word& name,
    const fvMesh& mesh,
    const labelList& decomp
)
{
    // Write the decomposition as labelList for use with 'manual'
    // decomposition method.
    labelIOList cellDecomposition
    (
        IOobject
        (
            "cellDecomposition",
            mesh.pointsInstance(),  // mesh read from pointsInstance
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        decomp
    );
    cellDecomposition.write();

    Info<< "Writing wanted cell distribution to volScalarField " << name
        << " for postprocessing purposes." << nl << endl;

    volScalarField procCells
    (
        IOobject
        (
            name,
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE,
            false                   // do not register
        ),
        mesh,
        dimensionedScalar(name, dimless, -1),
        zeroGradientFvPatchScalarField::typeName
    );

    forAll(procCells, cI)
    {
        procCells[cI] = decomp[cI];
    }
    procCells.write();
}


void determineDecomposition
(
    const Time& baseRunTime,
    const fvMesh& mesh,
    const bool writeCellDist,
    const bool writeUndecomposed,

    label& nDestProcs,
    labelList& decomp
)
{
    // Read decomposeParDict (on all processors)
    //bool oldParRun = Pstream::parRun();
    //Pstream::parRun() = false;
    IOdictionary decompositionDict
    (
        IOobject
        (
            "decomposeParDict",
            mesh.time().system(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );
    //Pstream::parRun() = oldParRun;


    // Create decompositionMethod and new decomposition
    autoPtr<decompositionMethod> decomposer
    (
        decompositionMethod::New
        (
            decompositionDict
        )
    );

    if (!decomposer().parallelAware())
    {
        WarningIn("determineDecomposition(..)")
            << "You have selected decomposition method "
            << decomposer().typeName
            << " which does" << endl
            << "not synchronise the decomposition across"
            << " processor patches." << endl
            << "    You might want to select a decomposition method"
            << " which is aware of this. Continuing."
            << endl;
    }

    nDestProcs = decomposer().nDomains();
    decomp = decomposer().decompose(mesh, mesh.cellCentres());

    // Dump decomposition to volScalarField
    if (writeCellDist)
    {
        // Note: on master make sure to write to processor0
        if (writeUndecomposed && Pstream::master())
        {
            const fileName oldCaseName(mesh.time().caseName());
            Time& tm = const_cast<Time&>(mesh.time());

            tm.TimePaths::caseName() = baseRunTime.caseName();
            writeDecomposition("cellDist", mesh, decomp);
            tm.TimePaths::caseName() = oldCaseName;
        }
        else
        {
            writeDecomposition("cellDist", mesh, decomp);
        }
    }
}


// Write addressing if decomposing (1 to many) or reconstructing (many to 1)
void writeProcAddressing
(
    const bool decompose,
    const fvMesh& mesh,
    const mapDistributePolyMesh& map
)
{
    Info<< "Writing procXXXAddressing files to " << mesh.facesInstance()
        << endl;

    labelIOList cellMap
    (
        IOobject
        (
            "cellProcAddressing",
            mesh.facesInstance(),
            polyMesh::meshSubDir,
            mesh,
            IOobject::NO_READ
        ),
        0
    );

    labelIOList faceMap
    (
        IOobject
        (
            "faceProcAddressing",
            mesh.facesInstance(),
            polyMesh::meshSubDir,
            mesh,
            IOobject::NO_READ
        ),
        0
    );

    labelIOList pointMap
    (
        IOobject
        (
            "pointProcAddressing",
            mesh.pointsInstance(),
            polyMesh::meshSubDir,
            mesh,
            IOobject::NO_READ
        ),
        0
    );

    labelIOList patchMap
    (
        IOobject
        (
            "boundaryProcAddressing",
            mesh.pointsInstance(),
            polyMesh::meshSubDir,
            mesh,
            IOobject::NO_READ
        ),
        0
    );

    // Decomposing: see how cells moved from undecomposed case
    if (decompose)
    {
        cellMap = identity(map.nOldCells());
        map.distributeCellData(cellMap);

        faceMap = identity(map.nOldFaces());
        {
            const mapDistribute& faceDistMap = map.faceMap();

            if (faceDistMap.subHasFlip() || faceDistMap.constructHasFlip())
            {
                // Offset by 1
                faceMap = faceMap + 1;
            }
            // Apply face flips
            mapDistributeBase::distribute
            (
                Pstream::nonBlocking,
                List<labelPair>(),
                faceDistMap.constructSize(),
                faceDistMap.subMap(),
                faceDistMap.subHasFlip(),
                faceDistMap.constructMap(),
                faceDistMap.constructHasFlip(),
                faceMap,
                flipLabelOp()
            );
        }

        pointMap = identity(map.nOldPoints());
        map.distributePointData(pointMap);

        patchMap = identity(map.oldPatchSizes().size());
        const mapDistribute& patchDistMap = map.patchMap();
        // Use explicit distribute since we need to provide a null value
        // (for new patches) and this is the only call that allow us to
        // provide one ...
        mapDistributeBase::distribute
        (
            Pstream::nonBlocking,
            List<labelPair>(),
            patchDistMap.constructSize(),
            patchDistMap.subMap(),
            patchDistMap.subHasFlip(),
            patchDistMap.constructMap(),
            patchDistMap.constructHasFlip(),
            patchMap,
            eqOp<label>(),
            flipOp(),
            -1,
            UPstream::msgType()
        );
    }
    else    // if (nDestProcs == 1)
    {
        cellMap = identity(mesh.nCells());
        map.cellMap().reverseDistribute(map.nOldCells(), cellMap);

        faceMap = identity(mesh.nFaces());
        {
            const mapDistribute& faceDistMap = map.faceMap();

            if (faceDistMap.subHasFlip() || faceDistMap.constructHasFlip())
            {
                // Offset by 1
                faceMap = faceMap + 1;
            }

            mapDistributeBase::distribute
            (
                Pstream::nonBlocking,
                List<labelPair>(),
                map.nOldFaces(),
                faceDistMap.constructMap(),
                faceDistMap.constructHasFlip(),
                faceDistMap.subMap(),
                faceDistMap.subHasFlip(),
                faceMap,
                flipLabelOp()
            );
        }

        pointMap = identity(mesh.nPoints());
        map.pointMap().reverseDistribute(map.nOldPoints(), pointMap);

        const mapDistribute& patchDistMap = map.patchMap();
        patchMap = identity(mesh.boundaryMesh().size());
        patchDistMap.reverseDistribute
        (
            map.oldPatchSizes().size(),
            -1,
            patchMap
        );
    }

    bool cellOk = cellMap.write();
    bool faceOk = faceMap.write();
    bool pointOk = pointMap.write();
    bool patchOk = patchMap.write();
    if (!cellOk || !faceOk || !pointOk || !patchOk)
    {
        WarningIn
        (
            "void writeProcAddressing"
            "(const bool, const fvMesh&, const mapDistributePolyMesh&)"
        )   << "Failed to write " << cellMap.objectPath()
            << ", " << faceMap.objectPath()
            << ", " << pointMap.objectPath()
            << ", " << patchMap.objectPath()
            << endl;
    }
}


// Read vol or surface fields
template<class GeoField>
void readFields
(
    const boolList& haveMesh,
    const fvMesh& mesh,
    const autoPtr<fvMeshSubset>& subsetterPtr,
    IOobjectList& allObjects,
    PtrList<GeoField>& fields
)
{
    //typedef GeometricField<T, fvPatchField, Mesh> fldType;

    // Get my objects of type
    IOobjectList objects(allObjects.lookupClass(GeoField::typeName));
    // Check that we all have all objects
    wordList objectNames = objects.toc();
    // Get master names
    wordList masterNames(objectNames);
    Pstream::scatter(masterNames);

    if (haveMesh[Pstream::myProcNo()] && objectNames != masterNames)
    {
        FatalErrorIn("readFields()")
            << "differing fields of type " << GeoField::typeName
            << " on processors." << endl
            << "Master has:" << masterNames << endl
            << Pstream::myProcNo() << " has:" << objectNames
            << abort(FatalError);
    }

    fields.setSize(masterNames.size());

    // Have master send all fields to processors that don't have a mesh
    if (Pstream::master())
    {
        forAll(masterNames, i)
        {
            const word& name = masterNames[i];
            IOobject& io = *objects[name];
            io.writeOpt() = IOobject::AUTO_WRITE;

            // Load field
            fields.set(i, new GeoField(io, mesh));

            // Create zero sized field and send
            if (subsetterPtr.valid())
            {
                tmp<GeoField> tsubfld = subsetterPtr().interpolate(fields[i]);

                // Send to all processors that don't have a mesh
                for (label procI = 1; procI < Pstream::nProcs(); procI++)
                {
                    if (!haveMesh[procI])
                    {
                        OPstream toProc(Pstream::blocking, procI);
                        toProc<< tsubfld();
                    }
                }
            }
        }
    }
    else if (!haveMesh[Pstream::myProcNo()])
    {
        // Don't have mesh (nor fields). Receive empty field from master.

        forAll(masterNames, i)
        {
            const word& name = masterNames[i];

            // Receive field
            IPstream fromMaster(Pstream::blocking, Pstream::masterNo());
            dictionary fieldDict(fromMaster);

            fields.set
            (
                i,
                new GeoField
                (
                    IOobject
                    (
                        name,
                        mesh.time().timeName(),
                        mesh,
                        IOobject::NO_READ,
                        IOobject::AUTO_WRITE
                    ),
                    mesh,
                    fieldDict
                )
            );

            //// Write it for next time round (since mesh gets written as well)
            //fields[i].write();
        }
    }
    else
    {
        // Have mesh so just try to load
        forAll(masterNames, i)
        {
            const word& name = masterNames[i];
            IOobject& io = *objects[name];
            io.writeOpt() = IOobject::AUTO_WRITE;

            // Load field
            fields.set(i, new GeoField(io, mesh));
        }
    }
}


// Debugging: compare two fields.
void compareFields
(
    const scalar tolDim,
    const volVectorField& a,
    const volVectorField& b
)
{
    forAll(a, cellI)
    {
        if (mag(b[cellI] - a[cellI]) > tolDim)
        {
            FatalErrorIn
            (
                "compareFields"
                "(const scalar, const volVectorField&, const volVectorField&)"
            )   << "Did not map volVectorField correctly:" << nl
                << "cell:" << cellI
                << " transfer b:" << b[cellI]
                << " real cc:" << a[cellI]
                << abort(FatalError);
        }
    }
    forAll(a.boundaryField(), patchI)
    {
        // We have real mesh cellcentre and
        // mapped original cell centre.

        const fvPatchVectorField& aBoundary =
            a.boundaryField()[patchI];

        const fvPatchVectorField& bBoundary =
            b.boundaryField()[patchI];

        if (!bBoundary.coupled())
        {
            forAll(aBoundary, i)
            {
                if (mag(aBoundary[i] - bBoundary[i]) > tolDim)
                {
                    WarningIn
                    (
                        "compareFields"
                        "(const scalar, const volVectorField&"
                        ", const volVectorField&)"
                    )   << "Did not map volVectorField correctly:"
                        << endl
                        << "patch:" << patchI << " patchFace:" << i
                        << " cc:" << endl
                        << "    real    :" << aBoundary[i] << endl
                        << "    mapped  :" << bBoundary[i] << endl
                        << "This might be just a precision entry"
                        << " on writing the mesh." << endl;
                        //<< abort(FatalError);
                }
            }
        }
    }
}


int main(int argc, char *argv[])
{
#   include "addRegionOption.H"
#   include "addOverwriteOption.H"
    argList::addBoolOption("decompose", "decompose case");
    argList::addBoolOption("reconstruct", "reconstruct case");
    argList::addOption
    (
        "mergeTol",
        "scalar",
        "specify the merge distance relative to the bounding box size "
        "(default 1e-6)"
    );
    argList::addBoolOption
    (
        "cellDist",
        "write cell distribution as a labelList - for use with 'manual' "
        "decomposition method or as a volScalarField for post-processing."
    );


    // Handle arguments
    // ~~~~~~~~~~~~~~~~
    // (replacement for setRootCase that does not abort)

    Foam::argList args(argc, argv);
    bool decompose = args.optionFound("decompose");
    const bool reconstruct = args.optionFound("reconstruct");
    bool overwrite = args.optionFound("overwrite");
    bool writeCellDist = args.optionFound("cellDist");

    if (decompose)
    {
        Info<< "Decomposing case (like decomposePar)" << nl << endl;
        if (reconstruct)
        {
            FatalErrorIn(args.executable())
                << "Cannot specify both -decompose and -reconstruct"
                << exit(FatalError);
        }
    }
    else if (reconstruct)
    {
        Info<< "Reconstructing case (like reconstructParMesh)" << nl << endl;
    }


    if (decompose || reconstruct)
    {
        if (!overwrite)
        {
            WarningIn(args.executable())
                << "Working in decompose or reconstruction mode automatically"
                << " implies -overwrite" << nl << endl;
            overwrite = true;
        }
    }


    if (!Pstream::parRun())
    {
        FatalErrorIn(args.executable())
            << ": This utility can only be run parallel"
            << exit(FatalError);
    }


    if (!isDir(args.rootPath()))
    {
        FatalErrorIn(args.executable())
            << ": cannot open root directory " << args.rootPath()
            << exit(FatalError);
    }

    if (decompose && isDir(args.path()))
    {
        Info<< "Removing existing processor directories" << endl;
        rmDir(args.path());
    }


    // Construct time
    // ~~~~~~~~~~~~~~

    // Make sure we do not use the master-only reading. The system/controlDict
    // itself is ok but the uniform/time directory is not ok
    regIOobject::fileModificationChecking = regIOobject::timeStamp;

    // Switch off any reductions since if there are some processors without
    // time directories they will read the undecomposed one and if you've
    // selected latestTime you will have different times on different processors
    // (Note: time set later on)
    bool oldParRun = Pstream::parRun();
    Pstream::parRun() = false;

#   include "createTime.H"
    Pstream::parRun() = oldParRun;
    runTime.functionObjects().off();


    // Construct undecomposed Time
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // This will read the same controlDict but might have a different
    // set of times so e.g. latestTime will work
    Info<< "Create undecomposed database"<< nl << endl;
    Time baseRunTime
    (
        runTime.controlDict(),
        runTime.rootPath(),
        runTime.globalCaseName(),
        runTime.system(),
        runTime.constant(),
        false                   // enableFunctionObjects
    );



    // Determine any region
    word regionName = polyMesh::defaultRegion;
    fileName meshSubDir;

    if (args.optionReadIfPresent("region", regionName))
    {
        meshSubDir = regionName/polyMesh::meshSubDir;
    }
    else
    {
        meshSubDir = polyMesh::meshSubDir;
    }
    Info<< "Using mesh subdirectory " << meshSubDir << nl << endl;



    // Create any processor directories that do not exist
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Special handling for the master to force it reading from the
    // undecomposed case

    // Saved casename on master
    fileName masterCaseName;

    if (!isDir(args.path()))
    {
        if (Pstream::master())
        {
            // No processor0 -> decompose mode
            decompose = true;

            Info<< "Setting caseName to " << baseRunTime.caseName()
                << " to read undecomposed mesh and fields." << endl;
            masterCaseName = runTime.caseName();
            runTime.TimePaths::caseName() = baseRunTime.caseName();
        }
        else
        {
            // Create slave directory if non-existing
            mkDir(args.path());
        }
    }

    // decompose option possibly switched on if no processor0 detected
    Pstream::scatter(decompose);


    // Time coming from processor0 (or undecomposed if no processor0)
    scalar masterTime = runTime.value();
    if (decompose)
    {
        // Copy base time. This is to handle e.g. startTime = latestTime
        // which will not do anything if there are no processor directories
        masterTime = baseRunTime.value();
    }
    Pstream::scatter(masterTime);
    Info<< "Setting time to that of master or undecomposed case : "
        << masterTime << endl;
    runTime.setTime(masterTime, 0);


    Info<< "Time = " << runTime.timeName() << endl;



    // Get time instance directory
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // At this point we should be able to read at least a mesh on
    // processor0 (since caseName was changed to point to it)

    fileName masterInstDir;
    if (Pstream::master())
    {
        masterInstDir = runTime.findInstance
        (
            meshSubDir,
            "points",
            IOobject::READ_IF_PRESENT
        );
    }
    Pstream::scatter(masterInstDir);

    // Check who has a mesh
    const fileName meshPath = runTime.path()/masterInstDir/meshSubDir;

    Info<< "Found points in " << runTime.caseName()/masterInstDir/meshSubDir
        << nl << endl;


    boolList haveMesh(Pstream::nProcs(), false);
    haveMesh[Pstream::myProcNo()] = isDir(meshPath);
    Pstream::gatherList(haveMesh);
    Pstream::scatterList(haveMesh);
    Info<< "Per processor mesh availability : " << haveMesh << endl;
    const bool allHaveMesh = (findIndex(haveMesh, false) == -1);


    // Load mesh (or create dummy one)
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    autoPtr<fvMesh> meshPtr = loadOrCreateMesh
    (
        IOobject
        (
            regionName,
            masterInstDir,
            runTime,
            Foam::IOobject::MUST_READ
        )
    );

    fvMesh& mesh = meshPtr();

    // Print some statistics
    Info<< "Before distribution:" << endl;
    printMeshData(mesh);


    // Determine decomposition
    // ~~~~~~~~~~~~~~~~~~~~~~~

    label nDestProcs;
    labelList finalDecomp;
    {
        if (reconstruct)
        {
            // Move all to master processor
            nDestProcs = 1;
            finalDecomp = labelList(mesh.nCells(), 0);
        }
        else
        {
            determineDecomposition
            (
                baseRunTime,
                mesh,
                writeCellDist,
                decompose,  //writeUndecomposed,

                nDestProcs,
                finalDecomp
            );
        }
    }



    // Create 0 sized mesh to do all the generation of zero sized
    // fields on processors that have zero sized meshes. Note that this is
    // only nessecary on master but since polyMesh construction with
    // Pstream::parRun does parallel comms we have to do it on all
    // processors
    autoPtr<fvMeshSubset> subsetterPtr;

    if (!allHaveMesh)
    {
        // Find last non-processor patch.
        const polyBoundaryMesh& patches = mesh.boundaryMesh();

        label nonProcI = -1;

        forAll(patches, patchI)
        {
            if (isA<processorPolyPatch>(patches[patchI]))
            {
                break;
            }
            nonProcI++;
        }

        if (nonProcI == -1)
        {
            FatalErrorIn(args.executable())
                << "Cannot find non-processor patch on processor "
                << Pstream::myProcNo() << endl
                << " Current patches:" << patches.names() << abort(FatalError);
        }

        // Subset 0 cells, no parallel comms. This is used to create zero-sized
        // fields.
        subsetterPtr.reset(new fvMeshSubset(mesh));
        subsetterPtr().setLargeCellSubset(labelHashSet(0), nonProcI, false);
    }


    // Get original objects (before incrementing time!)
    IOobjectList objects(mesh, runTime.timeName());

    Info<< "From time " << runTime.timeName()
        << " have objects:" << objects.names() << endl;

    // We don't want to map the decomposition (mapping already tested when
    // mapping the cell centre field)
    IOobjectList::iterator iter = objects.find("cellDist");
    if (iter != objects.end())
    {
        objects.erase(iter);
    }


    // volFields

    PtrList<volScalarField> volScalarFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        volScalarFields
    );

    PtrList<volVectorField> volVectorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        volVectorFields
    );

    PtrList<volSphericalTensorField> volSphereTensorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        volSphereTensorFields
    );

    PtrList<volSymmTensorField> volSymmTensorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        volSymmTensorFields
    );

    PtrList<volTensorField> volTensorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        volTensorFields
    );


    // surfaceFields

    PtrList<surfaceScalarField> surfScalarFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        surfScalarFields
    );

    PtrList<surfaceVectorField> surfVectorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        surfVectorFields
    );

    PtrList<surfaceSphericalTensorField> surfSphereTensorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        surfSphereTensorFields
    );

    PtrList<surfaceSymmTensorField> surfSymmTensorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        surfSymmTensorFields
    );

    PtrList<surfaceTensorField> surfTensorFields;
    readFields
    (
        haveMesh,
        mesh,
        subsetterPtr,
        objects,
        surfTensorFields
    );


    // Now we've read all. Reset caseName on master
    if (Pstream::master() && masterCaseName.size())
    {
        Info<< "Restoring caseName to " << masterCaseName << endl;
        runTime.TimePaths::caseName() = masterCaseName;
        masterCaseName.clear();
    }

    // Debugging: Create additional volField that will be mapped.
    // Used to test correctness of mapping
    //volVectorField mapCc("mapCc", 1*mesh.C());

    // Global matching tolerance
    const scalar tolDim = getMergeDistance
    (
        args,
        runTime,
        mesh.bounds()
    );

    // Mesh distribution engine
    fvMeshDistribute distributor(mesh, tolDim);


    IOmapDistributePolyMesh map
    (
        IOobject
        (
            "procAddressing",
            mesh.facesInstance(),
            polyMesh::meshSubDir,
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        )
    );
    {
        // Do actual sending/receiving of mesh
        autoPtr<mapDistributePolyMesh> rawMap = distributor.distribute
        (
            finalDecomp
        );
        map.transfer(rawMap());
    }


    //// Distribute any non-registered data accordingly
    //map.distributeFaceData(faceCc);


    // Print some statistics
    Info<< "After distribution:" << endl;
    printMeshData(mesh);


    // Set the minimum write precision
    IOstream::defaultPrecision(max(10u, IOstream::defaultPrecision()));


    if (!overwrite)
    {
        runTime++;
        mesh.setInstance(runTime.timeName());
    }
    else
    {
        mesh.setInstance(masterInstDir);
    }


    if (nDestProcs == 1)
    {
        if (Pstream::master())
        {
            Info<< "Setting caseName to " << baseRunTime.caseName()
                << " to write reconstructed mesh and fields." << endl;
            fileName masterCaseName = runTime.caseName();
            runTime.TimePaths::caseName() = baseRunTime.caseName();

            mesh.write();

            // Now we've written all. Reset caseName on master
            Info<< "Restoring caseName to " << masterCaseName << endl;
            runTime.TimePaths::caseName() = masterCaseName;
        }
    }
    else
    {
        mesh.write();
    }
    Info<< "Written redistributed mesh to " << mesh.pointsInstance() << nl
        << endl;


    if (decompose || nDestProcs == 1)
    {
        writeProcAddressing(decompose, mesh, map);
    }



    // Debugging: test mapped cellcentre field.
    //compareFields(tolDim, mesh.C(), mapCc);


    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
