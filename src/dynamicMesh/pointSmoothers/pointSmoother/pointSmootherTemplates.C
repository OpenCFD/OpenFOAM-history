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
#include "syncTools.H"

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template <class weightType>
void Foam::pointSmoother::reset
(
    const labelList& facesToMove,
    Field<weightType>& weights,
    vectorField& displacements,
    const bool resetInternalFaces
) const
{
    PackedBoolList resetPoints(mesh().nPoints(), false);

    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        if (resetInternalFaces || !isInternalOrProcessorFace(faceI))
        {
            const face& facePoints(mesh().faces()[faceI]);

            forAll(facePoints, facePointI)
            {
                const label pointI(facePoints[facePointI]);

                resetPoints[pointI] = true;
            }
        }
    }

    syncTools::syncPointList
    (
        mesh(),
        resetPoints,
        orEqOp<unsigned int>(),
        0U
    );

    forAll(resetPoints, pointI)
    {
        if (resetPoints[pointI])
        {
            weights[pointI] = pTraits<weightType>::zero;
            displacements[pointI] = vector::zero;
        }
    }
}


template <class weightType>
void Foam::pointSmoother::average
(
    const labelList& facesToMove,
    Field<weightType>& weights,
    vectorField& displacements
) const
{
    syncTools::syncPointList
    (
        mesh(),
        weights,
        plusEqOp<weightType>(),
        pTraits<weightType>::zero
    );

    syncTools::syncPointList
    (
        mesh(),
        displacements,
        plusEqOp<vector>(),
        vector::zero
    );

    PackedBoolList averagePoints(mesh().nPoints(), false);

    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        const face& facePoints(mesh().faces()[faceI]);

        forAll(facePoints, facePointI)
        {
            const label pointI(facePoints[facePointI]);

            averagePoints[pointI] = true;
        }
    }

    syncTools::syncPointList
    (
        mesh(),
        averagePoints,
        orEqOp<unsigned int>(),
        0U
    );

    forAll(averagePoints, pointI)
    {
        if (averagePoints[pointI])
        {
            displacements[pointI] /= weights[pointI];
        }
    }
}


// ************************************************************************* //
