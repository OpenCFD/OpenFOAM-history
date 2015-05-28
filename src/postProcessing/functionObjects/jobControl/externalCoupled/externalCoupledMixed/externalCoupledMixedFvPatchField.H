/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2015 OpenFOAM Foundation
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
    Foam::externalCoupledMixedFvPatchField

Group
    grpGenericBoundaryConditions grpCoupledBoundaryConditions

Description
    This boundary condition provides an interface to an external application.
    Values are transferred as plain text files, where OpenFOAM data is written
    as:

        # Patch: <patch name>
        <magSf1> <value1> <surfaceNormalGradient1>
        <magSf2> <value2> <surfaceNormalGradient2>
        <magSf3> <value3> <surfaceNormalGradient3>
        ...
        <magSfN> <valueN> <surfaceNormalGradientN>

    and received as the constituent pieces of the `mixed' condition. To be
    used in combination with the externalCoupled functionObject.

SeeAlso
    mixedFvPatchField
    externalCoupled (functionObject)

SourceFiles
    externalCoupledMixedFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef externalCoupledMixedFvPatchField_H
#define externalCoupledMixedFvPatchField_H

#include "mixedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
              Class externalCoupledMixedFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class externalCoupledMixedFvPatchField
:
    public mixedFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("externalCoupled");


    // Constructors

        //- Construct from patch and internal field
        externalCoupledMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        externalCoupledMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given externalCoupledMixedFvPatchField
        //  onto a new patch
        externalCoupledMixedFvPatchField
        (
            const externalCoupledMixedFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        externalCoupledMixedFvPatchField
        (
            const externalCoupledMixedFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new externalCoupledMixedFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        externalCoupledMixedFvPatchField
        (
            const externalCoupledMixedFvPatchField&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >
            (
                new externalCoupledMixedFvPatchField<Type>(*this, iF)
            );
        }


    //- Destructor
    virtual ~externalCoupledMixedFvPatchField();


    // Member functions

        //- Write header to transfer file
        virtual void writeHeader(Ostream& os) const;

        //- Transfer data for external source
        virtual void transferData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "externalCoupledMixedFvPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
