if (mesh.changing())
{
    forAll(U.boundaryField(), patchi)
    {
        if (U.boundaryField()[patchi].fixesValue())
        {
            U.boundaryField()[patchi].initEvaluate();
        }
    }

    forAll(U.boundaryField(), patchi)
    {
        if (U.boundaryField()[patchi].fixesValue())
        {
            U.boundaryField()[patchi].evaluate();

            phi.boundaryField()[patchi] =
                U.boundaryField()[patchi]
              & mesh.Sf().boundaryField()[patchi];
        }
    }
}

{
    volScalarField pcorr
    (
        IOobject
        (
            "pcorr",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("pcorr", p_gh.dimensions(), 0.0),
        pcorrTypes
    );

    surfaceScalarField rAUf("rAUf", fvc::interpolate(rAU));

    while (pimple.correctNonOrthogonal())
    {
        fvScalarMatrix pcorrEqn
        (
            fvm::laplacian(rAUf, pcorr) == fvc::div(phi)
        );

        pcorrEqn.solve();

        if (pimple.finalNonOrthogonalIter())
        {
            phi -= pcorrEqn.flux();
        }
    }

    #include "continuityErrs.H"
}
