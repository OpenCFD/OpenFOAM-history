/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenFOAM Foundation
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

#include "acceleratedLaplacianFvPointSmoother.H"
#include "fvmLaplacian.H"
#include "volPointInterpolation.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace pointSmoothers
{
    defineTypeNameAndDebug(acceleratedLaplacianFvPointSmoother, 0);
    addToRunTimeSelectionTable
    (
        pointSmoother,
        acceleratedLaplacianFvPointSmoother,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointSmoothers::acceleratedLaplacianFvPointSmoother::
acceleratedLaplacianFvPointSmoother
(
    const dictionary& dict,
    pointVectorField& pointDisplacement
)
:
    laplacianPointSmoother(dict, pointDisplacement),
    fvMotionSolverCore(pointSmoother::mesh()),
    cellDisplacement_
    (
        IOobject
        (
            "cellDisplacement",
            pointSmoother::mesh().time().timeName(),
            pointSmoother::mesh(),
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        fvMesh_,
        dimensionedVector("cellDisplacement", dimLength, vector::zero),
        cellMotionBoundaryTypes<vector>(pointDisplacement.boundaryField())
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointSmoothers::acceleratedLaplacianFvPointSmoother::
~acceleratedLaplacianFvPointSmoother()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::pointSmoothers::acceleratedLaplacianFvPointSmoother::calculate
(
    const labelList& facesToMove,
    const pointField& oldPoints,
    const pointField& currentPoints,
    polyMeshGeometry& meshGeometry
)
{
    pointDisplacement().correctBoundaryConditions();

    solve(fvm::laplacian(cellDisplacement()));

    volPointInterpolation::New(fvMesh_).interpolate
    (
        cellDisplacement(),
        pointDisplacement()
    );

    meshGeometry.correct
    (
        oldPoints + pointDisplacement().internalField(),
        facesToMove
    );

    laplacianPointSmoother::calculate
    (
        facesToMove,
        oldPoints,
        oldPoints + pointDisplacement().internalField(),
        meshGeometry
    );
}


// ************************************************************************* //
