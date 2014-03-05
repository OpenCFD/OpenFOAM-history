/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2014 OpenFOAM Foundation
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
    Foam::fv::enthalpyPorositySource

Description
    This source is designed to model the effect of solidification and melting
    processes, e.g. windhield defrosting.  The phase change occurs between the
    limiting temperatures of Tliquidus and Tsolidus.

    The presence of the solid phase in the flow field is incorporated into the
    model as a momentum porosity contribution; the energy associated with the
    phase change is added as an enthalpy contribution.

    Based on the references:

      1. V.R. Voller and C. Prakash, A fixed grid numerical modelling
         methodology for convection-diffusion mushy phase-change problems,
         Int. J. Heat Mass Transfer 30(8):17091719, 1987.
      2. C.R. Swaminathan. and V.R. Voller, A general enthalpy model for
         modeling solidification processes, Metallurgical Transactions
         23B:651664, 1992.


    \heading Source usage
    Example usage:
    \verbatim
    enthalpyPorositySourceCoeffs
    {
        type            enthalpyPorositySource;
        active          on;
        selectionMode   cellZone;
        cellZone        iceZone;

        enthalpyPorositySourceCoeffs
        {
            Tliquidus       288;
            Tsolidus        268;
            L               334000;
            thermoMode      thermo;
            beta            50e-6;

            // only for incompressible solvers:
            // rhoName         rhoRef;
            // rhoRef          1;

            // only for solvers that do not define gravity:
            g               (0 -9.81 0);
        }
    }
    \endverbatim

    Where:
    \table
        Property     | Description             | Required    | Default value
        Tliquidus    | Temperature when liquid [K] | yes     |
        Tsolidus     | Temperature when solid [K]  | yes     |
        L            | Latent heat of fusion [J/kg] | yes    |
        thermoMode   | Thermo mode [thermo|lookup] | yes     |
        beta         | Thermal expansion coefficient [1/K] | yes |
        rhoName      | Name of density field   | no          | rho
        rhoRef       | Reference density       | no          |
        g            | Accelerartion due to gravity | no     |
    \endtable

SourceFiles
    enthalpyPorositySource.C
    enthalpyPorositySourceIO.C

\*---------------------------------------------------------------------------*/

#ifndef enthalpyPorositySource_H
#define enthalpyPorositySource_H

#include "fvMesh.H"
#include "volFields.H"
#include "fvOption.H"
#include "NamedEnum.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                   Class enthalpyPorositySource Declaration
\*---------------------------------------------------------------------------*/

class enthalpyPorositySource
:
    public option
{
public:

    enum thermoMode
    {
        mdThermo,
        mdLookup
    };

    static const NamedEnum<thermoMode, 2> thermoModeTypeNames_;


private:

    // Private data

        //- Solidification initiated when T > Tliquidus_ [K]
        scalar Tliquidus_;

        //- Solidification temperature [K]
        scalar Tsolidus_;

        //- Latent heat of fusion [J/kg]
        scalar L_;

        //- Phase fraction under-relaxation coefficient
        scalar relax_;

        //- Thermodynamics mode
        thermoMode mode_;

        //- Name of density field - default = "rho" (optional)
        word rhoName_;

        //- Name of temperature field - default = "T" (optional)
        word TName_;

        //- Name of velocity field - default = "U" (optional)
        word UName_;

        //- Mushy region momentum sink coefficient [1/s]; default = 10^5
        scalar Cu_;

        //- Coefficient used in porosity calc - default = 0.001
        scalar q_;

        //- Thermal expansion coefficient [1/K]
        scalar beta_;

        //- Phase fraction indicator field
        volScalarField alpha1_;

        //- Change in phase fraction indicator field
        scalarField dAlpha1_;

        //- Current time index (used for updating)
        label curTimeIndex_;


    // Private Member Functions

        //- Return the list of alpha1 boundary types
        wordList alpha1BoundaryTypes() const;

        //- Flag to indicate whether to solve for given field
        bool solveField(const word& fieldName) const;

        //- Return the density field
        tmp<volScalarField> rho() const;

        //- Return the gravity vector
        vector g() const;

        //- Update the model
        void update();

        //- Disallow default bitwise copy construct
        enthalpyPorositySource(const enthalpyPorositySource&);

        //- Disallow default bitwise assignment
        void operator=(const enthalpyPorositySource&);


public:

    //- Runtime type information
    TypeName("enthalpyPorositySource");


    // Constructors

        //- Construct from explicit source name and mesh
        enthalpyPorositySource
        (
            const word& sourceName,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    // Member Functions

        //- Flag to bypass the apply flag list checking
        virtual bool alwaysApply() const;


        // Evaluate

            //- Add explicit contribution to enthalpy equation
            virtual void addSup(fvMatrix<scalar>& eqn, const label fieldI);

            //- Add implicit contribution to momentum equation
            virtual void addSup(fvMatrix<vector>& eqn, const label fieldI);


        // I-O

            //- Write the source properties
            virtual void writeData(Ostream&) const;

            //- Read source dictionary
            virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
