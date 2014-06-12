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
    Foam::unregisteredIOdictionary

Description
    unregisteredIOdictionary is like IOdictionary but stores
    dependencies as files instead of fileMonitor watchIndices. Used
    to read controlDict since there fileMonitor not yet setup.

\*---------------------------------------------------------------------------*/

#ifndef unregisteredIOdictionary_H
#define unregisteredIOdictionary_H

#include "baseIOdictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class unregisteredIOdictionary Declaration
\*---------------------------------------------------------------------------*/

class unregisteredIOdictionary
:
    public baseIOdictionary
{

    fileNameList files_;

public:

    // Constructors

        //- Construct given an IOobject
        unregisteredIOdictionary(const IOobject& io);

        //- Construct given an IOobject and dictionary
        unregisteredIOdictionary
        (
            const IOobject& io,
            const dictionary& dict
        );

        //- Construct given an IOobject and Istream
        unregisteredIOdictionary(const IOobject& io, Istream& is);

    //- Destructor
    virtual ~unregisteredIOdictionary();


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

        //- Add file watch on object (READ_IF_MODIFIED)
        virtual void addWatch();

        //- Add file watch for fileName on object if not yet watched. Return
        //  index of watch
        virtual label addWatch(const fileName&);

        const fileNameList& files() const
        {
            return files_;
        }

        fileNameList& files()
        {
            return files_;
        }

};


//- Template function for obtaining global status
template<>
inline bool typeGlobal<unregisteredIOdictionary>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
