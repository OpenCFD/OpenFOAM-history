    Info<< "Reading field Phi\n" << endl;
    volScalarField Phi
    (
        IOobject
        (
            "Phi",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        ),
        mesh
    );

    Phi = dimensionedScalar("zero", Phi.dimensions(), 0.0);


    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    U = dimensionedVector("0", U.dimensions(), vector::zero);

    surfaceScalarField phi
    (
        IOobject
        (
            "phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        fvc::interpolate(U) & mesh.Sf()
    );

    if (args.optionFound("initialiseUBCs"))
    {
        U.correctBoundaryConditions();
        phi = fvc::interpolate(U) & mesh.Sf();
    }


    label PhiRefCell = 0;
    scalar PhiRefValue = 0.0;
    setRefCell
    (
        Phi,
        potentialFlow,
        PhiRefCell,
        PhiRefValue
    );
