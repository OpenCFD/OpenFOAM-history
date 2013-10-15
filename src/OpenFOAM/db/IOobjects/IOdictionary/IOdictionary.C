/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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

#include "IOdictionary.H"
#include "objectRegistry.H"
#include "Pstream.H"
#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
defineTypeNameAndDebug(IOdictionary, 0);

bool IOdictionary::writeDictionaries
(
    debug::infoSwitch("writeDictionaries", 0)
);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::IOdictionary::readFile()
{
    // Temporary warning
    if (debug && readOpt() == IOobject::MUST_READ)
    {
        WarningIn("IOdictionary::IOdictionary(const IOobject&)")
            << "Dictionary " << name()
            << " constructed with IOobject::MUST_READ"
            " instead of IOobject::MUST_READ_IF_MODIFIED." << nl
            << "Use MUST_READ_IF_MODIFIED if you need automatic rereading."
            << endl;
    }

    // Everyone check or just master
    bool masterOnly =
        global()
     && (
            regIOobject::fileModificationChecking == timeStampMaster
         || regIOobject::fileModificationChecking == inotifyMaster
        );


    // Check if header is ok for READ_IF_PRESENT
    bool isHeaderOk = false;
    if (readOpt() == IOobject::READ_IF_PRESENT)
    {
        if (masterOnly)
        {
            if (Pstream::master())
            {
                isHeaderOk = headerOk();
            }
            Pstream::scatter(isHeaderOk);
        }
        else
        {
            isHeaderOk = headerOk();
        }
    }


    if
    (
        (
            readOpt() == IOobject::MUST_READ
         || readOpt() == IOobject::MUST_READ_IF_MODIFIED
        )
     || isHeaderOk
    )
    {
        return regIOobject::read(masterOnly, IOstream::ASCII, typeName);
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::IOdictionary::IOdictionary(const IOobject& io)
:
    regIOobject(io)
{
    globalObject() = true;

    readFile();

    dictionary::name() = IOobject::objectPath();
}


Foam::IOdictionary::IOdictionary(const IOobject& io, const dictionary& dict)
:
    regIOobject(io)
{
    globalObject() = true;

    if (!readFile())
    {
        dictionary::operator=(dict);
    }

    dictionary::name() = IOobject::objectPath();
}


Foam::IOdictionary::IOdictionary(const IOobject& io, Istream& is)
:
    regIOobject(io)
{
    globalObject() = true;

    dictionary::name() = IOobject::objectPath();
    // Note that we do construct the dictionary null and read in afterwards
    // so that if there is some fancy massaging due to a functionEntry in
    // the dictionary at least the type information is already complete.
    is  >> *this;
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

Foam::IOdictionary::~IOdictionary()
{}


// * * * * * * * * * * * * * * * Members Functions * * * * * * * * * * * * * //

Foam::fileName Foam::IOdictionary::filePath() const
{
    if (instance().isAbsolute())
    {
        fileName objectPath = instance()/name();

        if (isFile(objectPath))
        {
            Pout<< "IOdictionary : returning absolute:" << objectPath << endl;
            return objectPath;
        }
        else
        {
            Pout<< "IOdictionary : absolute not found:" << objectPath << endl;
            return fileName::null;
        }
    }
    else
    {
        fileName path = this->path();
        fileName objectPath = path/name();
        //Pout<< "IOdictionary : objectPath:" << objectPath << endl;

        if
        (
            instance() == time().system()
         || instance() == time().constant()
        )
        {
            // Constant & system come from global case

            if (time().processorCase())
            {
                fileName parentObjectPath =
                    rootPath()/time().globalCaseName()
                   /instance()/db().dbDir()/local()/name();

                if (isFile(parentObjectPath))
                {
                    Pout<< "IOdictionary : returning parent:"
                        << parentObjectPath << endl;
                    return parentObjectPath;
                }
                else
                {
                    Pout<< "IOdictionary : parent not found:"
                        << parentObjectPath << endl;
                    return fileName::null;
                }
            }
            else if (isFile(objectPath))
            {
                Pout<< "IOdictionary : returning local:"
                    << objectPath << endl;
                return objectPath;
            }
            else
            {

                Pout<< "IOdictionary : local not found:"
                    << objectPath << endl;
                return fileName::null;
            }
        }
        // Dictionary from time directory
        else if (isFile(objectPath))
        {
            Pout<< "IOdictionary : returning time:"
                << objectPath << endl;
            return objectPath;
        }
        else
        {
            // Check for approximately same time
            if (!isDir(path))
            {
                word newInstancePath = time().findInstancePath
                (
                    instant(instance())
                );

                if (newInstancePath.size())
                {
                    fileName fName
                    (
                        rootPath()/caseName()
                       /newInstancePath/db().dbDir()/local()/name()
                    );

                    if (isFile(fName))
                    {
                        Pout<< "IOdictionary : returning similar time:"
                            << fName << endl;

                        return fName;
                    }
                }
            }

            Pout<< "IOdictionary : time not found:"
                << objectPath << endl;
            return fileName::null;
        }
    }
}


const Foam::word& Foam::IOdictionary::name() const
{
    return regIOobject::name();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void Foam::IOdictionary::operator=(const IOdictionary& rhs)
{
    dictionary::operator=(rhs);
}


// ************************************************************************* //
