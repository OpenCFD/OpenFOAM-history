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
    Foam::functionObjectState

Description
    Base class for function objects, adding functionality to read/write state
    information to/from the state dictionary

See Also
    Foam::functionObject

SourceFiles
    functionObjectState.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjectState_H
#define functionObjectState_H

#include "objectRegistry.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class functionObjectState Declaration
\*---------------------------------------------------------------------------*/

class functionObjectState
{
private:

    // Private data

        //- Reference to the database
        const objectRegistry& obr_;


protected:

    // Protected data

        //- Name of model
        const word name_;

        //- Flag to indicate whether the object is active
        bool active_;

        //- Reference to the state dictionary
        dictionary& stateDict_;


protected:

    // Protected Member Functions


        //- Disallow default bitwise copy construct
        functionObjectState(const functionObjectState&);

        //- Disallow default bitwise assignment
        void operator=(const functionObjectState&);


public:

    // Constructors

        //- Construct from components
        functionObjectState
        (
            const objectRegistry& obr,
            const word& name
        );


    //- Destructor
    virtual ~functionObjectState();


    // Member Functions

        //- Set the active status by querying objectRegistry type
        //  returns new active status
        template<class Type>
        bool setActive();

        //- Return true if the property exists
        bool foundProperty(const word& entryName) const;

        //- Retrieve generic property
        template<class Type>
        Type getProperty
        (
            const word& entryName,
            const Type& defaultValue = pTraits<Type>::zero
        ) const;

        //- Retrieve generic property
        template<class Type>
        void getProperty(const word& entryName, Type& value) const;

        //- Add generic property
        template<class Type>
        void setProperty(const word& entryName, const Type& value);

        //- Retrieve generic property from named object
        template<class Type>
        Type getObjectProperty
        (
            const word& objectName,
            const word& entryName,
            const Type& defaultValue = pTraits<Type>::zero
        ) const;

        //- Retrieve generic property from named object
        template<class Type>
        void getObjectProperty
        (
            const word& objectName,
            const word& entryName,
            Type& value
        ) const;

        //- Add generic property from named object
        template<class Type>
        void setObjectProperty
        (
            const word& objectName,
            const word& entryName,
            const Type& value
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "functionObjectStateTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
