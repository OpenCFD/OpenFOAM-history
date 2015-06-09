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

#include "blendingFactor.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(blendingFactor, 0);
}


// * * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * //

void Foam::blendingFactor::writeFileHeader(Ostream& os) const
{
    writeHeader(os, "Blending factor");
    writeCommented(os, "Time");
    writeTabbed(os, "Scheme1");
    writeTabbed(os, "Scheme2");
    writeTabbed(os, "Blended");
    os  << endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::blendingFactor::blendingFactor
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectState(obr, name),
    functionObjectFile(obr, name, typeName, dict),
    name_(name),
    obr_(obr),
    phiName_("phi"),
    fieldName_("unknown-fieldName"),
    tolerance_(0.001),
    log_(true)
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (setActive<fvMesh>())
    {
        read(dict);
        writeFileHeader(file());
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::blendingFactor::~blendingFactor()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::blendingFactor::read(const dictionary& dict)
{
    if (active_)
    {
        functionObjectFile::read(dict);

        log_.readIfPresent("log", dict);

        dict.readIfPresent("phiName", phiName_);
        dict.lookup("fieldName") >> fieldName_;
        dict.readIfPresent("tolerance", tolerance_);

        if ((tolerance_ < 0) || (tolerance_ > 1))
        {
            FatalErrorIn("void Foam::blendingFactor::read(const dictionary&)")
                << "tolerance must be in the range 0 to 1.  Supplied value: "
                << tolerance_ << exit(FatalError);
        }
    }
}


void Foam::blendingFactor::execute()
{
    if (active_)
    {
        calc<scalar>();
        calc<vector>();
    }
}


void Foam::blendingFactor::end()
{
    if (active_)
    {
        execute();
    }
}

void Foam::blendingFactor::timeSet()
{
    // Do nothing
}


void Foam::blendingFactor::write()
{
    if (active_)
    {
        const word fieldName = "blendingFactor:" + fieldName_;

        const volScalarField& blendingFactor =
            obr_.lookupObject<volScalarField>(fieldName);

        Info(log_)
            << type() << " " << name_ << " output:" << nl
            << "    writing field " << blendingFactor.name() << nl
            << endl;

        blendingFactor.write();
    }
}


// ************************************************************************* //
