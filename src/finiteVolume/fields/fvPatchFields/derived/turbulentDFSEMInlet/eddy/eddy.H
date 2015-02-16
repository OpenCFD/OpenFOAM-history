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
    Foam::eddy

Description
    Class to describe eddies for the turbulentDFSEMInletFvPatchVectorField
    boundary condition.

SourceFiles
    eddy.C

\*---------------------------------------------------------------------------*/

#ifndef turbulentDFSEMInletFvPatchVectorField_eddy_H
#define turbulentDFSEMInletFvPatchVectorField_eddy_H

#include "vector.H"
#include "point.H"
#include "tensor.H"
#include "cachedRandom.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                            Class eddy Declaration
\*---------------------------------------------------------------------------*/

class eddy
{
    // Private data

        //- Position
        point position_;

        //- Length scales in 3-D space
        vector sigma_;

        //- Time-averaged intensity
        scalar alphaAve_;

        //- Co-ordinate system transformation from local principal to global
        //  axes
        tensor Rpg_;

        //- Model coefficient
        scalar c1_;


public:

    // Constructors

        //- Construct null
        eddy();

        //- Construct from Istream
        eddy(Istream& is);

        //- Construct from components
        eddy
        (
            const point& position,      // position
            const scalar sigmaX,        // length scale
            const tensor& R,            // Stress tensor
            const vector& n,            // Streamwise direction (normalised)
            cachedRandom& rndGen
        );

        //- Construct copy
        eddy(const eddy& e);



    // Public Member Functions

        // Access

            //- Return the position
            inline const point& position() const;

            //- Return the lLength scales in 3-D space
            inline const vector& sigma() const;

            //- Return the time-averaged intensity
            inline scalar alphaAve() const;

            //- Return the intensities in 3-D space
            inline vector alpha(cachedRandom& rndGen) const;

            //- Return the co-ordinate system transformation from local
            //  principal to global axes
            inline const tensor& Rpg() const;

            //- Return the model coefficient
            inline scalar c1() const;


        // Helper functions

            inline void initialise(const label nEddy, const scalar v0);

            //- Box volume
            inline scalar boxVolume() const;

            //- Average length scale
            inline scalar sigmaAve() const;

            //- Return a random number with zero mean and unit variance
            inline scalar eps(cachedRandom& rndGen) const;

            //- Move the eddy
            inline void move(const scalar dx);


        // Evaluate

            //- Return the fluctuating velocity contribution at local point xp
            vector uDash(const point& xp, cachedRandom& rndGen) const;


    // Member Operators

        void operator=(const eddy& e);


    // Friend Operators

        friend bool operator==(const eddy& a, const eddy& b)
        {
            return
                a.position_ == b.position_
             && a.sigma_ == b.sigma_
             && a.alphaAve_ == b.alphaAve_
             && a.Rpg_ == b.Rpg_
             && a.c1_ == b.c1_;
        }

        friend bool operator!=(const eddy& a, const eddy& b)
        {
            return !(a == b);
        }


    // IOstream Operators

        friend Istream& operator>>(Istream& is, eddy& e);
        friend Ostream& operator<<(Ostream& os, const eddy& e);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "eddyI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
