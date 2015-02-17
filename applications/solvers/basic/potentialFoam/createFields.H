    Info<< "Reading velocity field U\n" << endl;
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

    // Initialise the velocity internal field to zero
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


    // Construct a pressure field
    // If it is available read it otherwise construct from the velocity BCs
    // converting fixed-value BCs to zero-gradient and vice versa.
    word pName("p");

    // Update name of the pressure field from the command-line option
    args.optionReadIfPresent("pName", pName);

    wordList pTypes
    (
        U.boundaryField().size(),
        fixedValueFvPatchScalarField::typeName
    );

    forAll(U.boundaryField(), patchi)
    {
        if (U.boundaryField()[patchi].fixesValue())
        {
            pTypes[patchi] = zeroGradientFvPatchScalarField::typeName;
        }
    }

    Info<< "Constructing pressure field " << pName << nl << endl;
    volScalarField p
    (
        IOobject
        (
            pName,
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar(pName, sqr(dimVelocity), 0),
        pTypes
    );

    label pRefCell = 0;
    scalar pRefValue = 0.0;
    if (args.optionFound("writep"))
    {
        setRefCell
        (
            p,
            potentialFlow.dict(),
            pRefCell,
            pRefValue
        );
    }


    Info<< "Constructing velocity potential field Phi\n" << endl;
    volScalarField Phi
    (
        IOobject
        (
            "Phi",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("Phi", dimLength*dimVelocity, 0),
        p.boundaryField().types()
    );

    label PhiRefCell = 0;
    scalar PhiRefValue = 0.0;
    setRefCell
    (
        Phi,
        potentialFlow.dict(),
        PhiRefCell,
        PhiRefValue
    );
