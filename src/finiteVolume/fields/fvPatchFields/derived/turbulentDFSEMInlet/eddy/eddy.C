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

#include "eddy.H"

// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

Foam::eddy::eddy()
:
    position_(vector::zero),
    sigma_(vector::zero),
    sigmaXMax_(-1),
    alpha_(vector::zero),
    Rpg_(tensor::zero),
    c1_(-1)
{}


Foam::eddy::eddy
(
    const point& position,
    const scalar sigmaX,
    const tensor& R,
    const vector& n,
    cachedRandom& rndGen
)
:
    position_(position),
    sigma_(vector::zero),
    sigmaXMax_(-1),
    alpha_(vector::zero),
    Rpg_(tensor::zero),
    c1_(-1)
{
    // random length scale ratio, gamma = sigmax/sigmay = sigmax/sigmaz
    // - using gamma^2 to ease lookup of c2 coefficient
    label gamma2 = rndGen.position<label>(1, 8);

    static const scalar gamma2VsC2[8] =
        {2, 1.875, 1.737, 1.75, 0.91, 0.825, 0.806, 1.5};

    // c2 coefficient looked up from array
    scalar c2 = gamma2VsC2[gamma2 - 1];

    // length scale in x direction
    sigma_.x() = sigmaX;

    // length scale in y = z; given as function of gamma
    sigma_.y() = sigma_.x()/Foam::sqrt(scalar(gamma2));
    sigma_.z() = sigma_.y();

    // principal stresses
    vector lambda = eigenValues(R);

    // eddy rotation from principal-to-global axes
    // - given by the 3 eigen vectors of the Reynold stress tensor
    Rpg_ = eigenVectors(R, lambda).T();

    // distance when eddy passes out of eddy box in local principal axes system
    sigmaXMax_ = n & (sigma_ & Rpg_);

    // intensity (eq. 13)
    scalar alphaAve =
        sqrt
        (
            max
            (
                (lambda.y()/sqr(sigma_.y()) - 2*lambda.x()/sqr(sigma_.x()))
               /(2*c2),
                0
            )
        );

    alpha_.x() = eps(rndGen)*alphaAve;
    alpha_.y() = eps(rndGen)*alphaAve;
    alpha_.z() = eps(rndGen)*alphaAve;

    if (0)
    {
        Pout<< "gamma2:" << gamma2 << " c2:" << c2 << " sigma:" << sigma_
            << " lambda:" << lambda << " sigmaXMax:" << sigmaXMax_
            << " alpha:" << alpha_ << endl;
    }
}


Foam::eddy::eddy(const eddy& e)
:
    position_(e.position_),
    sigma_(e.sigma_),
    sigmaXMax_(e.sigmaXMax_),
    alpha_(e.alpha_),
    Rpg_(e.Rpg_),
    c1_(e.c1_)
{}


// ************************************************************************* //
