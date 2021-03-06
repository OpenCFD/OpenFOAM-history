/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2015 OpenFOAM Foundation
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

#include "humidityTemperatureCoupledMixedFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "mappedPatchBase.H"
#include "fixedGradientFvPatchFields.H"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

namespace Foam
{
    template<>
    const char* Foam::NamedEnum
    <
        Foam::
        humidityTemperatureCoupledMixedFvPatchScalarField::
        massTransferMode,
        4
    >::names[] =
    {
        "constantMass",
        "condensation",
        "evaporation",
        "condensationAndEvaporation"
    };
}

const Foam::NamedEnum
<
    Foam::
    humidityTemperatureCoupledMixedFvPatchScalarField::
    massTransferMode,
    4
> Foam::
humidityTemperatureCoupledMixedFvPatchScalarField:: MassModeTypeNames_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

humidityTemperatureCoupledMixedFvPatchScalarField::
humidityTemperatureCoupledMixedFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    temperatureCoupledBase(patch(), "fluidThermo", "undefined", "undefined-K"),
    mode_(mConstantMass),
    TnbrName_("undefined-Tnbr"),
    QrNbrName_("undefined-Qr"),
    QrName_("undefined-Qr"),
    specieName_("undefined"),
    liquid_(NULL),
    liquidDict_(NULL),
    mass_(patch().size(), 0.0),
    Tvap_(0.0),
    myKDelta_(patch().size(), 0.0),
    dmHfg_(patch().size(), 0.0),
    mpCpTp_(patch().size(), 0.0),
    Mcomp_(0.0),
    L_(0.0),
    fluid_(false),
    cp_(patch().size(), 0.0),
    thickness_(patch().size(), 0.0),
    rho_(patch().size(), 0.0)
{
    this->refValue() = 0.0;
    this->refGrad() = 0.0;
    this->valueFraction() = 1.0;
}


humidityTemperatureCoupledMixedFvPatchScalarField::
humidityTemperatureCoupledMixedFvPatchScalarField
(
    const humidityTemperatureCoupledMixedFvPatchScalarField& psf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(psf, p, iF, mapper),
    temperatureCoupledBase(patch(), psf),
    mode_(psf.mode_),
    TnbrName_(psf.TnbrName_),
    QrNbrName_(psf.QrNbrName_),
    QrName_(psf.QrName_),
    specieName_(psf.specieName_),
    liquid_(psf.liquid_),
    liquidDict_(psf.liquidDict_),
    mass_(psf.mass_, mapper),
    Tvap_(psf.Tvap_),
    myKDelta_(psf.myKDelta_, mapper),
    dmHfg_(psf.dmHfg_, mapper),
    mpCpTp_(psf.mpCpTp_, mapper),
    Mcomp_(psf.Mcomp_),
    L_(psf.L_),
    fluid_(psf.fluid_),
    cp_(psf.cp_, mapper),
    thickness_(psf.thickness_, mapper),
    rho_(psf.rho_, mapper)
{}


humidityTemperatureCoupledMixedFvPatchScalarField::
humidityTemperatureCoupledMixedFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    temperatureCoupledBase(patch(), dict),
    mode_(mCondensationAndEvaporation),
    TnbrName_(dict.lookupOrDefault<word>("Tnbr", "T")),
    QrNbrName_(dict.lookupOrDefault<word>("QrNbr", "none")),
    QrName_(dict.lookupOrDefault<word>("Qr", "none")),
    specieName_(dict.lookupOrDefault<word>("specieName", "none")),
    liquid_(NULL),
    liquidDict_(),
    mass_(patch().size(), 0.0),
    Tvap_(0.0),
    myKDelta_(patch().size(), 0.0),
    dmHfg_(patch().size(), 0.0),
    mpCpTp_(patch().size(), 0.0),
    Mcomp_(0.0),
    L_(0.0),
    fluid_(false),
    cp_(patch().size(), 0.0),
    thickness_(patch().size(), 0.0),
    rho_(patch().size(), 0.0)
{
    if (!isA<mappedPatchBase>(this->patch().patch()))
    {
        FatalErrorIn
        (
            "humidityTemperatureCoupledMixedFvPatchScalarField::"
            "humidityTemperatureCoupledMixedFvPatchScalarField\n"
            "(\n"
            "    const fvPatch&,\n"
            "    const DimensionedField<scalar, volMesh>&,\n"
            "    const dictionary&\n"
            ")\n"
        )   << "\n    patch type '" << p.type()
            << "' not type '" << mappedPatchBase::typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << dimensionedInternalField().name()
            << " in file " << dimensionedInternalField().objectPath()
            << exit(FatalError);
    }

    fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

    if (dict.found("mode"))
    {
        mode_ = MassModeTypeNames_.read(dict.lookup("mode"));
        fluid_ = true;
    }

    if (fluid_)
    {
        if (mode_ == mConstantMass)
        {
            thickness_ = scalarField("thickness", dict, p.size());
            cp_ = scalarField("cp", dict, p.size());
            rho_ = scalarField("rho", dict, p.size());
        }
        else if (mode_ > mConstantMass)
        {
            Mcomp_ = readScalar(dict.lookup("carrierMolWeight"));
            L_ = readScalar(dict.lookup("L"));
            Tvap_ = readScalar(dict.lookup("Tvap"));
            liquidDict_ = dict.subDict("liquid");
            liquid_.reset
            (
                liquidProperties::New(liquidDict_.subDict(specieName_)).ptr()
            );
            if (dict.found("thickness"))
            {
                scalarField& Tp = *this;
                const scalarField magSf = patch().magSf();
                // Assume initially standard pressure for rho calculation
                scalar pf = 1e5;
                thickness_ = scalarField("thickness", dict, p.size());
                forAll (thickness_, i)
                {
                    mass_[i] = thickness_[i]*liquid_->rho(pf, Tp[i])*magSf[i];
                }
            }
            fluid_ = true;
        }
        else
        {
            FatalErrorIn
            (
                "humidityTemperatureCoupledMixedFvPatchScalarField::"
                "humidityTemperatureCoupledMixedFvPatchScalarField\n"
                "(\n"
                "    const fvPatch&,\n"
                "    const DimensionedField<scalar, volMesh>&,\n"
                "    const dictionary& \n"
                ")\n"
            )
                << "Did not find mode " << mode_
                << " on  patch " << patch().name()
                << nl
                << "Please set 'mode' to one of "
                << MassModeTypeNames_.toc()
                << exit(FatalError);
        }
    }



    if (dict.found("refValue"))
    {
        // Full restart
        refValue() = scalarField("refValue", dict, p.size());
        refGrad() = scalarField("refGradient", dict, p.size());
        valueFraction() = scalarField("valueFraction", dict, p.size());
    }
    else
    {
        // Start from user entered data. Assume fixedValue.
        refValue() = *this;
        refGrad() = 0.0;
        valueFraction() = 1.0;
    }
}


humidityTemperatureCoupledMixedFvPatchScalarField::
humidityTemperatureCoupledMixedFvPatchScalarField
(
    const humidityTemperatureCoupledMixedFvPatchScalarField& psf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(psf, iF),
    temperatureCoupledBase(patch(), psf),
    mode_(psf.mode_),
    TnbrName_(psf.TnbrName_),
    QrNbrName_(psf.QrNbrName_),
    QrName_(psf.QrName_),
    specieName_(psf.specieName_),
    liquid_(psf.liquid_),
    liquidDict_(psf.liquidDict_),
    mass_(psf.mass_),
    Tvap_(psf.Tvap_),
    myKDelta_(psf.myKDelta_),
    dmHfg_(psf.dmHfg_),
    mpCpTp_(psf.mpCpTp_),
    Mcomp_(psf.Mcomp_),
    L_(psf.L_),
    fluid_(psf.fluid_),
    cp_(psf.cp_),
    thickness_(psf.thickness_),
    rho_(psf.rho_)
{}

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

scalar humidityTemperatureCoupledMixedFvPatchScalarField::Sh
(
    const scalar Re,
    const scalar Sc
) const
{
    if (Re < 5.0E+05)
    {
        return 0.664*sqrt(Re)*cbrt(Sc);
    }
    else
    {
        return 0.037*pow(Re, 0.8)*cbrt(Sc);
    }
}


scalar humidityTemperatureCoupledMixedFvPatchScalarField::htcCondensation
(
    const scalar Tsat,
    const scalar Re
) const
{
    if (Tsat > 295 && Tsat < 373)
    {
        return 51104 + 2044*Tsat;
    }
    else
    {
        return 255510;
    }
}


volScalarField& humidityTemperatureCoupledMixedFvPatchScalarField::
thicknessField
(
    const word& fieldName,
    const fvMesh& mesh
)
{
    if (!mesh.foundObject<volScalarField>(fieldName))
    {
        tmp<volScalarField> tField
        (
            new volScalarField
            (
                IOobject
                (
                    fieldName,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimLength, 0.0)
            )
        );

        volScalarField& field = tField();

        field.rename(fieldName);
        field.writeOpt() = IOobject::AUTO_WRITE;

        tField.ptr()->store();
    }

    return
        const_cast<volScalarField&>
        (
            mesh.lookupObject<volScalarField>(fieldName)
        );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void humidityTemperatureCoupledMixedFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    mixedFvPatchScalarField::autoMap(m);

    if (fluid_)
    {
        mass_.autoMap(m);
        myKDelta_.autoMap(m);
        dmHfg_.autoMap(m);
        mpCpTp_.autoMap(m);
        cp_.autoMap(m);
        thickness_.autoMap(m);
        rho_.autoMap(m);
    }
}


void humidityTemperatureCoupledMixedFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);

    const humidityTemperatureCoupledMixedFvPatchScalarField& tiptf =
        refCast<const humidityTemperatureCoupledMixedFvPatchScalarField>
        (
            ptf
        );

    if (fluid_)
    {
        mass_.rmap(tiptf.mass_, addr);
        myKDelta_.rmap(tiptf.myKDelta_, addr);
        dmHfg_.rmap(tiptf.dmHfg_, addr);
        mpCpTp_.rmap(tiptf.mpCpTp_, addr);
        cp_.rmap(tiptf.cp_, addr);
        thickness_.rmap(tiptf.thickness_, addr);
        rho_.rmap(tiptf.rho_, addr);
    }
}


void humidityTemperatureCoupledMixedFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    // Get the coupling information from the mappedPatchBase
    const mappedPatchBase& mpp =
        refCast<const mappedPatchBase>(patch().patch());

    const scalarField magSf = patch().magSf();

    const label nbrPatchI = mpp.samplePolyPatch().index();
    const polyMesh& mesh = patch().boundaryMesh().mesh();
    const polyMesh& nbrMesh = mpp.sampleMesh();
    const fvPatch& nbrPatch =
        refCast<const fvMesh>(nbrMesh).boundary()[nbrPatchI];

    const humidityTemperatureCoupledMixedFvPatchScalarField&
        nbrField =
        refCast
        <
            const humidityTemperatureCoupledMixedFvPatchScalarField
        >
        (
            nbrPatch.lookupPatchField<volScalarField, scalar>(TnbrName_)
        );

    // Swap to obtain full local values of neighbour internal field
    scalarField nbrIntFld(nbrField.patchInternalField());
    mpp.distribute(nbrIntFld);

    scalarField& Tp = *this;
    scalarField Tin(patchInternalField());

    const scalarField K(this->kappa(*this));

    // nrb kappa is done separatedly because I need kappa solid for
    // htc correlation
    scalarField nbrK(nbrField.kappa(*this));
    mpp.distribute(nbrK);

    scalarField nrbDeltaCoeffs(nbrPatch.deltaCoeffs());
    mpp.distribute(nrbDeltaCoeffs);

    scalarField KDeltaNbr(nbrK*nrbDeltaCoeffs);
    //mpp.distribute(KDeltaNbr);

    myKDelta_ = K*patch().deltaCoeffs();

    scalarField dm(patch().size(), 0.0);

    //Fluid Side
    if (fluid_)
    {
        scalarField Yvp(patch().size(), 0.0);
        const scalar dt = mesh.time().deltaTValue();

        const scalarField myDelta(patch().deltaCoeffs());

        if (mode_ != mConstantMass)
        {
            scalarField cp(patch().size(), 0.0);
            scalarField hfg(patch().size(), 0.0);
            scalarField htc(patch().size(), GREAT);
            scalarField liquidRho(patch().size(), 0.0);

            fixedGradientFvPatchField<scalar>& Yp =
                const_cast<fixedGradientFvPatchField<scalar>& >
                (
                    refCast
                    <
                        const fixedGradientFvPatchField<scalar>
                    >
                    (
                        patch().lookupPatchField<volScalarField, scalar>
                        (
                            specieName_
                        )
                    )
                );

            const fvPatchScalarField& pp =
                patch().lookupPatchField<volScalarField, scalar>("p");

            const fvPatchVectorField& Up =
                patch().lookupPatchField<volVectorField, vector>("U");

            const fvPatchScalarField& rhop =
                patch().lookupPatchField<volScalarField, scalar>("rho");

            const fvPatchScalarField& mup =
                patch().lookupPatchField<volScalarField, scalar>("thermo:mu");

            const vectorField Ui(Up.patchInternalField());
            const scalarField Yi(Yp.patchInternalField());

            forAll (Tp, faceI)
            {
                const scalar Tf = Tp[faceI];
                const scalar Tint = Tin[faceI];
                const vector Uf = Ui[faceI];
                const scalar pf = pp[faceI];

                const scalar muf = mup[faceI];
                const scalar rhof = rhop[faceI];
                const scalar nuf = muf/rhof;
                const scalar pSat = liquid_->pv(pf, Tint);
                const scalar Mv = liquid_->W();
                const scalar TSat = liquid_->pvInvert(pSat);
                const scalar Re = mag(Uf)*L_/nuf;

                cp[faceI] = liquid_->Cp(pf, Tf);
                hfg[faceI] = liquid_->hl(pf, Tf);

                // Calculate relative humidity
                const scalar invMwmean =
                        Yi[faceI]/Mv + (1.0 - Yi[faceI])/Mcomp_;
                const scalar Xv = Yi[faceI]/invMwmean/Mv;
                const scalar RH = min(Xv*pf/pSat, 1.0);

                scalar RHmin = 0.01;
                scalar Tdew = -GREAT;

                if (RH > RHmin)
                {
                    scalar b = 243.5;
                    scalar c = 17.65;
                    scalar TintDeg = Tint - 273;
                    Tdew =
                        b*
                        (
                            log(RH)
                          + (c*TintDeg)/(b + TintDeg)
                        )
                        /
                        (
                            c - log(RH) - ((c*TintDeg)/(b + TintDeg))
                        ) + 273;
                }

                if
                (
                    Tf < Tdew && RH > RHmin
                 &&
                    (
                        mode_ == mCondensation
                     || mode_ == mCondensationAndEvaporation
                    )
                )
                {
                    htc[faceI] =
                        this->htcCondensation(TSat, Re)*nbrK[faceI]/L_;

                    scalar htcTotal =
                        1.0/((1.0/myKDelta_[faceI]) + (1.0/htc[faceI]));

                    // heat flux W (>0 heat is converted into mass)
                    const scalar q = (Tint - Tf)*htcTotal*magSf[faceI];

                    // mass flux rate [Kg/s/m2]
                    dm[faceI] = q/hfg[faceI]/magSf[faceI];

                    mass_[faceI] += q/hfg[faceI]*dt;

                    // -dYp/dn = q/Dab (fixedGradient)
                    const scalar Dab = liquid_->D(pf, Tf);
                    Yvp[faceI] =
                        -min(dm[faceI]/Dab/rhof, Yi[faceI]*myDelta[faceI]);

                }
                else if
                (
                    Tf > Tvap_ && mass_[faceI] > 0.0
                 &&
                    (
                        mode_ == mEvaporation
                     || mode_ == mCondensationAndEvaporation
                    )
                )
                {
                    const scalar Dab = liquid_->D(pf, Tf);

                    const scalar Sc = nuf/Dab;
                    const scalar Sh = this->Sh(Re, Sc);

                    const scalar Ys = Mv*pSat/(Mv*pSat + Mcomp_*(pf - pSat));
                    // mass transfer coefficient [m/s]
                    const scalar hm = Dab*Sh/L_;

                    const scalar Yinf = max(Yi[faceI], 0.0);

                    // mass flux rate [Kg/s/m2]
                    dm[faceI] = -rhof*hm*max((Ys - Yinf), 0.0)/(1.0 - Ys);

                    // Set fixedGradient for carrier species.
                    Yvp[faceI] = -dm[faceI]/Dab/rhof;

                    // Total mass accumulated [Kg]
                    mass_[faceI] += dm[faceI]*magSf[faceI]*dt;

                    htc[faceI] =
                        this->htcCondensation(TSat, Re)*nbrK[faceI]/L_;
                }
                else if (Tf > Tdew && Tf < Tvap_ && mass_[faceI] > 0.0)
                {
                    htc[faceI] =
                        this->htcCondensation(TSat, Re)*nbrK[faceI]/L_;
                }
                else if (mass_[faceI] == 0.0)
                {
                    //Do nothing
                }

                liquidRho[faceI] = liquid_->rho(pf, Tf);
            }

            mass_ = max(mass_, scalar(0));

            Yp.gradient() = Yvp;

            // Output filmDelta [m]
            const word fieldName(specieName_ + "Thickness");

            scalarField& pDelta =
                thicknessField
                (
                    fieldName,
                    refCast<const fvMesh>(mesh)
                ).boundaryField()[patch().index()];


            pDelta = mass_/liquidRho/magSf;

            // Weight myKDelta and htc
            myKDelta_ = 1.0/((1.0/myKDelta_) + (1.0/htc));

            mpCpTp_ = mass_*cp/dt/magSf;

            // Heat flux due to change of phase [W/m2]
            dmHfg_ = dm*hfg;
        }
        else
        {
            // inertia term [W/K/m2]
            mpCpTp_ = thickness_*rho_*cp_/dt;
        }
    }

    scalarField myKDeltaNbr(patch().size(), 0.0);
    scalarField mpCpTpNbr(patch().size(), 0.0);
    scalarField dmHfgNbr(patch().size(), 0.0);

    if (!fluid_)
    {
        myKDeltaNbr = nbrField.myKDelta();
        mpp.distribute(myKDeltaNbr);

        mpCpTpNbr = nbrField.mpCpTp();
        mpp.distribute(mpCpTpNbr);

        dmHfgNbr = nbrField.dmHfg();
        mpp.distribute(dmHfgNbr);
    }

    // Obtain Rad heat (Qr)
    scalarField Qr(Tp.size(), 0.0);
    if (QrName_ != "none")
    {
        Qr = patch().lookupPatchField<volScalarField, scalar>(QrName_);
    }

    scalarField QrNbr(Tp.size(), 0.0);
    if (QrNbrName_ != "none")
    {
        QrNbr = nbrPatch.lookupPatchField<volScalarField, scalar>(QrNbrName_);
        mpp.distribute(QrNbr);
    }

    const scalarField dmHfg(dmHfgNbr + dmHfg_);

    const scalarField mpCpdt(mpCpTpNbr + mpCpTp_);

    // Qr > 0 (heat up the wall)
    scalarField alpha(KDeltaNbr + mpCpdt - (Qr + QrNbr + dmHfg)/Tp);

    valueFraction() = alpha/(alpha + myKDelta_);

    refValue() = (KDeltaNbr*nbrIntFld + mpCpdt*Tp)/alpha;

    mixedFvPatchScalarField::updateCoeffs();


    if (debug)
    {
        if (fluid_)
        {
            scalar Qdm = gSum(dm);
            scalar QMass = gSum(mass_);
            scalar Qt = gSum(myKDelta_*(Tp - Tin)*magSf);
            scalar QtSolid = gSum(KDeltaNbr*(Tp - nbrIntFld)*magSf);

            Info<< mesh.name() << ':'
                << patch().name() << ':'
                << this->dimensionedInternalField().name() << " <- "
                << nbrMesh.name() << ':'
                << nbrPatch.name() << ':'
                << this->dimensionedInternalField().name() << " :" << nl
                << "    Total mass flux   [Kg/s] : " << Qdm << nl
                << "    Total mass on the wall [Kg] : " << QMass << nl
                << "    Total heat (>0 leaving the wall to the fluid) [W] : "
                << Qt << nl
                << "     Total heat (>0 leaving the wall to the solid) [W] : "
                << QtSolid << nl
                << "     wall temperature "
                << " min:" << gMin(*this)
                << " max:" << gMax(*this)
                << " avg:" << gAverage(*this)
                << endl;
        }
    }
}


void humidityTemperatureCoupledMixedFvPatchScalarField::write
(
    Ostream& os
) const
{
    mixedFvPatchScalarField::write(os);
    os.writeKeyword("QrNbr")<< QrNbrName_ << token::END_STATEMENT << nl;
    os.writeKeyword("Qr")<< QrName_ << token::END_STATEMENT << nl;

    if (fluid_)
    {
        os.writeKeyword("mode")<< MassModeTypeNames_[mode_]
            << token::END_STATEMENT <<nl;
        os.writeKeyword("specieName")<< specieName_
            << token::END_STATEMENT <<nl;
        os.writeKeyword("carrierMolWeight")<< Mcomp_
            << token::END_STATEMENT <<nl;

        os.writeKeyword("L")<< L_ << token::END_STATEMENT << nl;
        os.writeKeyword("Tvap")<< Tvap_ << token::END_STATEMENT << nl;
        os.writeKeyword("fluid")<< fluid_ << token::END_STATEMENT << nl;
        mass_.writeEntry("mass", os);

        if (mode_ == mConstantMass)
        {
            cp_.writeEntry("cp", os);
//             thickness_.writeEntry("thickness", os);
            rho_.writeEntry("rho", os);
        }

        thickness_.writeEntry("thickness", os);
        word liq = "liquid";
        os << token::TAB << token::TAB << liq;
        liquidDict_.write(os);
    }

    temperatureCoupledBase::write(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    humidityTemperatureCoupledMixedFvPatchScalarField
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// ************************************************************************* //
