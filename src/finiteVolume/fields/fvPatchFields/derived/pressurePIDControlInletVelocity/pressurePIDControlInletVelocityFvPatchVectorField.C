/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenCFD Ltd.
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

#include "pressurePIDControlInletVelocityFvPatchVectorField.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "surfaceFields.H"
#include "linear.H"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

const Foam::surfaceScalarField&
Foam::pressurePIDControlInletVelocityFvPatchVectorField::facePressure() const
{
    const volScalarField& p(db().lookupObject<volScalarField>(pName_));

    const word pfName(pName_ + "f");

    if (!db().foundObject<surfaceScalarField>(pfName))
    {
        surfaceScalarField* pfPtr
        (
            new surfaceScalarField(pfName, linearInterpolate(p))
        );

        pfPtr->store();
    }

    surfaceScalarField& pf
    (
        const_cast<surfaceScalarField&>
        (
            db().lookupObject<surfaceScalarField>(pfName)
        )
    );

    if (!pf.upToDate(p))
    {
        pf = linearInterpolate(p);
    }

    return pf;
}


void Foam::pressurePIDControlInletVelocityFvPatchVectorField::
zoneAreaAndPressure(const word& name, scalar& a, scalar& p) const
{
    const surfaceScalarField& pf(facePressure());

    const faceZone& zone = pf.mesh().faceZones()[name];

    a = p = 0;

    forAll(zone, faceI)
    {
        const label f(zone[faceI]);

        const scalar da(pf.mesh().magSf()[f]);

        a += da;
        p += da*pf[f];
    }

    reduce(a, sumOp<scalar>());
    reduce(p, sumOp<scalar>());

    p /= a;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pressurePIDControlInletVelocityFvPatchVectorField::
pressurePIDControlInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    deltaP_(1),
    upstreamName_(word::null),
    downstreamName_(word::null),
    pName_("p"),
    rhoName_("none"),
    startTime_(0),
    P_(1),
    I_(0),
    D_(0),
    error_(0),
    errorIntegral_(0),
    errorTimeIndex_(db().time().timeIndex())
{}


Foam::pressurePIDControlInletVelocityFvPatchVectorField::
pressurePIDControlInletVelocityFvPatchVectorField
(
    const pressurePIDControlInletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    deltaP_(ptf.deltaP_),
    upstreamName_(ptf.upstreamName_),
    downstreamName_(ptf.downstreamName_),
    pName_(ptf.pName_),
    rhoName_(ptf.rhoName_),
    startTime_(ptf.startTime_),
    P_(ptf.P_),
    I_(ptf.I_),
    D_(ptf.D_),
    error_(ptf.error_),
    errorIntegral_(ptf.errorIntegral_),
    errorTimeIndex_(ptf.errorTimeIndex_)
{}


Foam::pressurePIDControlInletVelocityFvPatchVectorField::
pressurePIDControlInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF, dict),
    deltaP_(readScalar(dict.lookup("deltaP"))),
    upstreamName_(dict.lookup("upstream")),
    downstreamName_(dict.lookup("downstream")),
    pName_(dict.lookupOrDefault<word>("p", "p")),
    rhoName_(dict.lookupOrDefault<word>("rho", "none")),
    startTime_(dict.lookupOrDefault<scalar>("startTime", 0)),
    P_(readScalar(dict.lookup("P"))),
    I_(readScalar(dict.lookup("I"))),
    D_(readScalar(dict.lookup("D"))),
    error_(dict.lookupOrDefault<scalar>("error", 0)),
    errorIntegral_(dict.lookupOrDefault<scalar>("errorIntegral", 0)),
    errorTimeIndex_(db().time().timeIndex())
{}


Foam::pressurePIDControlInletVelocityFvPatchVectorField::
pressurePIDControlInletVelocityFvPatchVectorField
(
    const pressurePIDControlInletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    deltaP_(ptf.deltaP_),
    upstreamName_(ptf.upstreamName_),
    downstreamName_(ptf.downstreamName_),
    pName_(ptf.pName_),
    rhoName_(ptf.rhoName_),
    startTime_(ptf.startTime_),
    P_(ptf.P_),
    I_(ptf.I_),
    D_(ptf.D_),
    error_(ptf.error_),
    errorIntegral_(ptf.errorIntegral_),
    errorTimeIndex_(ptf.errorTimeIndex_)
{}


Foam::pressurePIDControlInletVelocityFvPatchVectorField::
pressurePIDControlInletVelocityFvPatchVectorField
(
    const pressurePIDControlInletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    deltaP_(ptf.deltaP_),
    upstreamName_(ptf.upstreamName_),
    downstreamName_(ptf.downstreamName_),
    pName_(ptf.pName_),
    rhoName_(ptf.rhoName_),
    startTime_(ptf.startTime_),
    P_(ptf.P_),
    I_(ptf.I_),
    D_(ptf.D_),
    error_(ptf.error_),
    errorIntegral_(ptf.errorIntegral_),
    errorTimeIndex_(ptf.errorTimeIndex_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::pressurePIDControlInletVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated() || errorTimeIndex_ == db().time().timeIndex())
    {
        return;
    }

    errorTimeIndex_ = db().time().timeIndex();

    const scalar deltaT(db().time().deltaTValue());

    const volScalarField& p(db().lookupObject<volScalarField>(pName_));

    // Get the density
    scalar rho = 1;
    if (p.dimensions() == dimPressure/dimDensity)
    {
        // do nothing ...
    }
    else if (p.dimensions() == dimPressure)
    {
        const fvPatchField<scalar>& rhoField =
            patch().lookupPatchField<volScalarField, scalar>(rhoName_);

        rho = gSum(rhoField*patch().magSf())/gSum(patch().magSf());
    }
    else
    {
        FatalErrorIn
        (
            "void Foam::"
            "pressurePIDControlInletVelocityFvPatchVectorField::"
            "updateCoeffs()"
        )   << "Pressure dimensions not recognised"
            << exit(FatalError);
    }

    // Patch properties
    const scalar patchA = gSum(patch().magSf());
    const scalar patchU = - gSum(*this & patch().Sf())/patchA;

    // Zone properties
    scalar upstreamA, upstreamP, downstreamA, downstreamP;
    zoneAreaAndPressure(upstreamName_, upstreamA, upstreamP);
    zoneAreaAndPressure(downstreamName_, downstreamA, downstreamP);
    const scalar pDrop = upstreamP - downstreamP;

    // Difference in area ratios
    const scalar AStar = sqr(patchA/downstreamA) - sqr(patchA/upstreamA);

    // Theoretical velocity
    const scalar U = sqrt(2*deltaP_/(rho*AStar));

    // If not yet controlling then set the patch to the theoretical velocity
    if (db().time().value() < startTime_)
    {
        operator==(- patch().nf()*U);
    }

    // If controlling then push the patch velocity in the direction given by
    // the various errors
    else
    {
        const scalar errorOld = error_;

        error_ = U - sqrt(2*pDrop/(rho*AStar));

        errorIntegral_ += 0.5*(error_ + errorOld)*deltaT;

        const scalar errorDifferential = (errorOld - error_)/deltaT;

        operator==
        (
          - patch().nf()
           *(
               patchU
             + P_*error_
             + (I_/deltaT)*errorIntegral_
             + (D_*deltaT)*errorDifferential
            )
        );
    }

    // Log output
    const scalar error = pDrop/deltaP_ - 1;
    Info<< "pressurePIDControlInletVelocity " << patch().name()
        << " : pressure drop = " << pDrop << " (" << mag(error)*100 << "\% "
        << (error < 0 ? "below" : "above") << " the target)" << endl;

    fixedValueFvPatchField<vector>::updateCoeffs();
}


void Foam::pressurePIDControlInletVelocityFvPatchVectorField::write
(
    Ostream& os
) const
{
    fvPatchField<vector>::write(os);

    os.writeKeyword("deltaP") << deltaP_ << token::END_STATEMENT << nl;
    os.writeKeyword("upstream") << upstreamName_ << token::END_STATEMENT << nl;
    os.writeKeyword("downstream")
        << downstreamName_ << token::END_STATEMENT << nl;
    if (pName_ != "p")
    {
        os.writeKeyword("p") << pName_ << token::END_STATEMENT << nl;
    }
    if (rhoName_ != "none")
    {
        os.writeKeyword("rho") << rhoName_ << token::END_STATEMENT << nl;
    }
    if (startTime_ != 0)
    {
        os.writeKeyword("startTime")
            << startTime_ << token::END_STATEMENT << nl;
    }
    os.writeKeyword("P") << P_ << token::END_STATEMENT << nl;
    os.writeKeyword("I") << I_ << token::END_STATEMENT << nl;
    os.writeKeyword("D") << D_ << token::END_STATEMENT << nl;
    os.writeKeyword("error") << error_ << token::END_STATEMENT << nl;
    os.writeKeyword("errorIntegral")
        << errorIntegral_ << token::END_STATEMENT << nl;

    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
   makePatchTypeField
   (
       fvPatchVectorField,
       pressurePIDControlInletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
