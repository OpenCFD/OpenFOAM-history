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

\*---------------------------------------------------------------------------*/

#include "refinementHistoryConstraint.H"
#include "addToRunTimeSelectionTable.H"
#include "syncTools.H"
#include "refinementHistory.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
defineTypeName(refinementHistoryConstraint);
addToRunTimeSelectionTable
(
    decompositionConstraint,
    refinementHistoryConstraint,
    dictionary
);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::refinementHistoryConstraint::refinementHistoryConstraint
(
    const dictionary& constraintsDict,
    const word& modelType
)
:
    decompositionConstraint(constraintsDict, typeName)
{
    if (decompositionConstraint::debug)
    {
        Info<< type() << " : setting constraints to preserve refinement history"
            << endl;
    }
}


Foam::refinementHistoryConstraint::refinementHistoryConstraint()
:
    decompositionConstraint(dictionary(), typeName)
{
    if (decompositionConstraint::debug)
    {
        Info<< type() << " : setting constraints to refinement history"
            << endl;
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::refinementHistoryConstraint::add
(
    const polyMesh& mesh,
    boolList& blockedFace,
    PtrList<labelList>& specifiedProcessorFaces,
    labelList& specifiedProcessor,
    List<labelPair>& explicitConnections
) const
{
    tmp<refinementHistory> historyPtr;
    if (mesh.foundObject<refinementHistory>("refinementHistory"))
    {
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : found refinementHistory" << endl;
        }
        historyPtr = tmp<refinementHistory>
        (
            const_cast<refinementHistory&>
            (
                mesh.lookupObject<refinementHistory>("refinementHistory")
            )
        );
    }
    else
    {
        // Load refinementHistory
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : reading refinementHistory from time "
                << mesh.facesInstance() << endl;
        }
        historyPtr = tmp<refinementHistory>
        (
            new refinementHistory
            (
                IOobject
                (
                    "refinementHistory",
                    mesh.facesInstance(),
                    polyMesh::meshSubDir,
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
    }
    const refinementHistory& history = historyPtr();

    // refinementHistory itself implements decompositionConstraint
    history.add
    (
        blockedFace,
        specifiedProcessorFaces,
        specifiedProcessor,
        explicitConnections
    );
}


void Foam::refinementHistoryConstraint::apply
(
    const polyMesh& mesh,
    const boolList& blockedFace,
    const PtrList<labelList>& specifiedProcessorFaces,
    const labelList& specifiedProcessor,
    const List<labelPair>& explicitConnections,
    labelList& decomposition
) const
{
    tmp<refinementHistory> historyPtr;
    if (mesh.foundObject<refinementHistory>("refinementHistory"))
    {
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : found refinementHistory" << endl;
        }
        historyPtr = tmp<refinementHistory>
        (
            const_cast<refinementHistory&>
            (
                mesh.lookupObject<refinementHistory>("refinementHistory")
            )
        );
    }
    else
    {
        // Load refinementHistory
        if (decompositionConstraint::debug)
        {
            Info<< type() << " : reading refinementHistory from time "
                << mesh.facesInstance() << endl;
        }
        historyPtr = tmp<refinementHistory>
        (
            new refinementHistory
            (
                IOobject
                (
                    "refinementHistory",
                    mesh.facesInstance(),
                    polyMesh::meshSubDir,
                    mesh,
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
    }
    const refinementHistory& history = historyPtr();

    // refinementHistory itself implements decompositionConstraint
    history.apply
    (
        blockedFace,
        specifiedProcessorFaces,
        specifiedProcessor,
        explicitConnections,
        decomposition
    );
}


// ************************************************************************* //
