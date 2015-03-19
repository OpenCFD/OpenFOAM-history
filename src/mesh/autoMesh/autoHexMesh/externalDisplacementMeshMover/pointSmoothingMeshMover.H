/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenFOAM Foundation
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

Class
    Foam::pointSmoothingMeshMover

Description
    Quality-based under-relaxation for run-time selectable point smoothing.

SourceFiles
    pointSmoothingMeshMover.C

\*---------------------------------------------------------------------------*/

#ifndef pointSmoothingMeshMover_H
#define pointSmoothingMeshMover_H

#include "externalDisplacementMeshMover.H"
#include "pointSmoother.H"
#include "polyMeshGeometry.H"
#include "motionSmootherAlgo.H"
#include "fieldSmoother.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class pointSmoothingMeshMover Declaration
\*---------------------------------------------------------------------------*/

class pointSmoothingMeshMover
:
    public externalDisplacementMeshMover
{
    // Private Data

        //- Part-updatable mesh geometry
        polyMeshGeometry meshGeometry_;

        //- Point smoothing method
        autoPtr<pointSmoother> pointSmoother_;

        //- IDs of fixedValue patches that we can modify
        const labelList adaptPatchIDs_;

        //- Combined indirect fixedValue patches that we can modify
        autoPtr<indirectPrimitivePatch> adaptPatchPtr_;

        //- Scale factor for displacement
        pointScalarField scale_;

        //- Old point field
        pointField oldPoints_;

        //- Mesh mover algorithm
        motionSmootherAlgo meshMover_;

        //- Field smoothing
        fieldSmoother fieldSmoother_;


    // Private Member Functions

        //- Apply the mesh mover algorithm
        bool moveMesh
        (
            const dictionary& moveDict,
            const label nAllowableErrors,
            labelList& checkFaces
        );


public:

    //- Runtime type information
    TypeName("displacementPointSmoothing");


    // Constructors

        //- Construct from a polyMesh and an IOdictionary
        pointSmoothingMeshMover
        (
            const dictionary& dict,
            const List<labelPair>& baffles,
            pointVectorField& pointDisplacement
        );


    //- Destructor
    virtual ~pointSmoothingMeshMover();


    // Member Functions

        //- Move mesh using current pointDisplacement boundary values.
        //  Return true if succesful (errors on checkFaces less than
        //  allowable). Updates pointDisplacement.
        virtual bool move
        (
            const dictionary&,
            const label nAllowableErrors,
            labelList& checkFaces
        );

        //- Update local data for geometry changes
        virtual void movePoints(const pointField&);

        //-  Update local data for topology changes
        virtual void updateMesh(const mapPolyMesh&)
        {
            notImplemented
            (
                "medialAxisMeshMover::updateMesh"
                "(const mapPolyMesh&)"
            );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
