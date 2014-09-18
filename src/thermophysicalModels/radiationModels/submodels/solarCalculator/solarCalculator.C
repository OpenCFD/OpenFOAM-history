/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd
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

#include "solarCalculator.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(solarCalculator, 0);

    template<>
    const char* NamedEnum
    <
        solarCalculator::sunDirModel,
        2
    >::names[] =
    {
        "sunDirConstant",
        "sunDirTraking"
    };

    template<>
    const char* NamedEnum
    <
        solarCalculator::sunLModel,
        3
    >::names[] =
    {
        "sunLoadConstant",
        "sunLoadFairWeatherConditions",
        "sunLoadTheoreticalMaximum"
    };
}

const Foam::NamedEnum<Foam::solarCalculator::sunDirModel, 2>
  Foam::solarCalculator::sunDirectionModelTypeNames_;

const Foam::NamedEnum<Foam::solarCalculator::sunLModel, 3>
   Foam::solarCalculator::sunLoadModelTypeNames_;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


void Foam::solarCalculator::init()
{
    switch (sunDirectionModel_)
    {
        case mSunDirConstant:
        {
            dict_.lookup("sunDirection") >> direction_;
            direction_ /= mag(direction_);
            break;
        }
        case mSunDirTraking:
        {
            break;
        }
    }

    switch (sunLoadModel_)
    {
        case mSunLoadConstant:
        {
            dict_.lookup("directSolarRad") >> directSolarRad_;
            dict_.lookup("diffuseSolarRad") >> diffuseSolarRad_;
            break;
        }
        case mSunLoadFairWeatherConditions:
        {
            A_ = readScalar(dict_.lookup("A"));
            B_ = readScalar(dict_.lookup("B"));
            beta_ = readScalar(dict_.lookup("beta"));
            directSolarRad_ = A_/exp(B_/sin(beta_));
            // diffusive radiation on vertical walls
            diffuseSolarRad_ = directSolarRad_*C_*Y_;
            groundReflectivity_ =
                readScalar(dict_.lookup("groundReflectivity"));
            break;
        }
        case mSunLoadTheoreticalMaximum:
        {
            Setrn_ = readScalar(dict_.lookup("Setrn"));
            SunPrime_ = readScalar(dict_.lookup("SunPrime"));
            directSolarRad_ = Setrn_*SunPrime_;
            // diffusive radiation on vertical walls
            diffuseSolarRad_ = directSolarRad_*C_*Y_;
            groundReflectivity_ =
                readScalar(dict_.lookup("groundReflectivity"));
            break;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::solarCalculator::solarCalculator
(
    const dictionary& dict,
    const fvMesh& mesh
)
:
    mesh_(mesh),
    dict_(dict),
    direction_(vector::zero),
    directSolarRad_(0.0),
    diffuseSolarRad_(0.0),
    groundReflectivity_(0.0),
    A_(0.0),
    B_(0.0),
    beta_(0.0),
    Setrn_(0.0),
    SunPrime_(0.0),
    C_(readScalar(dict.lookup("C"))),
    Y_(readScalar(dict.lookup("Y"))),
    sunDirectionModel_
    (
        sunDirectionModelTypeNames_.read(dict.lookup("sunDirectionModel"))
    ),
    sunLoadModel_
    (
        sunLoadModelTypeNames_.read(dict.lookup("sunLoadModel"))
    )
{
    init();
}



// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::solarCalculator::~solarCalculator()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::solarCalculator::correctSunDirection()
{
    switch (sunDirectionModel_)
    {
        case mSunDirConstant:
        {
            break;
        }
        case mSunDirTraking:
        {
            // To be implemented
            break;
        }
    }
}


// ************************************************************************* //
