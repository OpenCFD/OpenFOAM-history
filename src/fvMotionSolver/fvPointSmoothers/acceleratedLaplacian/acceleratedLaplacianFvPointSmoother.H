/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014-2015 OpenCFD Ltd.
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
    Foam::acceleratedLaplacianFvPointSmoother

Description
    Accelerated laplacian point smoothing. Points are moved towards the average
    of the surrounding cell centres. A Finite-volume solution is used as an
    initial guess to accelerate the solution.

SourceFiles
    acceleratedLaplacianFvPointSmoother.C

\*---------------------------------------------------------------------------*/

#ifndef acceleratedLaplacianFvPointSmoother_H
#define acceleratedLaplacianFvPointSmoother_H

#include "laplacianPointSmoother.H"
#include "fvMotionSolverCore.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace pointSmoothers
{

/*---------------------------------------------------------------------------*\
                      Class acceleratedLaplacianFvPointSmoother Declaration
\*---------------------------------------------------------------------------*/

class acceleratedLaplacianFvPointSmoother
:
    public laplacianPointSmoother,
    public fvMotionSolverCore
{
private:

    // Private data

        //- Cell-centre motion field
        volVectorField cellDisplacement_;


public:

    //- Runtime type information
    TypeName("acceleratedLaplacian");


    // Constructors

        //- Construct from a dictionary and a polyMesh
        acceleratedLaplacianFvPointSmoother
        (
            const dictionary& dict,
            pointVectorField& pointDisplacement
        );


    //- Destructor
    virtual ~acceleratedLaplacianFvPointSmoother();


    // Member Functions

        // Access

            // Constant access to the cell displacements
            const volVectorField& cellDisplacement() const
            {
                return cellDisplacement_;
            }

            // Non-constant access to the cell displacements
            volVectorField& cellDisplacement()
            {
                return cellDisplacement_;
            }


        //- Calculate the point displacements
        virtual void calculate
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
