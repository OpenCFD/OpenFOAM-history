/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "externalCoupled.H"
#include "OSspecific.H"
#include "IFstream.H"
#include "OFstream.H"
#include "volFields.H"
#include "externalCoupledMixedFvPatchField.H"
#include "mixedFvPatchFields.H"
#include "globalIndex.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(externalCoupled, 0);
}

Foam::word Foam::externalCoupled::lockName = "OpenFOAM";

Foam::string Foam::externalCoupled::patchKey = "# Patch: ";

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::fileName Foam::externalCoupled::baseDir() const
{
    fileName result(commsDir_);
    result.clean();

    return result;
}


Foam::fileName Foam::externalCoupled::groupDir
(
    const fileName& commsDir,
    const word& regionName,
    const wordRe& groupName
)
{
    fileName result(commsDir/regionName/string::validate<fileName>(groupName));
    result.clean();

    return result;
}


Foam::fileName Foam::externalCoupled::lockFile() const
{
    return fileName(baseDir()/(lockName + ".lock"));
}


void Foam::externalCoupled::createLockFile() const
{
    if (!Pstream::master())
    {
        return;
    }

    const fileName fName(lockFile());
    IFstream is(fName);

    // only create lock file if it doesn't already exist
    if (!is.good())
    {
        if (log_)
        {
            Info<< type() << ": creating lock file" << endl;
        }

        OFstream os(fName);
        os  << "lock file";
        os.flush();
    }
}


void Foam::externalCoupled::removeLockFile() const
{
    if (!Pstream::master())
    {
        return;
    }

    if (log_)
    {
        Info<< type() << ": removing lock file" << endl;
    }

    rm(lockFile());
}


void Foam::externalCoupled::removeReadFiles() const
{
    if (!Pstream::master())
    {
        return;
    }

    if (log_)
    {
        Info<< type() << ": removing all read files" << endl;
    }

    forAll(regionNames_, regionI)
    {
        const word& regionName = regionNames_[regionI];
        const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);
        const labelList& groups = regionToGroups_[regionName];
        forAll(groups, i)
        {
            label groupI = groups[i];
            const wordRe& groupName = groupNames_[groupI];

            forAll(groupReadFields_[groupI], fieldI)
            {
                const word& fieldName = groupReadFields_[groupI][fieldI];
                rm
                (
                    groupDir(commsDir_, mesh.dbDir(), groupName)
                  / fieldName + ".in"
                );
            }
        }
    }
}


void Foam::externalCoupled::removeWriteFiles() const
{
    if (!Pstream::master())
    {
        return;
    }

    if (log_)
    {
        Info<< type() << ": removing all write files" << endl;
    }

    forAll(regionNames_, regionI)
    {
        const word& regionName = regionNames_[regionI];
        const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);
        const labelList& groups = regionToGroups_[regionName];
        forAll(groups, i)
        {
            label groupI = groups[i];
            const wordRe& groupName = groupNames_[groupI];

            forAll(groupWriteFields_[groupI], fieldI)
            {
                const word& fieldName = groupWriteFields_[groupI][fieldI];
                rm
                (
                    groupDir(commsDir_, mesh.dbDir(), groupName)
                  / fieldName + ".out"
                );
            }
        }
    }
}


void Foam::externalCoupled::wait() const
{
    const fileName fName(lockFile());
    label found = 0;
    label totalTime = 0;

    if (log_)
    {
        Info<< type() << ": beginning wait for lock file " << fName << endl;
    }

    while (found == 0)
    {
        if (Pstream::master())
        {
            if (totalTime > timeOut_)
            {
                FatalErrorIn
                (
                    "void "
                    "Foam::externalCoupled::wait() "
                    "const"
                )
                    << "Wait time exceeded time out time of " << timeOut_
                    << " s" << abort(FatalError);
            }

            IFstream is(fName);

            if (is.good())
            {
                found++;

                if (log_)
                {
                    Info<< type() << ": found lock file " << fName << endl;
                }
            }
            else
            {
                sleep(waitInterval_);
                totalTime += waitInterval_;

                if (log_)
                {
                    Info<< type() << ": wait time = " << totalTime << endl;
                }
            }
        }

        // prevent other procs from racing ahead
        reduce(found, sumOp<label>());
    }
}


void Foam::externalCoupled::readColumns
(
    const label nRows,
    const label nColumns,
    autoPtr<IFstream>& masterFilePtr,
    List<scalarField>& data
) const
{
    // Get sizes for all processors
    const globalIndex globalFaces(nRows);

    PstreamBuffers pBufs(Pstream::nonBlocking);
    if (Pstream::master())
    {
        string line;

        // Read data from file and send to destination processor

        for (label procI = 0; procI < Pstream::nProcs(); procI++)
        {
            // Temporary storage
            List<scalarField> values(nColumns);

            // Number of rows to read for processor procI
            label procNRows = globalFaces.localSize(procI);

            forAll(values, columnI)
            {
                values[columnI].setSize(procNRows);
            }

            for (label rowI = 0; rowI < procNRows; rowI++)
            {
                // Get a line
                masterFilePtr().getLine(line);

                IStringStream lineStr(line);

                for (label columnI = 0; columnI < nColumns; columnI++)
                {
                    lineStr >> values[columnI][rowI];
                }
            }

            // Send to procI
            UOPstream str(procI, pBufs);
            str << values;
        }
    }
    pBufs.finishedSends();

    // Read from PstreamBuffers
    UIPstream str(Pstream::masterNo(), pBufs);
    str >> data;
}


void Foam::externalCoupled::writeGeometry
(
    const fvMesh& mesh,
    const fileName& commsDir,
    const wordRe& groupName
)
{
    fileName dir(groupDir(commsDir, mesh.dbDir(), groupName));

    //if (log_)
    {
        Info<< typeName << ": writing geometry to " << dir << endl;
    }

    autoPtr<OFstream> osPointsPtr;
    autoPtr<OFstream> osFacesPtr;
    if (Pstream::master())
    {
        mkDir(dir);
        osPointsPtr.reset(new OFstream(dir/"patchPoints"));
        osFacesPtr.reset(new OFstream(dir/"patchFaces"));
    }

    const labelList patchIDs
    (
        mesh.boundaryMesh().patchSet
        (
            List<wordRe>(1, groupName)
        ).sortedToc()
    );

    forAll(patchIDs, i)
    {
        label patchI = patchIDs[i];

        const polyPatch& p = mesh.boundaryMesh()[patchI];

        labelList pointToGlobal;
        labelList uniquePointIDs;
        mesh.globalData().mergePoints
        (
            p.meshPoints(),
            p.meshPointMap(),
            pointToGlobal,
            uniquePointIDs
        );

        label procI = Pstream::myProcNo();

        List<pointField> allPoints(Pstream::nProcs());
        allPoints[procI] = pointField(mesh.points(), uniquePointIDs);
        Pstream::gatherList(allPoints);

        List<faceList> allFaces(Pstream::nProcs());
        faceList& patchFaces = allFaces[procI];
        patchFaces = p.localFaces();
        forAll(patchFaces, faceI)
        {
            inplaceRenumber(pointToGlobal, patchFaces[faceI]);
        }
        Pstream::gatherList(allFaces);

        if (Pstream::master())
        {
            pointField pts
            (
                ListListOps::combine<pointField>
                (
                    allPoints,
                    accessOp<pointField>()
                )
            );

            //if (log_)
            {
                Info<< typeName << ": for patch " << p.name()
                    << " writing " << pts.size() << " points to "
                    << osPointsPtr().name() << endl;
            }

            // Write points
            osPointsPtr() << patchKey.c_str() << p.name() << pts << endl;

            faceList fcs
            (
                ListListOps::combine<faceList>(allFaces, accessOp<faceList>())
            );

            //if (log_)
            {
                Info<< typeName << ": for patch " << p.name()
                    << " writing " << fcs.size() << " faces to "
                    << osFacesPtr().name() << endl;
            }

            // Write faces
            osFacesPtr() << patchKey.c_str() << p.name() << fcs << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::externalCoupled::externalCoupled
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    initByExternal_(true)
{
    read(dict);

    commsDir_.expand();

    if (Pstream::master())
    {
        mkDir(baseDir());
    }

    if (!initByExternal_)
    {
        createLockFile();
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::externalCoupled::~externalCoupled()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::externalCoupled::read(const dictionary& dict)
{
    dict.lookup("commsDir") >> commsDir_;
    commsDir_.expand();

    waitInterval_ = dict.lookupOrDefault("waitInterval", 1);
    timeOut_ = dict.lookupOrDefault("timeOut", 100*waitInterval_);
    calcFrequency_ = dict.lookupOrDefault("calcFrequency", 1);
    initByExternal_ = readBool(dict.lookup("initByExternal"));
    log_ = dict.lookupOrDefault("log", false);

    const dictionary& allRegionsDict = dict.subDict("regions");

    forAllConstIter(dictionary, allRegionsDict, iter)
    {
        if (!iter().isDict())
        {
            FatalIOErrorIn
            (
                "void Foam::externalCoupled::read(const dictionary&)",
                allRegionsDict
            )
                << "Regions must be specified in dictionary format"
                << exit(FatalIOError);
        }

        const word& regionName = iter().keyword();
        const dictionary& regionDict = iter().dict();
        regionNames_.append(regionName);

        forAllConstIter(dictionary, regionDict, regionIter)
        {
            if (!regionIter().isDict())
            {
                FatalIOErrorIn
                (
                    "void Foam::externalCoupled::read(const dictionary&)",
                    regionDict
                )
                    << "Regions must be specified in dictionary format"
                    << exit(FatalIOError);
            }
            const wordRe groupName(regionIter().keyword());
            const dictionary& groupDict = regionIter().dict();

            label nGroups = groupNames_.size();
            const wordList readFields(groupDict.lookup("readFields"));
            const wordList writeFields(groupDict.lookup("writeFields"));

            HashTable<labelList>::iterator fnd = regionToGroups_.find
            (
                regionName
            );
            if (fnd != regionToGroups_.end())
            {
                fnd().append(nGroups);
            }
            else
            {
                regionToGroups_.insert(regionName, labelList(1, nGroups));
            }
            groupNames_.append(groupName);
            groupReadFields_.append(readFields);
            groupWriteFields_.append(writeFields);

            // Pre-calculate the patchIDs
            const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);
            groupPatchIDs_.append
            (
                mesh.boundaryMesh().patchSet
                (
                    List<wordRe>(1, groupName)
                ).sortedToc()
            );
        }
    }


    // Print a bit
    if (log_)
    {
        forAll(regionNames_, regionI)
        {
            const word& regionName = regionNames_[regionI];
            const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);

            Info<< "Region: " << mesh.name() << endl << incrIndent;
            const labelList& groups = regionToGroups_[regionName];
            forAll(groups, i)
            {
                label groupI = groups[i];
                const wordRe& groupName = groupNames_[groupI];
                const labelList& patchIDs = groupPatchIDs_[groupI];

                Info<< indent << "group: " << groupName << "\t"
                    << " patches: " << patchIDs << endl
                    << incrIndent
                    << indent << "Reading fields: " << groupReadFields_[groupI]
                    << endl
                    << indent << "Writing fields: " << groupWriteFields_[groupI]
                    << endl
                    << decrIndent;
            }
            Info<< decrIndent;
        }
    }


    // Note: we should not have to make directories since the geometry
    //       should already be written but just make sure
    if (Pstream::master())
    {
        forAll(regionNames_, regionI)
        {
            const word& regionName = regionNames_[regionI];
            const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);
            const labelList& groups = regionToGroups_[regionName];
            forAll(groups, i)
            {
                label groupI = groups[i];
                const wordRe& groupName = groupNames_[groupI];

                fileName dir(groupDir(commsDir_, mesh.dbDir(), groupName));
                if (!isDir(dir))
                {
                    if (log_)
                    {
                        Info<< type() << ": creating comms dir " << dir << endl;
                    }
                    mkDir(dir);
                }
            }
        }
    }
}


void Foam::externalCoupled::readData()
{
    forAll(regionNames_, regionI)
    {
        const word& regionName = regionNames_[regionI];
        const labelList& groups = regionToGroups_[regionName];

        const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);

        forAll(groups, i)
        {
            label groupI = groups[i];
            const wordRe& groupName = groupNames_[groupI];
            const labelList& patchIDs = groupPatchIDs_[groupI];
            const wordList& fieldNames = groupReadFields_[groupI];

            forAll(fieldNames, fieldI)
            {
                const word& fieldName = fieldNames[fieldI];

                bool ok = readData<scalar>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || readData<vector>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || readData<sphericalTensor>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || readData<symmTensor>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || readData<tensor>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );

                if (!ok)
                {
                    WarningIn
                    (
                        "void Foam::externalCoupled::readData()"
                    )
                        << "Field " << fieldName << " in region " << mesh.name()
                        << " was not found." << endl;
                }
            }
        }
    }
}


void Foam::externalCoupled::writeData() const
{
    forAll(regionNames_, regionI)
    {
        const word& regionName = regionNames_[regionI];
        const labelList& groups = regionToGroups_[regionName];

        const fvMesh& mesh = obr_.time().lookupObject<fvMesh>(regionName);

        forAll(groups, i)
        {
            label groupI = groups[i];
            const wordRe& groupName = groupNames_[groupI];
            const labelList& patchIDs = groupPatchIDs_[groupI];
            const wordList& fieldNames = groupWriteFields_[groupI];

            forAll(fieldNames, fieldI)
            {
                const word& fieldName = fieldNames[fieldI];
                bool ok = writeData<scalar>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || writeData<vector>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || writeData<sphericalTensor>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || writeData<symmTensor>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );
                ok = ok || writeData<tensor>
                (
                    mesh,
                    groupName,
                    patchIDs,
                    fieldName
                );

                if (!ok)
                {
                    WarningIn
                    (
                        "void Foam::externalCoupled::writeData()"
                    )
                        << "Field " << fieldName << " in region " << mesh.name()
                        << " was not found." << endl;
                }
            }
        }
    }
}


void Foam::externalCoupled::initialise()
{
    if (initialised_)
    {
        return;
    }

    if (initByExternal_)
    {
        // Wait for initial data to be made available
        wait();

        // Eead data passed back from external source
        readData();
    }

    initialised_ = true;
}


void Foam::externalCoupled::execute()
{
    if (!initialised_ || obr_.time().timeIndex() % calcFrequency_ == 0)
    {
        // Initialise the coupling
        initialise();

        // Write data for external source
        writeData();

        // remove lock file, signalling external source to execute
        removeLockFile();

        // Wait for response
        wait();

        // Remove old data files from OpenFOAM
        removeWriteFiles();

        // Read data passed back from external source
        readData();

        // create lock file for external source
        createLockFile();
    }
}


void Foam::externalCoupled::end()
{
    if (Pstream::master())
    {
        // Remove old data files
        removeReadFiles();
        removeWriteFiles();
        removeLockFile();
    }
}


void Foam::externalCoupled::timeSet()
{
    // Do nothing - only valid on execute
}


void Foam::externalCoupled::write()
{
    // Do nothing - only valid on execute
}


// ************************************************************************* //
