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

\*---------------------------------------------------------------------------*/

#include "externalCoupledMixedFvPatchField.H"
#include "fvPatchFieldMapper.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::externalCoupledMixedFvPatchField<Type>::
externalCoupledMixedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(p, iF)
{
    this->refValue() = pTraits<Type>::zero;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;
}


template<class Type>
Foam::externalCoupledMixedFvPatchField<Type>::
externalCoupledMixedFvPatchField
(
    const externalCoupledMixedFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
Foam::externalCoupledMixedFvPatchField<Type>::
externalCoupledMixedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<Type>(p, iF)
{
    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<Type>::operator=(this->patchInternalField());
    }

    // Initialise as a fixed value
    this->refValue() = *this;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 1.0;
}


template<class Type>
Foam::externalCoupledMixedFvPatchField<Type>::
externalCoupledMixedFvPatchField
(
    const externalCoupledMixedFvPatchField& ecmpf
)
:
    mixedFvPatchField<Type>(ecmpf)
{}


template<class Type>
Foam::externalCoupledMixedFvPatchField<Type>::
externalCoupledMixedFvPatchField
(
    const externalCoupledMixedFvPatchField& ecmpf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(ecmpf, iF)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
Foam::externalCoupledMixedFvPatchField<Type>::
~externalCoupledMixedFvPatchField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::externalCoupledMixedFvPatchField<Type>::writeHeader
(
    Ostream& os
) const
{
    os  << "# Values: magSf value snGrad" << endl;
}


template<class Type>
void Foam::externalCoupledMixedFvPatchField<Type>::transferData
(
    Ostream& os
) const
{
    if (Pstream::parRun())
    {
        List<Field<scalar> > magSfs(Pstream::nProcs());
        magSfs[Pstream::myProcNo()].setSize(this->patch().size());
        magSfs[Pstream::myProcNo()] = this->patch().magSf();
        Pstream::gatherList(magSfs);

        List<Field<Type> > values(Pstream::nProcs());
        values[Pstream::myProcNo()].setSize(this->patch().size());
        values[Pstream::myProcNo()] = this->refValue();
        Pstream::gatherList(values);

        List<Field<Type> > snGrads(Pstream::nProcs());
        snGrads[Pstream::myProcNo()].setSize(this->patch().size());
        snGrads[Pstream::myProcNo()] = this->snGrad();
        Pstream::gatherList(snGrads);

        if (Pstream::master())
        {
            forAll(values, procI)
            {
                const Field<scalar>& magSf = magSfs[procI];
                const Field<Type>& value = values[procI];
                const Field<Type>& snGrad = snGrads[procI];

                forAll(magSf, faceI)
                {
                    os  << magSf[faceI] << token::SPACE
                        << value[faceI] << token::SPACE
                        << snGrad[faceI] << nl;
                }
            }

            os.flush();
        }
    }
    else
    {
        const Field<scalar>& magSf(this->patch().magSf());
        const Field<Type>& value(this->refValue());
        const Field<Type> snGrad(this->snGrad());

        forAll(magSf, faceI)
        {
            os  << magSf[faceI] << token::SPACE
                << value[faceI] << token::SPACE
                << snGrad[faceI] << nl;
        }

        os.flush();
    }
}


// ************************************************************************* //
