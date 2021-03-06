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

#include "velocityDisplacementMotionSolver.H"
#include "displacementMotionSolver.H"
#include "fixedValuePointPatchField.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(velocityDisplacementMotionSolver, 0);

    addToRunTimeSelectionTable
    (
        motionSolver,
        velocityDisplacementMotionSolver,
        dictionary
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::wordList
Foam::velocityDisplacementMotionSolver::pointDisplacementBoundaryTypes() const
{
    const pointVectorField::GeometricBoundaryField& pmUbf
    (
        pointMotionU().boundaryField()
    );

    wordList cmUbf = pmUbf.types();

    forAll(pmUbf, patchI)
    {
        if (isA<fixedValuePointPatchField<vector> >(pmUbf[patchI]))
        {
            cmUbf[patchI] = fixedValuePointPatchField<vector>::typeName;
        }
    }

    return cmUbf;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::velocityDisplacementMotionSolver::velocityDisplacementMotionSolver
(
    const polyMesh& mesh,
    const IOdictionary& dict
)
:
    velocityMotionSolver(mesh, dict, typeName),
    displacementMotionSolverPtr_()
{
    pointIOField points0(displacementMotionSolver::points0IO(mesh));

    pointVectorField pointDisplacement
    (
        IOobject
        (
            "pointVelocityDisplacement",
            mesh.time().timeName(),
            mesh
        ),
        pointMotionU().mesh(),
        dimLength,
        pointDisplacementBoundaryTypes()
    );

    pointDisplacement.internalField() = mesh.points() - points0;

    displacementMotionSolverPtr_.set
    (
        dynamic_cast<displacementMotionSolver*>
        (
            displacementMotionSolver::New
            (
                coeffDict().lookup("solver"),
                mesh,
                IOdictionary
                (
                    IOobject
                    (
                        dict.name() + "Coeffs",
                        mesh.time().constant(),
                        mesh
                    ),
                    coeffDict()
                ),
                pointDisplacement,
                points0
            ).ptr()
        )
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::velocityDisplacementMotionSolver::~velocityDisplacementMotionSolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::tmp<Foam::pointField>
Foam::velocityDisplacementMotionSolver::curPoints() const
{
    return displacementMotionSolverPtr_->curPoints();
}


void Foam::velocityDisplacementMotionSolver::solve()
{
    movePoints(mesh().points());

    const scalar deltaT(mesh().time().deltaTValue());

    // Current and old point displacements
    pointVectorField& displacement
    (
        displacementMotionSolverPtr_->pointDisplacement()
    );
    const vectorField displacementOld
    (
        mesh().points() - displacementMotionSolverPtr_->points0()
    );

    // Update the velocity boundary conditions
    pointMotionU().correctBoundaryConditions();

    // Update the displacement boundary conditions
    forAll(pointMotionU().boundaryField(), patchI)
    {
        const pointPatchVectorField& patchField
        (
            pointMotionU().boundaryField()[patchI]
        );

        displacement.boundaryField()[patchI] ==
            patchField.patchInternalField()*deltaT
          + patchField.patchInternalField(displacementOld);
    }

    // Run the sub-solver
    displacementMotionSolverPtr_->solve();

    // Update the velocity
    pointMotionU().internalField() =
        (displacement.internalField() - displacementOld)/deltaT;
}


void Foam::velocityDisplacementMotionSolver::movePoints(const pointField& p)
{
    velocityMotionSolver::movePoints(p);

    displacementMotionSolverPtr_->movePoints(p);
}


void Foam::velocityDisplacementMotionSolver::updateMesh
(
    const mapPolyMesh& mpm
)
{
    velocityMotionSolver::updateMesh(mpm);

    displacementMotionSolverPtr_->updateMesh(mpm);
}


// ************************************************************************* //
