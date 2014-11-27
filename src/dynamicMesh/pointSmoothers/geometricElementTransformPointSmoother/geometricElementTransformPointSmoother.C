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

#include "geometricElementTransformPointSmoother.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace pointSmoothers
{
    defineTypeNameAndDebug(geometricElementTransformPointSmoother, 0);
    addToRunTimeSelectionTable
    (
        pointSmoother,
        geometricElementTransformPointSmoother,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointSmoothers::geometricElementTransformPointSmoother::
geometricElementTransformPointSmoother
(
    const dictionary& dict,
    const polyMesh& mesh
)
:
    pointSmoother(dict, mesh),
    cellPointConnectivity_
    (
        cellPointConnectivity::get(mesh)
    ),
    transformationParameter_
    (
        readScalar(dict.lookup("transformationParameter"))
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointSmoothers::geometricElementTransformPointSmoother::
~geometricElementTransformPointSmoother()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::pointSmoothers::geometricElementTransformPointSmoother::update
(
    const labelList& facesToMove,
    const polyMeshGeometry& meshGeometry,
    const pointField& oldPoints,
    const pointField& currentPoints,
    vectorField& displacements
) const
{
    // Number of points used in each average
    labelField counts(mesh().nPoints(), -1);

    // Reset the displacements which are about to be calculated
    reset(facesToMove, counts, displacements);

    // Identify the cells which are to be moved
    labelHashSet cellsToMove(facesToMove.size()*2/3);
    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        cellsToMove.insert(mesh().faceOwner()[faceI]);

        if (mesh().isInternalFace(faceI))
        {
            cellsToMove.insert(mesh().faceNeighbour()[faceI]);
        }
    }

    // Transformed point field
    pointField transformedPoints(currentPoints);

    // Calculate the internal transformations
    forAllConstIter(labelHashSet, cellsToMove, iter)
    {
        const label cellI(iter.key());

        const labelList cellFaceLabels
        (
            mesh().cells()[cellI]
        );
        const labelList cellPoints
        (
            mesh().cells()[cellI].labels(mesh().faces())
        );
        const edgeList cellEdges
        (
            mesh().cells()[cellI].edges(mesh().faces())
        );

        // Calculate a transformed point for each cell point
        forAll(cellPoints, cellPointI)
        {
            const label pointI(cellPoints[cellPointI]);

            if (counts[pointI] == -1) continue;

            const labelList& pointPoints
            (
                cellPointConnectivity_.cellPointPoints()[cellI][cellPointI]
            );
            const labelList& pointFaces
            (
                cellPointConnectivity_.cellPointFaces()[cellI][cellPointI]
            );
            const label nPointPoints(pointPoints.size());

            // Initial guess of the dual face centre
            vector dualAverage(vector::zero);
            forAll(pointPoints, pointPointI)
            {
                dualAverage +=
                    currentPoints[pointPoints[pointPointI]]
                  + meshGeometry.faceCentres()[pointFaces[pointPointI]];
            }
            dualAverage /= 2*nPointPoints;

            // Calculate the dual face centre and normal
            vector dualNormal(vector::zero);
            forAll(pointPoints, pointPointI)
            {
                const label nextPointPointI((pointPointI + 1) % nPointPoints);

                point edgeCentre
                (
                    0.5
                   *(
                        currentPoints[pointPoints[pointPointI]]
                      + currentPoints[pointI]
                    )
                );
                point nextFaceCentre
                (
                    meshGeometry.faceCentres()[pointFaces[nextPointPointI]]
                );
                point nextEdgeCentre
                (
                    0.5
                   *(
                        currentPoints[pointPoints[nextPointPointI]]
                      + currentPoints[pointI]
                    )
                );

                dualNormal +=
                    (nextFaceCentre - edgeCentre)
                   ^(edgeCentre - dualAverage);
                dualNormal +=
                    (nextEdgeCentre - nextFaceCentre)
                   ^(nextFaceCentre - dualAverage);
            }
            vector dualNormalHat(dualNormal/max(mag(dualNormal), ROOTVSMALL));

            scalar sumA(0);
            vector sumAc(vector::zero);
            forAll(pointPoints, pointPointI)
            {
                const label nextPointPointI((pointPointI + 1) % nPointPoints);

                point edgeCentre
                (
                    0.5
                   *(
                        currentPoints[pointPoints[pointPointI]]
                      + currentPoints[pointI]
                    )
                );
                point nextFaceCentre
                (
                    meshGeometry.faceCentres()[pointFaces[nextPointPointI]]
                );
                point nextEdgeCentre
                (
                    0.5
                   *(
                        currentPoints[pointPoints[nextPointPointI]]
                      + currentPoints[pointI]
                    )
                );

                vector c1 = edgeCentre + nextFaceCentre + dualAverage;
                vector c2 = nextFaceCentre + nextEdgeCentre + dualAverage;

                vector n1 =
                    (nextFaceCentre - edgeCentre)
                   ^(edgeCentre - dualAverage);
                vector n2 =
                    (nextEdgeCentre - nextFaceCentre)
                   ^(nextFaceCentre - dualAverage);

                scalar a1 = n1 & dualNormalHat;
                scalar a2 = n2 & dualNormalHat;

                sumA += a1 + a2;
                sumAc += a1*c1 + a2*c2;
            }

            const vector dualCentre(sumAc/max(sumA, ROOTVSMALL)/3);

            // Calculate the transformed point
            transformedPoints[pointI] =
                dualCentre
              + transformationParameter_
               *dualNormal/sqrt(max(mag(dualNormal), ROOTVSMALL));
        }

        // Length scale
        scalar lengthScale(0), transformedLengthScale(0), lengthScaleRatio;
        forAll(cellEdges, edgeI)
        {
            lengthScale +=
                cellEdges[edgeI].mag(currentPoints);
            transformedLengthScale +=
                cellEdges[edgeI].mag(transformedPoints);
        }
        lengthScale /= cellEdges.size();
        transformedLengthScale /= cellEdges.size();
        lengthScaleRatio = lengthScale/transformedLengthScale;

        // Add the displacement to the average
        forAll(cellPoints, cellPointI)
        {
            const label pointI(cellPoints[cellPointI]);

            if (counts[pointI] == -1) continue;

            const vector newPoint
            (
                meshGeometry.cellCentres()[cellI]
              + lengthScaleRatio
               *(
                    transformedPoints[pointI]
                  - meshGeometry.cellCentres()[cellI]
                )
            );

            ++ counts[pointI];

            displacements[pointI] += newPoint - oldPoints[pointI];
        }
    }

    // Reset all the boundary faces
    reset(facesToMove, counts, displacements, false);

    // Calculate the boundary transformations
    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        if (!isInternalOrProcessorFace(faceI))
        {
            const labelList& facePoints(mesh().faces()[faceI]);

            // Face normal
            vector faceNormalHat(meshGeometry.faceAreas()[faceI]);
            faceNormalHat /= max(SMALL, mag(faceNormalHat));

            // Calculate a transformed point for each face point
            forAll(facePoints, facePointI)
            {
                const label pointI(facePoints[facePointI]);

                const label facePointIPrev
                (
                    (facePointI - 1 + facePoints.size()) % facePoints.size()
                );
                const label facePointINext
                (
                    (facePointI + 1) % facePoints.size()
                );

                const vector dualCentre
                (
                    currentPoints[pointI]/2
                  + (
                        currentPoints[facePoints[facePointINext]]
                      + currentPoints[facePoints[facePointIPrev]]
                    )/4
                );
                const vector dualNormal
                (
                    (
                        currentPoints[facePoints[facePointINext]]
                      - currentPoints[facePoints[facePointIPrev]]
                    )/2
                   ^faceNormalHat
                );

                transformedPoints[pointI] =
                    dualCentre
                  + transformationParameter_
                   *dualNormal/sqrt(max(mag(dualNormal), ROOTVSMALL));
            }

            // Length scale
            scalar lengthScale(0), transformedLengthScale(0), lengthScaleRatio;
            forAll(facePoints, facePointI)
            {
                const label facePointINext((facePointI + 1)%facePoints.size());

                const label pointI(facePoints[facePointI]);
                const label pointINext(facePoints[facePointINext]);

                lengthScale += mag
                (
                    currentPoints[pointINext] - currentPoints[pointI]
                );
                transformedLengthScale += mag
                (
                    transformedPoints[pointINext] - transformedPoints[pointI]
                );
            }
            lengthScale /= facePoints.size();
            transformedLengthScale /= facePoints.size();
            lengthScaleRatio = lengthScale/transformedLengthScale;

            // Add the displacement to the average
            forAll(facePoints, facePointI)
            {
                const label pointI(facePoints[facePointI]);

                const vector newPoint
                (
                    meshGeometry.faceCentres()[faceI]
                  + lengthScaleRatio
                   *(
                        transformedPoints[pointI]
                      - meshGeometry.faceCentres()[faceI]
                    )
                );

                ++ counts[pointI];

                displacements[pointI] += newPoint - oldPoints[pointI];
            }
        }
    }

    // Average
    average(facesToMove, counts, displacements);
}


// ************************************************************************* //
