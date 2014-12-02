/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd.
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

#include "pointSmoothingMeshMover.H"
#include "addToRunTimeSelectionTable.H"
#include "syncTools.H"
#include "pointConstraints.H"
#include "zeroFixedValuePointPatchField.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(pointSmoothingMeshMover, 0);

    addToRunTimeSelectionTable
    (
        externalDisplacementMeshMover,
        pointSmoothingMeshMover,
        dictionary
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::pointSmoothingMeshMover::moveMesh
(
    const dictionary& moveDict,
    const label nAllowableErrors,
    labelList& checkFaces
)
{
    const label nRelaxIter = readLabel(moveDict.lookup("nRelaxIter"));

    meshMover_.setDisplacementPatchFields();

    Info<< typeName << " : Moving mesh ..." << endl;

    scalar oldErrorReduction = -1;

    bool meshOk = false;

    for (label iter = 0; iter < 2*nRelaxIter; ++ iter)
    {
        Info<< typeName << " : Iteration " << iter << endl;

        if (iter == nRelaxIter)
        {
            Info<< typeName
                << " : Displacement scaling for error reduction set to 0."
                << endl;
            oldErrorReduction = meshMover_.setErrorReduction(0.0);
        }

        if
        (
            meshMover_.scaleMesh
            (
                checkFaces,
                baffles_,
                meshMover_.paramDict(),
                moveDict,
                true,
                nAllowableErrors
            )
        )
        {
            Info<< typeName << " : Successfully moved mesh" << endl;
            meshOk = true;
            break;
        }
    }

    if (oldErrorReduction >= 0)
    {
        meshMover_.setErrorReduction(oldErrorReduction);
    }

    Info<< typeName << " : Finished moving mesh ..." << endl;

    return meshOk;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointSmoothingMeshMover::pointSmoothingMeshMover
(
    const dictionary& dict,
    const List<labelPair>& baffles,
    pointVectorField& pointDisplacement
)
:
    externalDisplacementMeshMover(dict, baffles, pointDisplacement),

    meshGeometry_(mesh()),

    pointSmoother_(pointSmoother::New(dict, pointDisplacement)),

    adaptPatchIDs_(getFixedValueBCs(pointDisplacement)),
    adaptPatchPtr_(getPatch(mesh(), adaptPatchIDs_)),

    scale_
    (
        IOobject
        (
            "scale",
            pointDisplacement.time().timeName(),
            pointDisplacement.db(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        pMesh(),
        dimensionedScalar("scale", dimless, 1.0)
    ),

    oldPoints_(mesh().points()),

    meshMover_
    (
        const_cast<polyMesh&>(mesh()),
        const_cast<pointMesh&>(pMesh()),
        adaptPatchPtr_(),
        pointDisplacement,
        scale_,
        oldPoints_,
        adaptPatchIDs_,
        dict
    ),

    fieldSmoother_(mesh())
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointSmoothingMeshMover::~pointSmoothingMeshMover()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::pointSmoothingMeshMover::move
(
    const dictionary& moveDict,
    const label nAllowableErrors,
    labelList& checkFaces
)
{
    // Update the point smoother displacements
    {
        meshGeometry_.correct
        (
            oldPoints_ + pointDisplacement().internalField(),
            checkFaces
        );

        pointSmoother_->update
        (
            checkFaces,
            oldPoints_,
            oldPoints_ + pointDisplacement().internalField(),
            meshGeometry_
        );

        pointConstraints::New
        (
            pointDisplacement().mesh()
        ).constrainDisplacement(pointDisplacement());
    }

    // Correct and smooth the patch displacements
    {
        const label nSmoothPatchThickness = readLabel
        (
            moveDict.lookup("nSmoothThickness")
        );

        const word minThicknessName = word(moveDict.lookup("minThicknessName"));

        scalarField zeroMinThickness;

        if (minThicknessName == "none")
        {
            zeroMinThickness = scalarField(adaptPatchPtr_().nPoints(), 0.0);
        }

        const scalarField& minThickness =
        (
            (minThicknessName == "none")
          ? zeroMinThickness
          : mesh().lookupObject<scalarField>(minThicknessName)
        );

        const PackedBoolList isPatchMasterPoint
        (
            meshRefinement::getMasterPoints
            (
                mesh(),
                adaptPatchPtr_().meshPoints()
            )
        );

        const PackedBoolList isPatchMasterEdge
        (
            meshRefinement::getMasterEdges
            (
                mesh(),
                adaptPatchPtr_().meshEdges(mesh().edges(), mesh().pointEdges())
            )
        );

        pointDisplacement().correctBoundaryConditions();

        vectorField displacement
        (
            pointDisplacement().internalField(),
            adaptPatchPtr_().meshPoints()
        );

        fieldSmoother_.minSmoothField
        (
            nSmoothPatchThickness,
            isPatchMasterPoint,
            isPatchMasterEdge,
            adaptPatchPtr_(),
            minThickness,
            displacement
        );

        forAll(displacement, patchPointI)
        {
            const label pointI(adaptPatchPtr_().meshPoints()[patchPointI]);
            pointDisplacement()[pointI] = displacement[patchPointI];
        }
    }

    return moveMesh(moveDict, nAllowableErrors, checkFaces);
}


void Foam::pointSmoothingMeshMover::movePoints
(
    const pointField& p
)
{
    externalDisplacementMeshMover::movePoints(p);

    // Update local data for new geometry
    adaptPatchPtr_().movePoints(p);

    // Update motionSmoother for new geometry
    meshMover_.movePoints();

    // Assume corrent mesh location is correct
    meshMover_.correct();
}


// ************************************************************************* //
