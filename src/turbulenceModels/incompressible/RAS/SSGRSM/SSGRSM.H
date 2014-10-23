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

Class
    Foam::incompressible::RASModels::SSGRSM

Group
    grpIcoRASTurbulence

Description
    SSG Reynolds stress turbulence model for incompressible flows based
    on:
    \verbatim
        Speziale, C. Sarkar, S. and Gatski, t.
        ICASE
        Modeling the pressure-strain correlation of turbulence - An
        invariant dynamical system approach

        Contract number NAS1~18605. 1990
    \endverbatim


    The default model coefficients correspond to the following:
    \verbatim
        SSGRSMCoeffs
        {
            Cmu             0.09;
            C1              3.4;
            C1start         1.8;
            C2              4.2;
            C3              0.8;
            C3start         1.3;
            C4              1.25;
            C5              0.4;
            Ceps1           1.44;
            Ceps2           1.83;
            sigmaEps        1.3;
            sigmaR          0.81967;
            couplingFactor  0.0;
        }
    \endverbatim

SourceFiles
    SSGRSM.C

\*---------------------------------------------------------------------------*/

#ifndef SSGRSM_H
#define SSGRSM_H

#include "RASModel.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{
namespace RASModels
{

/*---------------------------------------------------------------------------*\
                           Class SSGRSM Declaration
\*---------------------------------------------------------------------------*/

class SSGRSM
:
    public RASModel
{

protected:

    // Protected data

        // Model coefficients

            dimensionedScalar Cmu_;

            dimensionedScalar C1_;
            dimensionedScalar C1Start_;

            dimensionedScalar C2_;
            dimensionedScalar C3_;
            dimensionedScalar C3Start_;
            dimensionedScalar C4_;
            dimensionedScalar C5_;
            dimensionedScalar Ceps1_;
            dimensionedScalar Ceps2_;
            dimensionedScalar sigmaR_;
            dimensionedScalar sigmaEps_;

            dimensionedScalar couplingFactor_;


        // Fields

            volScalarField k_;
            volScalarField epsilon_;
            volScalarField nut_;
            volSymmTensorField R_;
            volSymmTensorField b_;


public:

    //- Runtime type information
    TypeName("SSGRSM");

    // Constructors

        //- Construct from components
        SSGRSM
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            transportModel& transport,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~SSGRSM()
    {}


    // Member Functions

        //- Return the turbulence viscosity
        virtual tmp<volScalarField> nut() const
        {
            return nut_;
        }

        //- Return the effective diffusivity for R
        tmp<volScalarField> DREff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("DREff", nut_/sigmaR_ + nu())
            );
        }

        //- Return the effective diffusivity for epsilon
        tmp<volScalarField> DepsilonEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("DepsilonEff", nut_/sigmaEps_ + nu())
            );
        }

        //- Return the turbulence kinetic energy
        virtual tmp<volScalarField> k() const
        {
            return k_;
        }

        //- Return the turbulence kinetic energy dissipation rate
        virtual tmp<volScalarField> epsilon() const
        {
            return epsilon_;
        }

        //- Return the Reynolds stress tensor
        virtual tmp<volSymmTensorField> R() const
        {
            return R_;
        }

        //- Return the effective stress tensor including the laminar stress
        virtual tmp<volSymmTensorField> devReff() const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevReff(volVectorField& U) const;

        //- Return the source term for the momentum equation
        virtual tmp<fvVectorMatrix> divDevRhoReff
        (
            const volScalarField& rho,
            volVectorField& U
        ) const;

        //- Solve the turbulence equations and correct the turbulence viscosity
        virtual void correct();

        //- Read RASProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RASModels
} // End namespace incompressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
