/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenFOAM Foundation
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
    Foam::equationMaxInitialResidualCondition

Description
    Maximum initial residual run time condition

SourceFiles
    equationMaxInitialResidualCondition.H
    equationMaxInitialResidualCondition.C

\*---------------------------------------------------------------------------*/

#ifndef equationMaxInitialResidualCondition_H
#define equationMaxInitialResidualCondition_H

#include "runTimeCondition.H"
#include "NamedEnum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
             Class equationMaxInitialResidualCondition Declaration
\*---------------------------------------------------------------------------*/

class equationMaxInitialResidualCondition
:
    public runTimeCondition
{
protected:

    // Protected data

        //- Field name
        const wordList fieldNames_;

        //- Value to compare
        const scalar value_;

        //- Start checking from iteration - always skips first iteration
        label startIter_;


public:

    //- Runtime type information
    TypeName("equationMaxInitialResidual");

    //- Constructor
    equationMaxInitialResidualCondition
    (
        const word& name,
        const objectRegistry& obr,
        const dictionary& dict,
        functionObjectState& state
    );

    //- Destructor
    virtual ~equationMaxInitialResidualCondition();


    // Public Member Functions

        //- Apply the condition
        virtual bool apply();

        //- Write
        virtual void write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
