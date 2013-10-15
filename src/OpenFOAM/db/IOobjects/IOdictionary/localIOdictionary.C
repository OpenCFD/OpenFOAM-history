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

\*---------------------------------------------------------------------------*/

#include "localIOdictionary.H"
#include "objectRegistry.H"
#include "Pstream.H"
#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
defineTypeNameAndDebug(localIOdictionary, 0);

bool localIOdictionary::writeDictionaries
(
    debug::infoSwitch("writeDictionaries", 0)
);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::localIOdictionary::readFile()
{
    // Temporary warning
    if (debug && readOpt() == IOobject::MUST_READ)
    {
        WarningIn("localIOdictionary::localIOdictionary(const IOobject&)")
            << "Dictionary " << name()
            << " constructed with IOobject::MUST_READ"
            " instead of IOobject::MUST_READ_IF_MODIFIED." << nl
            << "Use MUST_READ_IF_MODIFIED if you need automatic rereading."
            << endl;
    }

    if
    (
        (
            readOpt() == IOobject::MUST_READ
         || readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || (readOpt() == IOobject::READ_IF_PRESENT && headerOk())
    )
    {
        return regIOobject::read(false, IOstream::ASCII, typeName);
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::localIOdictionary::localIOdictionary(const IOobject& io)
:
    regIOobject(io)
{
    readFile();

    dictionary::name() = IOobject::objectPath();
}


Foam::localIOdictionary::localIOdictionary
(
    const IOobject& io,
    const dictionary& dict
)
:
    regIOobject(io)
{
    if (!readFile())
    {
        dictionary::operator=(dict);
    }

    dictionary::name() = IOobject::objectPath();
}


Foam::localIOdictionary::localIOdictionary
(
    const IOobject& io,
    Istream& is
)
:
    regIOobject(io)
{
    dictionary::name() = IOobject::objectPath();
    // Note that we do construct the dictionary null and read in afterwards
    // so that if there is some fancy massaging due to a functionEntry in
    // the dictionary at least the type information is already complete.
    is  >> *this;
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

Foam::localIOdictionary::~localIOdictionary()
{}


// * * * * * * * * * * * * * * * Members Functions * * * * * * * * * * * * * //

const Foam::word& Foam::localIOdictionary::name() const
{
    return regIOobject::name();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void Foam::localIOdictionary::operator=(const localIOdictionary& rhs)
{
    dictionary::operator=(rhs);
}


// ************************************************************************* //
