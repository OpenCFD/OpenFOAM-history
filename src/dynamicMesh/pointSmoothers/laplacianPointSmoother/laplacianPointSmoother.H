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
    Foam::laplacianPointSmoother

Description
    Laplacian point smoothing. Points are moved towards the average of the
    surrounding cell centres.

SourceFiles
    laplacianPointSmoother.C

\*---------------------------------------------------------------------------*/

#ifndef laplacianPointSmoother_H
#define laplacianPointSmoother_H

#include "pointSmoother.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace pointSmoothers
{

/*---------------------------------------------------------------------------*\
                      Class laplacianPointSmoother Declaration
\*---------------------------------------------------------------------------*/

class laplacianPointSmoother
:
    public pointSmoother
{
public:

    //- Runtime type information
    TypeName("laplacian");


    // Constructors

        //- Construct from a dictionary and a polyMesh
        laplacianPointSmoother
        (
            const dictionary& dict,
            pointVectorField& pointDisplacement
        );


    //- Destructor
    virtual ~laplacianPointSmoother();


    // Member Functions

        //- Calculate the end point locations
        virtual void update
        (
            const labelList& facesToMove,
            const pointField& oldPoints,
            const pointField& currentPoints,
            polyMeshGeometry& meshGeometry
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace pointSmoothers
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
