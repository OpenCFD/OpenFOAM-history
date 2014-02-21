{
    surfaceScalarField phiAlpha
    (
        IOobject
        (
            "phiAlpha",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("0", phi.dimensions(), 0)
    );

    surfaceScalarField phir
    (
        rhoc*(mesh.Sf() & fvc::interpolate(Vdj/rho))
    );

    if (nAlphaSubCycles > 1)
    {
        dimensionedScalar totalDeltaT = runTime.deltaT();
        surfaceScalarField phiAlphaSum
        (
            IOobject
            (
                "phiAlphaSum",
                runTime.timeName(),
                mesh
            ),
            mesh,
            dimensionedScalar("0", phi.dimensions(), 0)
        );

        for
        (
            subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
            !(++alphaSubCycle).end();
        )
        {
            #include "alphaEqn.H"
            phiAlphaSum += (runTime.deltaT()/totalDeltaT)*phiAlpha;
        }

        phiAlpha = phiAlphaSum;
    }
    else
    {
        #include "alphaEqn.H"
    }

    // Apply the diffusion term separately to allow implicit solution
    // and boundedness of the explicit advection
    {
        fvScalarMatrix alpha1Eqn
        (
            fvm::ddt(alpha1) - fvc::ddt(alpha1)
          - fvm::laplacian(mut/rho, alpha1)
        );

        alpha1Eqn.solve();

        phiAlpha += alpha1Eqn.flux();
    }

    Info<< "Phase-1 volume fraction = "
        << alpha1.weightedAverage(mesh.Vsc()).value()
        << "  Min(alpha) = " << min(alpha1).value()
        << "  Max(alpha) = " << max(alpha1).value()
        << endl;

    rhoPhi = phiAlpha*(rhod - rhoc) + phi*rhoc;
    rho == alpha1*rhod + alpha2*rhoc;
}
