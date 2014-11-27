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

#include "cellPointConnectivity.H"
#include "Time.H"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

const Foam::cellPointConnectivity& Foam::cellPointConnectivity::get
(
    const polyMesh& mesh
)
{
    if (!mesh.foundObject<cellPointConnectivity>("cellPointConnectivity"))
    {
        cellPointConnectivity* ptr =
            new cellPointConnectivity
            (
                IOobject
                (
                    "cellPointConnectivity",
                    mesh.time().timeName(),
                    mesh
                ),
                mesh
          );
        ptr->store();
    }

    return mesh.lookupObject<cellPointConnectivity>("cellPointConnectivity");
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::cellPointConnectivity::generateCellPointConnectivity(label cellI)
{
    const cell& thisCell(mesh_.cells()[cellI]);

    const labelList cellPoints(thisCell.labels(mesh_.faces()));
    const labelList cellFaces(thisCell);
    const edgeList cellEdges(thisCell.edges(mesh_.faces()));

    // Generate a sorted list of points and corresponding point indices
    labelPairList pointLabelPointIndices(cellPoints.size());
    forAll(cellPoints, pointI)
    {
        pointLabelPointIndices[pointI] =
            labelPair(cellPoints[pointI], pointI);
    }
    sort(pointLabelPointIndices);

    // Generate a sorted list of edge labels and corresponding edge indices
    // Negative values indicate an edge which runs in an opposite direction to
    // the face node listing
    labelListList edgeLabelsEdgeIndices(2*cellEdges.size(), labelList(3,-1));
    forAll(cellEdges, cellEdgeI)
    {
        edgeLabelsEdgeIndices[2*cellEdgeI][0] = cellEdges[cellEdgeI][0];
        edgeLabelsEdgeIndices[2*cellEdgeI][1] = cellEdges[cellEdgeI][1];
        edgeLabelsEdgeIndices[2*cellEdgeI][2] = cellEdgeI;

        edgeLabelsEdgeIndices[2*cellEdgeI+1][0] = cellEdges[cellEdgeI][1];
        edgeLabelsEdgeIndices[2*cellEdgeI+1][1] = cellEdges[cellEdgeI][0];
        edgeLabelsEdgeIndices[2*cellEdgeI+1][2] = - cellEdgeI - 1;
    }
    sort(edgeLabelsEdgeIndices);

    // Generate a sorted list of edge labels and correspoinding face indices
    labelListList edgeLabelsFaceIndices;
    forAll(cellFaces, cellFaceI)
    {
        const face& cellFace(mesh_.faces()[cellFaces[cellFaceI]]);

        const bool owner(mesh_.faceOwner()[cellFaces[cellFaceI]] == cellI);

        const label& cellFaceNEdges(cellFace.size());
        forAll(cellFace, cellFaceEdgeI)
        {
            edgeLabelsFaceIndices.append(labelList(3, -1));
            edgeLabelsFaceIndices.last()[0] =
                cellFace[(cellFaceEdgeI + owner) % cellFaceNEdges];
            edgeLabelsFaceIndices.last()[1] =
                cellFace[(cellFaceEdgeI + !owner) % cellFaceNEdges];
            edgeLabelsFaceIndices.last()[2] = cellFaceI;
        }
    }
    sort(edgeLabelsFaceIndices);

    // Assemble lists of edge-face and cell-face connectivities
    // Negative values indicate an edge which runs in an opposite direction to
    // the face node listing
    labelListList edgeFaceIndices(cellEdges.size());
    labelListList faceEdgeIndices(cellFaces.size());
    forAll(edgeLabelsFaceIndices, I)
    {
        const label edgeLabelsEdgeIndex(edgeLabelsEdgeIndices[I][2]);
        const label absCellEdgeEdgeIndex
        (
            edgeLabelsEdgeIndex >= 0
          ? edgeLabelsEdgeIndex
          : - edgeLabelsEdgeIndex - 1
        );
        const label absCellEdgeFaceIndex(edgeLabelsFaceIndices[I][2]);
        const label edgeLabelsFaceIndex
        (
            edgeLabelsEdgeIndex >= 0
          ? absCellEdgeFaceIndex
          : - absCellEdgeFaceIndex - 1
        );

        edgeFaceIndices[absCellEdgeEdgeIndex].append(edgeLabelsFaceIndex);
        faceEdgeIndices[absCellEdgeFaceIndex].append(edgeLabelsEdgeIndex);
    }

    // Generate a list of point labels and face index pairs
    labelListList pointLabelEdgeIndexFaceIndexPairs;
    forAll(cellEdges, edgeI)
    {
        const labelPair pointIndices(cellEdges[edgeI]);

        const labelPair faceIndices
        (
            edgeFaceIndices[edgeI][0],
            edgeFaceIndices[edgeI][1]
        );
        const labelPair absFaceIndices
        (
            faceIndices[0] >= 0 ? faceIndices[0] : - faceIndices[0] - 1,
            faceIndices[1] >= 0 ? faceIndices[1] : - faceIndices[1] - 1
        );

        const bool order(faceIndices[0] > faceIndices[1]);

        pointLabelEdgeIndexFaceIndexPairs.append(labelList(4,-1));
        pointLabelEdgeIndexFaceIndexPairs.last()[0] = pointIndices[0];
        pointLabelEdgeIndexFaceIndexPairs.last()[1] = edgeI;
        pointLabelEdgeIndexFaceIndexPairs.last()[2] = absFaceIndices[order];
        pointLabelEdgeIndexFaceIndexPairs.last()[3] = absFaceIndices[!order];

        pointLabelEdgeIndexFaceIndexPairs.append(labelList(4,-1));
        pointLabelEdgeIndexFaceIndexPairs.last()[0] = pointIndices[1];
        pointLabelEdgeIndexFaceIndexPairs.last()[1] = edgeI;
        pointLabelEdgeIndexFaceIndexPairs.last()[2] = absFaceIndices[!order];
        pointLabelEdgeIndexFaceIndexPairs.last()[3] = absFaceIndices[order];
    }
    sort(pointLabelEdgeIndexFaceIndexPairs);

    // Assemble a list of point face pairs from the sorted lists
    labelListList pointEdgeIndices(cellPoints.size());
    List<List<Pair<label> > > pointFaceIndexPairs(cellPoints.size());
    {
        label I(0);
        label pointLabelOld(pointLabelEdgeIndexFaceIndexPairs[0][0]);
        forAll
        (
            pointLabelEdgeIndexFaceIndexPairs,
            pointLabelEdgeIndexFaceIndexPairI
        )
        {
            const labelList& pointLabelEdgeIndexFaceIndexPair
            (
                pointLabelEdgeIndexFaceIndexPairs
                [
                    pointLabelEdgeIndexFaceIndexPairI
                ]
            );

            if (pointLabelOld != pointLabelEdgeIndexFaceIndexPair[0])
            {
                I ++;
                pointLabelOld = pointLabelEdgeIndexFaceIndexPair[0];
            }

            const label pointI(pointLabelPointIndices[I][1]);

            pointEdgeIndices[pointI].append
            (
                pointLabelEdgeIndexFaceIndexPair[1]
            );

            pointFaceIndexPairs[pointI].append
            (
                labelPair
                (
                    pointLabelEdgeIndexFaceIndexPair[2],
                    pointLabelEdgeIndexFaceIndexPair[3]
                )
            );
        }
    }

    // Order the point face pairs and assemble a list of point face indices
    labelListList pointFaceIndices(cellPoints.size());
    forAll(pointFaceIndexPairs, pointI)
    {
        labelPairList& faceIndexPairs(pointFaceIndexPairs[pointI]);

        pointFaceIndices[pointI].append(faceIndexPairs[0][0]);

        for (label pairI = 1; pairI < faceIndexPairs.size(); pairI ++)
        {
            for (label pairJ = pairI; pairJ < faceIndexPairs.size(); ++ pairJ)
            {
                if (faceIndexPairs[pairI-1][1] == faceIndexPairs[pairJ][0])
                {
                    Swap
                    (
                        pointEdgeIndices[pointI][pairI],
                        pointEdgeIndices[pointI][pairJ]
                    );

                    Swap
                    (
                        faceIndexPairs[pairI],
                        faceIndexPairs[pairJ]
                    );

                    break;
                }
            }
            pointFaceIndices[pointI].append(faceIndexPairs[pairI][0]);
        }
    }

    // convert to global indices
    forAll(cellPoints, pointI)
    {
        labelList& edgeIndices(pointEdgeIndices[pointI]);
        labelList& faceIndices(pointFaceIndices[pointI]);

        const label nPointFaces(faceIndices.size());

        cellPointPoints_[cellI][pointI].resize(nPointFaces);

        forAll(edgeIndices, edgeI)
        {
            cellPointPoints_[cellI][pointI][edgeI] =
                cellEdges[edgeIndices[edgeI]]
                [
                    cellEdges[edgeIndices[edgeI]][0] == cellPoints[pointI]
                ];
        }

        cellPointFaces_[cellI][pointI].resize(nPointFaces);

        forAll(faceIndices, faceI)
        {
            cellPointFaces_[cellI][pointI][faceI] =
                cellFaces
                [
                    faceIndices[faceI]
                ];
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::cellPointConnectivity::cellPointConnectivity
(
    const IOobject& io,
    const polyMesh& mesh
)
:
    regIOobject(io),
    mesh_(mesh),
    cellPointPoints_(mesh.nCells()),
    cellPointFaces_(mesh.nCells())
{
    forAll(mesh.cells(), cellI)
    {
        const label nPoints(mesh.cellPoints()[cellI].size());

        cellPointPoints_[cellI].resize(nPoints);
        cellPointFaces_[cellI].resize(nPoints);

        generateCellPointConnectivity(cellI);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::cellPointConnectivity::~cellPointConnectivity()
{}


// ************************************************************************* //
