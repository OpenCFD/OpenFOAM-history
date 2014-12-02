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
    pointVectorField& pointDisplacement
)
:
    pointSmoother(dict, pointDisplacement),
    cellPointConnectivity_
    (
        MeshObject<polyMesh, MoveableMeshObject, cellPointConnectivity>::New
        (
            mesh()
        )
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
    const pointField& oldPoints,
    const pointField& currentPoints,
    polyMeshGeometry& meshGeometry
)
{
    // Number of points used in each average
    labelField counts(mesh().nPoints(), -1);

    // Reset the displacements which are about to be calculated
    reset(facesToMove, counts);

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

        const cell& cFaces
        (
            mesh().cells()[cellI]
        );
        const labelList cellPoints
        (
            cFaces.labels(mesh().faces())
        );
        const edgeList cellEdges
        (
            cFaces.edges(mesh().faces())
        );

        // Calculate a transformed point for each cell point
        forAll(cellPoints, cellPointI)
        {
            const label pointI(cellPoints[cellPointI]);

            if (counts[pointI] == -1) continue;

            const labelList& pPoints
            (
                cellPointConnectivity_.cellPointPoints()[cellI][cellPointI]
            );
            const labelList& pFaces
            (
                cellPointConnectivity_.cellPointFaces()[cellI][cellPointI]
            );
            const label nPPoints(pPoints.size());

            // Initial guess of the dual face centre
            vector dualAverage(vector::zero);
            forAll(pPoints, pointPointI)
            {
                dualAverage +=
                    currentPoints[pPoints[pointPointI]]
                  + meshGeometry.faceCentres()[pFaces[pointPointI]];
            }
            dualAverage /= 2*nPPoints;

            // Calculate the dual face centre and normal
            vector dualNormal(vector::zero);
            forAll(pPoints, pointPointI)
            {
                const label nextPPointI((pointPointI + 1) % nPPoints);

                point edgeCentre
                (
                    0.5
                   *(
                        currentPoints[pPoints[pointPointI]]
                      + currentPoints[pointI]
                    )
                );
                point nextFaceCentre
                (
                    meshGeometry.faceCentres()[pFaces[nextPPointI]]
                );
                point nextEdgeCentre
                (
                    0.5
                   *(
                        currentPoints[pPoints[nextPPointI]]
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
            forAll(pPoints, pointPointI)
            {
                const label nextPPointI((pointPointI + 1) % nPPoints);

                point edgeCentre
                (
                    0.5
                   *(
                        currentPoints[pPoints[pointPointI]]
                      + currentPoints[pointI]
                    )
                );
                point nextFaceCentre
                (
                    meshGeometry.faceCentres()[pFaces[nextPPointI]]
                );
                point nextEdgeCentre
                (
                    0.5
                   *(
                        currentPoints[pPoints[nextPPointI]]
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

            pointDisplacement()[pointI] += newPoint - oldPoints[pointI];
        }
    }

    // Reset all the boundary faces
    reset(facesToMove, counts, false);

    // Calculate the boundary transformations
    forAll(facesToMove, faceToMoveI)
    {
        const label faceI(facesToMove[faceToMoveI]);

        if (!isInternalOrProcessorFace(faceI))
        {
            const labelList& fPoints(mesh().faces()[faceI]);

            // Face normal
            vector faceNormalHat(meshGeometry.faceAreas()[faceI]);
            faceNormalHat /= max(SMALL, mag(faceNormalHat));

            // Calculate a transformed point for each face point
            forAll(fPoints, fPointI)
            {
                const label pointI(fPoints[fPointI]);

                const label fPointIPrev
                (
                    (fPointI - 1 + fPoints.size()) % fPoints.size()
                );
                const label fPointINext
                (
                    (fPointI + 1) % fPoints.size()
                );

                const vector dualCentre
                (
                    currentPoints[pointI]/2
                  + (
                        currentPoints[fPoints[fPointINext]]
                      + currentPoints[fPoints[fPointIPrev]]
                    )/4
                );
                const vector dualNormal
                (
                    (
                        currentPoints[fPoints[fPointINext]]
                      - currentPoints[fPoints[fPointIPrev]]
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
            forAll(fPoints, fPointI)
            {
                const label fPointINext((fPointI + 1)%fPoints.size());

                const label pointI(fPoints[fPointI]);
                const label pointINext(fPoints[fPointINext]);

                lengthScale += mag
                (
                    currentPoints[pointINext] - currentPoints[pointI]
                );
                transformedLengthScale += mag
                (
                    transformedPoints[pointINext] - transformedPoints[pointI]
                );
            }
            lengthScale /= fPoints.size();
            transformedLengthScale /= fPoints.size();
            lengthScaleRatio = lengthScale/transformedLengthScale;

            // Add the displacement to the average
            forAll(fPoints, fPointI)
            {
                const label pointI(fPoints[fPointI]);

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

                pointDisplacement()[pointI] += newPoint - oldPoints[pointI];
            }
        }
    }

    // Average
    average(facesToMove, counts);
}


// ************************************************************************* //
