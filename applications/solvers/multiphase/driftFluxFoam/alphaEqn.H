{
    word alphaScheme("div(phi,alpha)");
    word alpharScheme("div(phirb,alpha)");

    for (int aCorr=0; aCorr<nAlphaCorr; aCorr++)
    {
        phiAlpha =
        (
            fvc::flux
            (
                phi,
                alpha1,
                alphaScheme
            )
          + fvc::flux
            (
                phir,
                alpha1,
                alpharScheme
            )
        );

        MULES::explicitSolve(alpha1, phi, phiAlpha, 1, 0);
    }

    alpha2 = scalar(1) - alpha1;

    Info<< "Phase-1 volume fraction = "
        << alpha1.weightedAverage(mesh.Vsc()).value()
        << "  Min(alpha) = " << min(alpha1).value()
        << "  Max(alpha) = " << max(alpha1).value()
        << endl;
}
