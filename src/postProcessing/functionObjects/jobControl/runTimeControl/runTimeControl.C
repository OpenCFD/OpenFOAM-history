/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2015 OpenCFD Ltd.
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
    conditions_(),
    groupMap_()
{
    // Check if the available mesh is an fvMesh, otherwise deactivate
    if (setActive<fvMesh>())
    {
        read(dict);
    }
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

        groupMap_(conditions_.size());

        label nGroup = 0;

        forAll(conditionNames, conditionI)
        {
            const word& conditionName = conditionNames[conditionI];
            const dictionary& dict = conditionsDict.subDict(conditionName);

            conditions_.set
            (
                conditionI,
                runTimeCondition::New(conditionName, obr_, dict, *this)
            );

            label groupI = conditions_[conditionI].groupID();
            if (groupMap_.found(groupI))
            {
                groupMap_.set(groupI, nGroup++);
            }
        }
    }
}


void Foam::runTimeControl::execute()
{
    if (active_)
    {
        // IDs of satisfied conditions
        DynamicList<label> IDs(conditions_.size());

        // Run stops only if all conditions within a group are satisfied
        List<bool> groupSatisfied(groupMap_.size(), true);

        forAll(conditions_, conditionI)
        {
            runTimeCondition& condition = conditions_[conditionI];

            bool conditionSatisfied = condition.apply();

            label groupI = condition.groupID();
            Map<label>::const_iterator conditionIter = groupMap_.find(groupI);

            if (conditionIter == groupMap_.end())
            {
                FatalErrorIn("void Foam::runTimeControl::execute()")
                    << "group " << groupI << " not found in map"
                    << abort(FatalError);
            }

            if (conditionSatisfied)
            {
                IDs.append(conditionI);

                if (groupI == -1)
                {
                    groupSatisfied[conditionIter()] = true;
                    break;
                }
            }
            else
            {
                groupSatisfied[conditionIter()] = false;
            }
        }

        bool done = false;
        forAll(groupSatisfied, groupI)
        {
            if (groupSatisfied[groupI])
            {
                done = true;
                break;
            }
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
