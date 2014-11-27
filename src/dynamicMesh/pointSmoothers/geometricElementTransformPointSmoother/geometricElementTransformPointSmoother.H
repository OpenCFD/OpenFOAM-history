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

Class
    Foam::pointSmoothers::geometricElementTransformPointSmoother

Description
    Geometric Element Transformation Method (GETMe) point smoother. Points are
    moved in a direction normal to the face of the dual element. This tends to
    orthogonalise elements. This method can "push" as well as "pull".

SourceFiles
    geometricElementTransformPointSmoother.C

\*---------------------------------------------------------------------------*/

#ifndef geometricElementTransformPointSmoother_H
#define geometricElementTransformPointSmoother_H

#include "pointSmoother.H"
#include "cellPointConnectivity.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace pointSmoothers
{

/*---------------------------------------------------------------------------*\
             Class geometricElementTransformPointSmoother Declaration
\*---------------------------------------------------------------------------*/

class geometricElementTransformPointSmoother
:
    public pointSmoother
{
private:

    // Private Data

        //- Cell-point connectivity
        const cellPointConnectivity& cellPointConnectivity_;

        //- Transformation parameter
        const scalar transformationParameter_;


public:

    //- Runtime type information
    TypeName("geometricElementTransform");


    // Constructors

        //- Construct from a dictionary and a mesh
        geometricElementTransformPointSmoother
        (
            const dictionary& dict,
            const polyMesh& mesh
        );


    //- Destructor
    virtual ~geometricElementTransformPointSmoother();


    // Member Functions

        //- Calculate the end point locations
        virtual void update
        (
            const labelList& facesToMove,
            const polyMeshGeometry& meshGeometry,
            const pointField& oldPoints,
            const pointField& currentPoints,
            vectorField& displacements
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace pointSmoothers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
