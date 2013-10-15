/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013 OpenFOAM Foundation
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
    Foam::localIOdictionary

Description
    localIOdictionary is derived from dictionary and IOobject to give the
    dictionary
    automatic IO functionality via the objectRegistry.  To facilitate IO,
    IOdictioanry is provided with a constructor from IOobject and writeData and
    write functions.

SourceFiles
    localIOdictionary.C
    localIOdictionaryIO.C

\*---------------------------------------------------------------------------*/

#ifndef localIOdictionary_H
#define localIOdictionary_H

#include "dictionary.H"
#include "regIOobject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class localIOdictionary Declaration
\*---------------------------------------------------------------------------*/

class localIOdictionary
:
    public regIOobject,
    public dictionary
{

    // Private Member Functions

        //- Wrapper around readFile. Returns true if file read.
        bool readFile();


protected:

    // Protected data

        static bool writeDictionaries;


public:

    TypeName("dictionary");


    // Constructors

        //- Construct given an IOobject
        localIOdictionary(const IOobject&);

        //- Construct given an IOobject and dictionary
        localIOdictionary(const IOobject&, const dictionary&);

        //- Construct given an IOobject and Istream
        localIOdictionary(const IOobject&, Istream&);


    //- Destructor
    virtual ~localIOdictionary();


    // Member functions

//        //- Return complete path + object name if the file exists
//        //  either in the case/processor or case otherwise null
//        virtual fileName filePath() const;

        //- Name function is needed to disambiguate those inherited
        //  from regIOobject and dictionary
        const word& name() const;

        //- ReadData function required for regIOobject read operation
        virtual bool readData(Istream&);

        //- WriteData function required for regIOobject write operation
        virtual bool writeData(Ostream&) const;

//        //- Is object global
//        virtual bool global() const
//        {
//            return true;
//        }

    // Member operators

        //- Assignment of other localIOdictionary's entries to this localIOdictionary
        void operator=(const localIOdictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
