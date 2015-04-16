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
    Foam::solidChemistryModel

Description
    Extends base solid chemistry model by adding a thermo package, and ODE
    functions.
    Introduces chemistry equation system and evaluation of chemical source
    terms.

SourceFiles
    solidChemistryModelI.H
    solidChemistryModel.C

\*---------------------------------------------------------------------------*/

#ifndef solidChemistryModel_H
#define solidChemistryModel_H

#include "Reaction.H"
#include "ODESystem.H"
#include "volFieldsFwd.H"
#include "DimensionedField.H"
#include "simpleMatrix.H"

#include "chemistryModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                   Class solidChemistryModel Declaration
\*---------------------------------------------------------------------------*/

template<class CompType, class SolidThermo>
class solidChemistryModel
:
    public chemistryModel<CompType, SolidThermo>
{
    // Private Member Functions

        //- Disallow copy constructor
        solidChemistryModel(const solidChemistryModel&);

        //- Disallow default bitwise assignment
        void operator=(const solidChemistryModel&);


protected:


        //- List of active reacting cells
        List<bool> reactingCells_;


    // Protected Member Functions

        //- Set reacting status of cell, cellI
        void setCellReacting(const label cellI, const bool active);


public:

    //- Runtime type information
    TypeName("solidChemistryModel");


    // Constructors

        //- Construct from mesh and phase name
        solidChemistryModel(const fvMesh& mesh, const word& phaseName);


    //- Destructor
    virtual ~solidChemistryModel();


    // Member Functions


            //- Return total solid source term
            inline tmp<DimensionedField<scalar, volMesh> > RRt() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "solidChemistryModelI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "solidChemistryModel.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
