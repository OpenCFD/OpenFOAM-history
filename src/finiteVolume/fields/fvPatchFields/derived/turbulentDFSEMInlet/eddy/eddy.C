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

\*---------------------------------------------------------------------------*/

#include "eddy.H"

// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

Foam::eddy::eddy()
:
    position_(vector::zero),
    sigma_(vector::zero),
    alphaAve_(0),
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
    alphaAve_(0),
    Rpg_(tensor::zero),
    c1_(-1)
{
    // Random length scale ratio, gamma = sigmax/sigmay = sigmax/sigmaz
    // - using gamma^2 to ease lookup of c2 coefficient
    label gamma2 = rndGen.position<label>(1, 8);

    static const scalar gamma2VsC2[8] =
        {2, 1.875, 1.737, 1.75, 0.91, 0.825, 0.806, 1.5};

    // c2 coefficient retrieved up from array
    scalar c2 = gamma2VsC2[gamma2 - 1];

    // Length scale in x direction
    sigma_.x() = sigmaX;

    // Length scale in y = z; given as function of gamma
    sigma_.y() = sigma_.x()/Foam::sqrt(scalar(gamma2));
    sigma_.z() = sigma_.y();

    // Principal stresses
    vector lambda = eigenValues(R);

    // Eddy rotation from principal-to-global axes
    // - given by the 3 Eigenvectors of the Reynold stress tensor
    Rpg_ = eigenVectors(R, lambda).T();

    // Time-averaged intensity (eq. 13)
    alphaAve_ =
        sqrt
        (
            max
            (
                (lambda.y()/sqr(sigma_.y()) - 2*lambda.x()/sqr(sigma_.x()))
               /(2*c2),
                0
            )
        );

    if (0)
    {
        Pout<< "gamma2:" << gamma2 << " c2:" << c2 << " sigma:" << sigma_
            << " lambda:" << lambda << " alphaAve:" << alphaAve_ << endl;
    }
}


Foam::eddy::eddy(const eddy& e)
:
    position_(e.position_),
    sigma_(e.sigma_),
    alphaAve_(e.alphaAve_),
    Rpg_(e.Rpg_),
    c1_(e.c1_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::vector Foam::eddy::uDash(const point& xp, cachedRandom& rndGen) const
{
    vector u(vector::zero);

    // Relative position inside eddy
    vector r = cmptDivide(xp - position_, sigma_);

    scalar d2 = magSqr(r);

    if (d2 < 1)
    {
        // Intensities
        vector alpha = this->alpha(rndGen);

        for (label beta = 0; beta < 3; beta++)
        {
            label j = (beta + 1) % 3;
            label l = (j + 1) % 3;

            u[beta] += r[j]*alpha[l];
            u[beta] -= r[l]*alpha[j];
        }

        // Fluctuating velocity in principal axes system (eq. 8)
        u = cmptMultiply(sigma_*(scalar(1) - d2), u);
    }

    // Convert into eddy box system (eq. 10)
    return c1_*(Rpg_ & u);
}


// ************************************************************************* //
