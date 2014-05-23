{
    incompressibleCorrectPhi
    (
        phi,
        U,
        p_rgh,
        dimensionedScalar("rAUf", dimTime/rho.dimensions(), 1.0),
        geometricZeroField(),
        pimple
    );
    #include "continuityErrs.H"
}
