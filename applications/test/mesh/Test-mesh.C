/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "argList.H"
#include "fvMesh.H"
#include "volFields.H"
#include "surfaceFields.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{

    #include "setRootCase.H"
    #include "createTime.H"


    // Test READ_IF_PRESENT construction of fvMesh
    if (false)
    {
        fileName meshDir = "polyMesh";

        pointField points
        (
            pointIOField
            (
                IOobject
                (
                    "points",
                    runTime.findInstance(meshDir, "points"),
                    polyMesh::meshSubDir,
                    runTime,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
        points += vector(111, 0.0, 0.0);


        faceList faces
        (
            faceIOList
            (
                IOobject
                (
                    "faces",
                    runTime.findInstance(meshDir, "faces"),
                    polyMesh::meshSubDir,
                    runTime,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );

        Pout<< "faces:" << faces.size() << endl;
        labelList owner
        (
            labelIOList
            (
                IOobject
                (
                    "owner",
                    runTime.findInstance(meshDir, "faces"),
                    polyMesh::meshSubDir,
                    runTime,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                )
            )
        );

        labelList neighbour
        (
            labelIOList
            (
                IOobject
                (
                    "neighbour",
                    runTime.findInstance(meshDir, "faces"),
                    polyMesh::meshSubDir,
                    runTime,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                )
            )
        );

        Pout<< "Create mesh for time = "
            << runTime.timeName() << nl << endl;

        fvMesh mesh
        (
            IOobject
            (
                fvMesh::defaultRegion,
                runTime.timeName(),
                runTime,
                IOobject::READ_IF_PRESENT
            ),
            xferMoveTo<pointField>(points),
            faces.xfer(),
            owner.xfer(),
            neighbour.xfer()
        );

        Info<< "points:" << mesh.points() << endl;
    }



    Info<< "Create mesh, no clear-out\n" << endl;
    fvMesh mesh
    (
        IOobject
        (
            fvMesh::defaultRegion,
            runTime.timeName(),
            runTime,
            IOobject::MUST_READ
        )
    );

    Info<< mesh.C() << endl;
    Info<< mesh.V() << endl;

    surfaceVectorField Cf = mesh.Cf();

    Info<< Cf << endl;

    // Test construct from cellShapes
    {
        pointField points(mesh.points());
        cellShapeList shapes(mesh.cellShapes());

        const polyBoundaryMesh& pbm = mesh.boundaryMesh();

        faceListList boundaryFaces(pbm.size());
        forAll(pbm, patchI)
        {
            boundaryFaces[patchI] = pbm[patchI];
        }
        wordList boundaryPatchNames(pbm.names());
        PtrList<dictionary> boundaryDicts(pbm.size());
        forAll(pbm, patchI)
        {
            OStringStream os;
            os << pbm[patchI];
            IStringStream is(os.str());
            boundaryDicts.set(patchI, new dictionary(is));
        }

        word defaultBoundaryPatchName = "defaultFaces";
        word defaultBoundaryPatchType = emptyPolyPatch::typeName;

        fvMesh newMesh
        (
            IOobject
            (
                "newMesh",
                runTime.timeName(),
                runTime,
                Foam::IOobject::NO_READ
            ),
            Xfer<pointField>(points),
            shapes,
            boundaryFaces,
            boundaryPatchNames,
            boundaryDicts,
            defaultBoundaryPatchName,
            defaultBoundaryPatchType
        );

        Info<< newMesh.C() << endl;
        Info<< newMesh.V() << endl;

        surfaceVectorField Cf = newMesh.Cf();

        Info<< Cf << endl;
    }


    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
