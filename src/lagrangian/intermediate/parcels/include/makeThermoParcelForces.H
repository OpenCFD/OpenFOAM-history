/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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

#ifndef makeThermoParcelForces_H
#define makeThermoParcelForces_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "BrownianMotionForce.H"
#include "GravityForce.H"
#include "NonInertialFrameForce.H"
#include "NonSphereDragForce.H"
#include "ParamagneticForce.H"
#include "PressureGradientForce.H"
#include "SaffmanMeiLiftForce.H"
#include "SphereDragForce.H"
#include "SRFForce.H"
#include "TomiyamaLiftForce.H"
#include "VirtualMassForce.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeThermoParcelForces(CloudType)                                     \
                                                                              \
    makeParticleForceModel(CloudType);                                        \
    makeParticleForceModelType(BrownianMotionForce, CloudType);               \
    makeParticleForceModelType(GravityForce, CloudType);                      \
    makeParticleForceModelType(NonInertialFrameForce, CloudType);             \
    makeParticleForceModelType(NonSphereDragForce, CloudType);                \
    makeParticleForceModelType(ParamagneticForce, CloudType);                 \
    makeParticleForceModelType(PressureGradientForce, CloudType);             \
    makeParticleForceModelType(SaffmanMeiLiftForce, CloudType);               \
    makeParticleForceModelType(SphereDragForce, CloudType);                   \
    makeParticleForceModelType(SRFForce, CloudType);                          \
    makeParticleForceModelType(TomiyamaLiftForce, CloudType);                 \
    makeParticleForceModelType(VirtualMassForce, CloudType);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
