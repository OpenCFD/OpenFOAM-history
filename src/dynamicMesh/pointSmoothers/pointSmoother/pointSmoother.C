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

#include "pointSmoother.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(pointSmoother, 0);
    defineRunTimeSelectionTable(pointSmoother, dictionary);
}


// * * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * //

bool Foam::pointSmoother::isInternalOrProcessorFace(const label faceI) const
{
    if (mesh().isInternalFace(faceI))
    {
        return true;
    }

    if
    (
        processorPatchIDs_
        [
            mesh().boundaryMesh().patchID()
            [
                faceI - mesh().nInternalFaces()
            ]
        ]
    )
    {
        return true;
    }

    return false;
}


Foam::autoPtr<Foam::PackedBoolList> Foam::pointSmoother::pointsToMove
(
    const labelList& facesToMove,
    const bool moveInternalFaces
) const
{
    autoPtr<PackedBoolList> markerPtr
    (
        new PackedBoolList(mesh().nPoints(), false)
    );

    PackedBoolList& marker(markerPtr());

    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        if (moveInternalFaces || !isInternalOrProcessorFace(faceI))
        {
            const face& fPoints(mesh().faces()[faceI]);

            forAll(fPoints, fPointI)
            {
                const label pointI(fPoints[fPointI]);

                marker[pointI] = true;
            }
        }
    }

    syncTools::syncPointList
    (
        mesh(),
        marker,
        orEqOp<unsigned int>(),
        0U
    );

    return markerPtr;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointSmoother::pointSmoother
(
    const dictionary& dict,
    pointVectorField& pointDisplacement
)
:
    mesh_(pointDisplacement.mesh().mesh()),
    pointDisplacement_(pointDisplacement)
{
    forAll(mesh().boundaryMesh(), patchI)
    {
        const polyPatch& pp(mesh().boundaryMesh()[patchI]);

        if (isA<processorPolyPatch>(pp))
        {
            processorPatchIDs_.insert(patchI);
        }
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::pointSmoother>
Foam::pointSmoother::New
(
    const dictionary& dict,
    pointVectorField& pointDisplacement
)
{
    word pointSmootherType(dict.lookup(typeName));

    Info<< "Selecting pointSmoother type " << pointSmootherType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(pointSmootherType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn("pointSmoother::New")
            << "Unknown " << typeName << " type "
            << pointSmootherType << endl << endl
            << "Valid " << typeName << " types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(dict, pointDisplacement);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointSmoother::~pointSmoother()
{}


// ************************************************************************* //
