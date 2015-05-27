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

Class
    Foam::pressurePIDControlInletVelocityFvPatchVectorField

Group
    grpInletBoundaryConditions

Description
    This boundary condition modifies an inlet velocity to try and maintain a
    pressure drop between two face zones downstream. The integral and
    derivative gains are non-dimensionalised by the time-step.

    \heading Patch usage

    \table
        Property   | Description               | Required | Default value
        deltaP     | desired pressure drop     | yes      |
        upstream   | upstream face zone name   | yes      |
        downstream | downstream face zone name | yes      |
        p          | pressure field name       | no       | p
        rho        | density field name        | no       | none
        startTime  | time to start control     | no       | 0
        P          | proportional gain         | yes      |
        I          | integral gain             | yes      |
        D          | differential gain         | yes      |
    \endtable


    Example of the boundary condition specification:

    \verbatim
    myPatch
    {
        type            pressurePIDControlInletVelocity;
        deltaP          190;
        upstream        upstream;
        downstream      downstream;
        p               p;
        rho             none;
        startTime       0;
        P               0.002;
        I               0.002;
        D               0.002;
        value           uniform (0 0 0);
    }

SeeAlso
    Foam::fixedValueFvPatchField

SourceFiles
    pressurePIDControlInletVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef pressurePIDControlInletVelocityFvPatchVectorField_H
#define pressurePIDControlInletVelocityFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
     Class pressurePIDControlInletVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class pressurePIDControlInletVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Desired pressure difference between upstream and downstream
        const scalar deltaP_;

        //- Name of the upstream face zone
        const word upstreamName_;

        //- Name of the downstream face zone
        const word downstreamName_;

        //- Name of the pressure field
        const word pName_;

        //- Name of the density field (if any)
        const word rhoName_;

        //- Time at which to start the control algorithm
        const scalar startTime_;

        //- Proportional gain
        const scalar P_;

        //- Integral gain
        const scalar I_;

        //- Derivative gain
        const scalar D_;

        //- Current error
        scalar error_;

        //- Old error
        scalar errorOld_;

        //- Current error integral
        scalar errorIntegral_;

        //- Old error integral
        scalar errorOldIntegral_;

        //- Time index of the stored errors
        label errorTimeIndex_;


    // Private member functions

        //- Return the pressure interpolated to the faces
        const surfaceScalarField& facePressure() const;

        //- Calculate the area and pressure of a zone
        void zoneAreaAndPressure(const word& name, scalar& a, scalar& p) const;


public:

   //- Runtime type information
   TypeName("pressurePIDControlInletVelocity");


   // Constructors

        //- Construct from patch and internal field
        pressurePIDControlInletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        pressurePIDControlInletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  flowRateInletVelocityFvPatchVectorField
        //  onto a new patch
        pressurePIDControlInletVelocityFvPatchVectorField
        (
            const pressurePIDControlInletVelocityFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        pressurePIDControlInletVelocityFvPatchVectorField
        (
            const pressurePIDControlInletVelocityFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new pressurePIDControlInletVelocityFvPatchVectorField
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        pressurePIDControlInletVelocityFvPatchVectorField
        (
            const pressurePIDControlInletVelocityFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new pressurePIDControlInletVelocityFvPatchVectorField
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
