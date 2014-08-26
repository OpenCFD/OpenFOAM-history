/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd
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

#include "geometryBase.H"
#include "runTimePostProcessing.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::geometryBase::geometryBase
(
    const runTimePostProcessing& parent,
    const dictionary& dict,
    const HashTable<vector, word>& colours
)
:
    parent_(parent),
    name_(dict.dictName()),
    visible_(readBool(dict.lookup("visible"))),
    opacity_(dict.lookupOrDefault("opacity", 1.0)),
    colours_(colours)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::geometryBase::~geometryBase()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::runTimePostProcessing& Foam::geometryBase::parent() const
{
    return parent_;
}


const Foam::word& Foam::geometryBase::name() const
{
    return name_;
}


bool Foam::geometryBase::visible() const
{
    return visible_;
}


Foam::scalar Foam::geometryBase::opacity() const
{
    return opacity_;
}


const Foam::HashTable<Foam::vector, Foam::word>&
Foam::geometryBase::colours() const
{
    return colours_;
}


// ************************************************************************* //
