/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenFOAM Foundation
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
    Foam::globalIOField

Description
    IOField with global search scope.

SourceFiles
    globalIOField.C

\*---------------------------------------------------------------------------*/

#ifndef globalIOField_H
#define globalIOField_H

#include "regIOobject.H"
#include "Field.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class globalIOField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class globalIOField
:
    public regIOobject,
    public Field<Type>
{

public:

    TypeName("Field");


    // Constructors

        //- Construct from IOobject
        globalIOField(const IOobject&);

        //- Construct from IOobject and size (does not set values)
        globalIOField(const IOobject&, const label size);

        //- Construct from components
        globalIOField(const IOobject&, const Field<Type>&);

        //- Construct by transferring the Field contents
        globalIOField(const IOobject&, const Xfer<Field<Type> >&);


    //- Destructor
    virtual ~globalIOField();


    // Member functions

        //- Is object global
        virtual bool global() const
        {
            return true;
        }

        //- Return complete path + object name if the file exists
        //  either in the case/processor or case otherwise null
        virtual fileName filePath() const
        {
            return globalFilePath();
        }

        bool writeData(Ostream&) const;


    // Member operators

        void operator=(const globalIOField<Type>&);

        void operator=(const Field<Type>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "globalIOField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
