/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2014 OpenFOAM Foundation
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

#include "enthalpyPorositySource.H"
#include "fvMatrices.H"
#include "specie.H"
#include "basicThermo.H"
#include "uniformDimensionedFields.H"
#include "fixedValueFvPatchFields.H"
#include "zeroGradientFvPatchFields.H"
#include "addToRunTimeSelectionTable.H"
#include "fvcDdt.H"
#include "fvcDiv.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace Foam
{
    template<>
    const char* NamedEnum
    <
        fv::enthalpyPorositySource::thermoMode,
        2
    >::names[] =
    {
        "thermo",
        "lookup"
    };

    namespace fv
    {
        defineTypeNameAndDebug(enthalpyPorositySource, 0);

        addToRunTimeSelectionTable
        (
            option,
            enthalpyPorositySource,
            dictionary
        );
    }
}

const Foam::NamedEnum<Foam::fv::enthalpyPorositySource::thermoMode, 2>
    Foam::fv::enthalpyPorositySource::thermoModeTypeNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::wordList Foam::fv::enthalpyPorositySource::alpha1BoundaryTypes() const
{
    const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);

    wordList bTypes(T.boundaryField().size());

    forAll(bTypes, patchI)
    {
        const fvPatchField<scalar>& pf = T.boundaryField()[patchI];
        if (isA<fixedValueFvPatchScalarField>(pf))
        {
            bTypes[patchI] = fixedValueFvPatchScalarField::typeName;
        }
        else
        {
            bTypes[patchI] = zeroGradientFvPatchScalarField::typeName;
        }
    }

    return bTypes;
}


bool Foam::fv::enthalpyPorositySource::solveField(const word& fieldName) const
{
    bool result = true;

    switch (mode_)
    {
        case mdThermo:
        {
            const basicThermo& thermo =
                mesh_.lookupObject<basicThermo>("thermophysicalProperties");

            if (fieldName != thermo.he().name())
            {
                result = false;
            }
            break;
        }
        case mdLookup:
        {
            if (fieldName != TName_)
            {
                result = false;
            }
            break;
        }
        default:
        {
            FatalErrorIn
            (
                "bool Foam::fv::enthalpyPorositySource::solveField"
                "("
                    "const word&"
                ") const"
            )
                << "Unhandled thermo mode: " << thermoModeTypeNames_[mode_]
                << abort(FatalError);
        }
    }

    return result;
}


Foam::tmp<Foam::volScalarField> Foam::fv::enthalpyPorositySource::rho() const
{
    switch (mode_)
    {
        case mdThermo:
        {
            const basicThermo& thermo =
                mesh_.lookupObject<basicThermo>("thermophysicalProperties");

            return thermo.rho();
            break;
        }
        case mdLookup:
        {
            if (rhoName_ == "rhoRef")
            {
                scalar rhoRef(readScalar(coeffs_.lookup("rhoRef")));

                return tmp<volScalarField>
                (
                    new volScalarField
                    (
                        IOobject
                        (
                            name_ + ":rho",
                            mesh_.time().timeName(),
                            mesh_,
                            IOobject::NO_READ,
                            IOobject::NO_WRITE
                        ),
                        mesh_,
                        dimensionedScalar("rho", dimDensity, rhoRef),
                        zeroGradientFvPatchScalarField::typeName
                    )
                );
            }
            else
            {
                return mesh_.lookupObject<volScalarField>(rhoName_);
            }

            break;
        }
        default:
        {
            FatalErrorIn
            (
                "Foam::tmp<Foam::volScalarField> "
                "Foam::fv::enthalpyPorositySource::rho() const"
            )
                << "Unhandled thermo mode: " << thermoModeTypeNames_[mode_]
                << abort(FatalError);
        }
    }

    return tmp<volScalarField>(NULL);
}


Foam::vector Foam::fv::enthalpyPorositySource::g() const
{
    if (mesh_.foundObject<uniformDimensionedVectorField>("g"))
    {
        const uniformDimensionedVectorField& value =
            mesh_.lookupObject<uniformDimensionedVectorField>("g");
        return value.value();
    }
    else
    {
        return coeffs_.lookup("g");
    }
}


void Foam::fv::enthalpyPorositySource::update()
{
    if (curTimeIndex_ == mesh_.time().timeIndex())
    {
        return;
    }

    const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);

    forAll(cells_, i)
    {
        label cellI = cells_[i];

        scalar alpha1New = 0.0;
        scalar Tc = T[cellI];

        if (Tc > Tliquidus_)
        {
            alpha1New = 1.0;
        }
        else if (Tc < Tsolidus_)
        {
            alpha1New = 0.0;
        }
        else
        {
            // lever rule
            alpha1New = (Tc - Tsolidus_)/(Tliquidus_ - Tsolidus_);
        }

        alpha1New = (1.0 - relax_)*alpha1_[cellI] + relax_*alpha1New;

        dAlpha1_[i] = alpha1New - alpha1_[cellI];

        alpha1_[cellI] = alpha1New;

        curTimeIndex_ = mesh_.time().timeIndex();
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fv::enthalpyPorositySource::enthalpyPorositySource
(
    const word& sourceName,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(sourceName, modelType, dict, mesh),
    Tliquidus_(readScalar(coeffs_.lookup("Tliquidus"))),
    Tsolidus_(readScalar(coeffs_.lookup("Tsolidus"))),
    L_(readScalar(coeffs_.lookup("L"))),
    relax_(coeffs_.lookupOrDefault("relax", 0.9)),
    mode_(thermoModeTypeNames_.read(coeffs_.lookup("thermoMode"))),
    rhoName_(coeffs_.lookupOrDefault<word>("rhoName", "rho")),
    TName_(coeffs_.lookupOrDefault<word>("TName", "T")),
    UName_(coeffs_.lookupOrDefault<word>("UName", "U")),
    Cu_(coeffs_.lookupOrDefault<scalar>("Cu", 100000)),
    q_(coeffs_.lookupOrDefault("q", 0.001)),
    beta_(readScalar(coeffs_.lookup("beta"))),
    alpha1_
    (
        IOobject
        (
            name_ + ":alpha1",
            mesh.time().timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("alpha1", dimless, 0.0),
        alpha1BoundaryTypes()
    ),
    dAlpha1_(cells_.size(), 0.0),
    curTimeIndex_(-1)
{
    fieldNames_.setSize(1, "source");
    applied_.setSize(1, false);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::fv::enthalpyPorositySource::alwaysApply() const
{
    return true;
}


void Foam::fv::enthalpyPorositySource::addSup
(
    fvMatrix<scalar>& eqn,
    const label fieldI
)
{
    if (!solveField(eqn.psi().name()))
    {
        return;
    }

    if (debug)
    {
        Info<< type() << ": applying source to " << eqn.psi().name() << endl;
    }

    update();

    volScalarField dH
    (
        IOobject
        (
            name_ + ":dH",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedScalar("dH", dimEnergy/dimMass, 0.0)
    );

    scalarField& dHi = dH.internalField();

    forAll(cells_, i)
    {
        label cellI = cells_[i];
        dHi[cellI] = L_*dAlpha1_[i];
    }

    const volScalarField rho(this->rho());

    const surfaceScalarField& phi =
        mesh_.lookupObject<surfaceScalarField>("phi");

    dimensionedScalar rhoScale("rhoScale", dimless, 1.0);

    if
    (
        (phi.dimensions() == dimVolume/dimTime)
     && (rho.dimensions() == dimDensity)
    )
    {
        rhoScale.dimensions() /= dimDensity;
    }

    // contributions added to rhs
    if (eqn.psi().dimensions() == dimTemperature)
    {
        dimensionedScalar Cp
        (
            "Cp",
            dimEnergy/dimMass/dimTemperature,
            readScalar(coeffs_.lookup("CpRef"))
        );

        eqn -=
            fvc::ddt((rho*rhoScale*dH/Cp)())
          + fvc::div(phi*fvc::interpolate(dH/Cp));
    }
    else
    {
        eqn -=
            fvc::ddt((rho*rhoScale*dH)())
          + fvc::div(phi*fvc::interpolate(dH));
    }
}


void Foam::fv::enthalpyPorositySource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldI
)
{
    const volVectorField& U = eqn.psi();

    if (U.name() != UName_)
    {
        return;
    }

    if (debug)
    {
        Info<< type() << ": applying source to " << UName_ << endl;
    }

    update();

    scalarField& Sp = eqn.diag();
    vectorField& Su = eqn.source();

    const scalarField& V = mesh_.V();
    const volScalarField rho(this->rho());
    const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);

    vector g = this->g();

    forAll(cells_, i)
    {
        label cellI = cells_[i];

        scalar Vc = V[cellI];

        scalar Tc = T[cellI];
        scalar rhoc = rho[cellI];
        scalar alpha1c = alpha1_[cellI];

        Sp[cellI] -= Vc*rhoc*Cu_*sqr(1.0 - alpha1c)/(pow3(alpha1c) + q_);
        Su[cellI] += Vc*rhoc*g*beta_*max(0, (Tc - Tsolidus_));
    }
}


// ************************************************************************* //
