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
    potentialFoam

Group
    grpBasicSolvers

Description
    Potential flow solver.

    \heading Solver details
    The potential flow solution is typically employed to generate initial fields
    for full Navier-Stokes codes.  The flow is evolved using the equation:

    \f[
        \laplacian \Phi = \div(\vec{U})
    \f]

    Where:
    \vartable
        \Phi    | Velocity potential [m2/s]
        U       | Velocity [m/s]
    \endvartable

    The corresponding pressure field could be calculated from the divergence
    of the Euler equation:

    \f[
        \laplacian p + \div(\div(\vec{U}\otimes\vec{U})) = 0
    \f]

    but this generates excessive pressure variation in regions of large
    velocity gradient normal to the flow direction.  A better option is to
    calculate the pressure field corresponding to velocity variation along the
    stream-lines:

    \f[
        \laplacian p + \div(\vec{F}\cdot\div(\vec{U}\otimes\vec{U})) = 0
    \f]
    where the flow direction tensor \f$\vec{F}\f$ is obtained from
    \f[
        \vec{F} = \hat{\vec{U}}\otimes\hat{\vec{U}}
    \f]

    \heading Required fields
    \plaintable
        U       | Velocity [m/s]
    \endplaintable

    \heading Optional fields
    \plaintable
        Phi     | Velocity potential [m2/s]
                | Generated from p (if present) or U if not present
    \endplaintable

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "fvIOoptionList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addOption
    (
        "pName",
        "pName",
        "Name of the pressure field"
    );

    argList::addBoolOption
    (
        "writePhi",
        "write the final velocity potential field"
    );

    argList::addBoolOption
    (
        "writep",
        "calculate and write the Euler pressure field"
    );

    argList::addBoolOption
    (
        "initialiseUBCs",
        "initialise U boundary conditions"
    );

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readControls.H"
    #include "createFields.H"
    #include "createFvOptions.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl << "Calculating potential flow" << endl;

    // Since solver contains no time loop it would never execute
    // function objects so do it ourselves
    runTime.functionObjects().start();

    fvOptions.makeRelative(phi);

    adjustPhi(phi, U, Phi);

    for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++)
    {
        fvScalarMatrix PhiEqn
        (
            fvm::laplacian(dimensionedScalar("1", dimless, 1), Phi)
         ==
            fvc::div(phi)
        );

        PhiEqn.setReference(PhiRefCell, PhiRefValue);
        PhiEqn.solve();

        if (nonOrth == nNonOrthCorr)
        {
            phi -= PhiEqn.flux();
        }
    }

    fvOptions.makeAbsolute(phi);

    Info<< "continuity error = "
        << mag(fvc::div(phi))().weightedAverage(mesh.V()).value()
        << endl;

    U = fvc::reconstruct(phi);
    U.correctBoundaryConditions();

    Info<< "Interpolated U error = "
        << (sqrt(sum(sqr((fvc::interpolate(U) & mesh.Sf()) - phi)))
          /sum(mesh.magSf())).value()
        << endl;

    // Write U and phi
    U.write();
    phi.write();

    if (args.optionFound("writePhi"))
    {
        Phi.write();
    }

    // Calculate the pressure field from the Euler equation
    if (args.optionFound("writep"))
    {
        Info<< nl << "Calculating Euler pressure field" << endl;

        volScalarField magSqrU(magSqr(U));

        volScalarField divDivUU
        (
            // Euler equation form
            // fvc::div(fvc::div(phi, U), "div(div(phi,U))")

            // Euler equation filtered by the flow-direction
            fvc::div
            (
                (sqr(U)/(magSqrU + 1e-6*average(magSqrU))) & fvc::div(phi, U),
                "div(div(phi,U))"
            )
        );

        for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++)
        {
            fvScalarMatrix pEqn(fvm::laplacian(p) + divDivUU);

            pEqn.setReference(pRefCell, pRefValue);
            pEqn.solve();
        }

        p.write();
    }

    runTime.functionObjects().end();

    Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
        << "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
