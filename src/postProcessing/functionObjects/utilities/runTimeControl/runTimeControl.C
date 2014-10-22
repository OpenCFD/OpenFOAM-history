/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd.
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

#include "runTimeControl.H"
#include "dictionary.H"
#include "runTimeCondition.H"
#include "fvMesh.H"
#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(runTimeControl, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::runTimeControl::runTimeControl
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectState(obr, name),
    obr_(obr),
    active_(true),
    conditions_()
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningIn
        (
            "runTimeControl::runTimeControl"
            "("
                "const word&, "
                "const objectRegistry&, "
                "const dictionary&, "
                "const bool"
            ")"
        )   << "No fvMesh available, deactivating " << name_ << nl
            << endl;
    }

    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::runTimeControl::~runTimeControl()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::runTimeControl::read(const dictionary& dict)
{
    if (active_)
    {
        const dictionary& conditionsDict = dict.subDict("conditions");
        const wordList conditionNames(conditionsDict.toc());
        conditions_.setSize(conditionNames.size());

        forAll(conditionNames, conditionI)
        {
            const word& conditionName = conditionNames[conditionI];
            const dictionary& dict = conditionsDict.subDict(conditionName);

            conditions_.set
            (
                conditionI,
                runTimeCondition::New(conditionName, obr_, dict, *this)
            );
        }

    }
}


void Foam::runTimeControl::execute()
{
    if (active_)
    {
        bool done = false;
        DynamicList<label> IDs(10);
        forAll(conditions_, conditionI)
        {
            bool conditionSatisfied = conditions_[conditionI].apply();

            if (conditionSatisfied)
            {
                IDs.append(conditionI);
            }

            done = done || conditionSatisfied;
        }

        if (done)
        {
            Info<< type() << " output:" << nl;

            forAll(IDs, conditionI)
            {
                Info<< "    " << conditions_[conditionI].type() << ": "
                    <<  conditions_[conditionI].name()
                    << " condition satisfied" << nl;
            }

            Info<< "    Stopping calculation" << nl << endl;

            // Set to finalise calculation
            Time& time = const_cast<Time&>(obr_.time());
            time.writeAndEnd();
        }
    }
}


void Foam::runTimeControl::end()
{
    if (active_)
    {
        execute();
    }
}


void Foam::runTimeControl::timeSet()
{
    // Do nothing
}


void Foam::runTimeControl::write()
{
    if (active_)
    {
        forAll(conditions_, conditionI)
        {
            conditions_[conditionI].write();
        }
    }
}


// ************************************************************************* //
