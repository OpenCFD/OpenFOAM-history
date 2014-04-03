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
#include "fvmSup.H"

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


Foam::tmp<Foam::volScalarField> Foam::fv::enthalpyPorositySource::rho
(
    const surfaceScalarField& phi
) const
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
            bool volumetric = false;
            if (phi.dimensions() == dimVolume/dimTime)
            {
                volumetric = true;
            }

            if (rhoName_ == "rhoRef")
            {
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
                        dimensionedScalar
                        (
                            "rho",
                            volumetric ? dimless : dimDensity,
                            rhoRef_
                        ),
                        zeroGradientFvPatchScalarField::typeName
                    )
                );
            }
            else
            {
                volScalarField rho
                (
                    mesh_.lookupObject<volScalarField>(rhoName_)
                );

                if (volumetric && (rho.dimensions() == dimDensity))
                {
                    dimensionedScalar rhoRef
                    (
                        "rho",
                        dimDensity,
                        rhoRef_
                    );

                    rho /= rhoRef;
                }

                return tmp<volScalarField>(new volScalarField(rho));
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


Foam::tmp<Foam::volScalarField> Foam::fv::enthalpyPorositySource::Cp() const
{
    switch (mode_)
    {
        case mdThermo:
        {
            const basicThermo& thermo =
                mesh_.lookupObject<basicThermo>("thermophysicalProperties");

            return thermo.Cp();
            break;
        }
        case mdLookup:
        {
            if (CpName_ == "CpRef")
            {
                scalar CpRef = readScalar(coeffs_.lookup("CpRef"));

                return tmp<volScalarField>
                (
                    new volScalarField
                    (
                        IOobject
                        (
                            name_ + ":Cp",
                            mesh_.time().timeName(),
                            mesh_,
                            IOobject::NO_READ,
                            IOobject::NO_WRITE
                        ),
                        mesh_,
                        dimensionedScalar
                        (
                            "Cp",
                            dimEnergy/dimMass/dimTemperature,
                            CpRef
                        ),
                        zeroGradientFvPatchScalarField::typeName
                    )
                );
            }
            else
            {
                return mesh_.lookupObject<volScalarField>(CpName_);
            }

            break;
        }
        default:
        {
            FatalErrorIn
            (
                "Foam::tmp<Foam::volScalarField> "
                "Foam::fv::enthalpyPorositySource::Cp() const"
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

    if (debug)
    {
        Info<< type() << ":" << name_ << " - updating phase indicator" << endl;
    }

    // update old time alpha1 field
    alpha1_.oldTime();

    const volScalarField& T = mesh_.lookupObject<volScalarField>(TName_);
    volScalarField Cp(this->Cp());

    forAll(cells_, i)
    {
        label cellI = cells_[i];

        scalar Tc = T[cellI];

        scalar Cpc = Cp[cellI];

        scalar alpha1New = alpha1_[cellI] + relax_*Cpc*(Tc - Tmelt_)/L_;

        alpha1_[cellI] = max(0, min(alpha1New, 1));

        deltaT_[i] = Tc - Tmelt_;
    }

    alpha1_.correctBoundaryConditions();

    curTimeIndex_ = mesh_.time().timeIndex();
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
    Tmelt_(readScalar(coeffs_.lookup("Tmelt"))),
    L_(readScalar(coeffs_.lookup("L"))),
    relax_(coeffs_.lookupOrDefault("relax", 0.9)),
    mode_(thermoModeTypeNames_.read(coeffs_.lookup("thermoMode"))),
    rhoRef_(readScalar(coeffs_.lookup("rhoRef"))),
    rhoName_(coeffs_.lookupOrDefault<word>("rhoName", "rho")),
    TName_(coeffs_.lookupOrDefault<word>("TName", "T")),
    CpName_(coeffs_.lookupOrDefault<word>("CpName", "Cp")),
    UName_(coeffs_.lookupOrDefault<word>("UName", "U")),
    phiName_(coeffs_.lookupOrDefault<word>("phiName", "phi")),
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
        zeroGradientFvPatchScalarField::typeName
    ),
    curTimeIndex_(-1),
    deltaT_(cells_.size(), 0)
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


    const surfaceScalarField& phi =
        mesh_.lookupObject<surfaceScalarField>(phiName_);

    const volScalarField rho(this->rho(phi));

    dimensionedScalar L("L", dimEnergy/dimMass, L_);

    // contributions added to rhs of solver equation
    if (eqn.psi().dimensions() == dimTemperature)
    {
        dimensionedScalar Cp
        (
            "Cp",
            dimEnergy/dimMass/dimTemperature,
            readScalar(coeffs_.lookup("CpRef"))
        );

        // isothermal phase change - only include time derivative
//        eqn -= L/Cp*(fvc::ddt(rho, alpha1_) + fvc::div(phi, alpha1_));
        eqn -= L/Cp*(fvc::ddt(rho, alpha1_));
    }
    else
    {
        // isothermal phase change - only include time derivative
//        eqn -= L*(fvc::ddt(rho, alpha1_) + fvc::div(phi, alpha1_));
        eqn -= L*(fvc::ddt(rho, alpha1_));
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

    vector g = this->g();

    scalarField& Sp = eqn.diag();
    vectorField& Su = eqn.source();
    const scalarField& V = mesh_.V();

    forAll(cells_, i)
    {
        label cellI = cells_[i];

        scalar Vc = V[cellI];
        scalar alpha1c = alpha1_[cellI];

        scalar S = -Cu_*sqr(1.0 - alpha1c)/(pow3(alpha1c) + q_);
        vector Sb = rhoRef_*g*beta_*deltaT_[i];

        Sp[cellI] += Vc*S;
        Su[cellI] += Vc*Sb;
    }
}


// ************************************************************************* //
