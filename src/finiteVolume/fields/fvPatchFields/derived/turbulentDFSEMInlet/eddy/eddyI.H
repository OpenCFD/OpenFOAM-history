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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline Foam::label Foam::eddy::patchFaceI() const
{
    return patchFaceI_;
}


inline const Foam::point& Foam::eddy::position() const
{
    return position_;
}


inline const Foam::vector& Foam::eddy::sigma() const
{
    return sigma_;
}


inline Foam::scalar Foam::eddy::alphaAve() const
{
    return alphaAve_;
}


inline Foam::vector Foam::eddy::alpha(cachedRandom& rndGen) const
{
    // Random vectors of 1's and -1's
    vector a(eps(rndGen), eps(rndGen), eps(rndGen));

    return a/mag(a)*alphaAve_;
}


inline const Foam::tensor& Foam::eddy::Rpg() const
{
    return Rpg_;
}


inline Foam::scalar Foam::eddy::c1() const
{
    if (c1_ < 0)
    {
        FatalErrorIn("inline Foam::scalar Foam::eddy::c1() const")
            << "Eddy not initialised"
            << abort(FatalError);
    }

    return c1_;
}


inline void Foam::eddy::initialise(const label nEddy, const scalar v0)
{
    // Normalisation coefficient (eq. 11)
    c1_ =
        Foam::sqrt(10*v0)*sigmaAve()
       /(Foam::sqrt(scalar(nEddy))*boxVolume())
       *cmptMin(sigma_);
}


inline Foam::scalar Foam::eddy::boxVolume() const
{
    return cmptProduct(sigma_);
}


inline Foam::scalar Foam::eddy::sigmaAve() const
{
    return cmptAv(sigma_);
}


Foam::scalar Foam::eddy::eps(cachedRandom& rndGen) const
{
    // Random number with zero mean and unit variance
    scalar e = rndGen.sample01<scalar>() > 0.5 ? 1 : -1;

    return e;
}


void Foam::eddy::move(const scalar dx)
{
    position_.x() += dx;
}


// ************************************************************************* //
