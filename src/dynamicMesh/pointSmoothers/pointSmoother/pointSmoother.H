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
    Foam::pointSmoother

Description
    Abstract base class for point smoothing methods. Handles parallel
    communication via reset and average functions.

SourceFiles
    pointSmoother.C
    pointSmootherTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef pointSmoother_H
#define pointSmoother_H

#include "polyMeshGeometry.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class pointSmoother Declaration
\*---------------------------------------------------------------------------*/

class pointSmoother
{
private:

    // Private data

        //- Reference to the polyMesh
        const polyMesh& mesh_;

        //- Set of the processor patch indices
        labelHashSet processorPatchIDs_;

        //- Reference to the point displacement field
        pointVectorField& pointDisplacement_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointSmoother(const pointSmoother&);

        //- Disallow default bitwise assignment
        void operator=(const pointSmoother&);


protected:

    // Protected Member Functions

        //- Test if the given face is internal or on a processor boundary
        bool isInternalOrProcessorFace(const label faceI) const;

        //- Get a boolean list of the points to be moved
        autoPtr<PackedBoolList> pointsToMove
        (
            const labelList& facesToMove,
            const bool moveInternalFaces
        ) const;

        //- Reset the relevant weights and displacements to zero
        template <class weightType>
        void reset
        (
            const labelList& facesToMove,
            Field<weightType>& weights,
            const bool resetInternalFaces = true
        );

        //- Average the displacements using the weights provided
        template <class weightType>
        void average
        (
            const labelList& facesToMove,
            Field<weightType>& weights
        );

        //- Calculate the point displacements
        virtual void calculate
        (
            const labelList& facesToMove,
            const pointField& oldPoints,
            const pointField& currentPoints,
            polyMeshGeometry& meshGeometry
        ) = 0;


public:

    //- Runtime type information
    TypeName("pointSmoother");


    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        pointSmoother,
        dictionary,
        (const dictionary& dict, pointVectorField& pointDisplacement),
        (dict, pointDisplacement)
    );


    // Constructors

        //- Construct from a dictionary and a point displacement field
        pointSmoother(const dictionary&, pointVectorField&);


    // Selector

        static autoPtr<pointSmoother> New
        (
            const dictionary& dict,
            pointVectorField& pointDisplacement
        );


    //- Destructor
    virtual ~pointSmoother();


    // Member Functions

        //- Access

            // Access the mesh
            const polyMesh& mesh() const
            {
                return mesh_;
            }

            // Constant access to the point displacements
            const pointVectorField& pointDisplacement() const
            {
                return pointDisplacement_;
            }

            // Non-constant access to the point displacements
            pointVectorField& pointDisplacement()
            {
                return pointDisplacement_;
            }


        //- Update the point displacements
        void update
        (
            const labelList& facesToMove,
            const pointField& oldPoints,
            const pointField& currentPoints,
            polyMeshGeometry& meshGeometry
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "pointSmootherTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
