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
    Foam::solarCalculator

Description


SourceFiles
    solarCalculator.C

\*---------------------------------------------------------------------------*/

#ifndef solarCalculator_H
#define solarCalculator_H

#include "fvMesh.H"
#include "meshTools.H"
#include "DynamicField.H"
#include "HashSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class solarCalculator Declaration
\*---------------------------------------------------------------------------*/

class solarCalculator
{
public:

    //  Public enumeration

        //- Sun direction models
        enum sunDirModel
        {
            mSunDirConstant,
            mSunDirTraking
        };

        //- Direct sun load models
        enum sunLModel
        {
            mSunLoadConstant,
            mSunLoadFairWeatherConditions,
            mSunLoadTheoreticalMaximum
        };


protected:

         //- Sun direction models
        static const NamedEnum<sunDirModel, 2> sunDirectionModelTypeNames_;

        //- Sun load models
        static const NamedEnum<sunLModel, 3> sunLoadModelTypeNames_;

private:

    // Private data




        //- Reference to mesh
        const fvMesh& mesh_;

        //- Dictionary
        dictionary dict_;

        //- Direction
        vector direction_;

        //- Direct solar irradiation
        scalar directSolarRad_;

        //- Diffuse solar irradiation on vertical surfaces
        scalar diffuseSolarRad_;

        //- Ground reflectivity
        scalar groundReflectivity_;

        //- Fair weather direct solar load model parameters
        scalar A_;
        scalar B_;
        scalar beta_;


        //- Maximum theoretical direct solar load model parameters
        scalar Setrn_;
        scalar SunPrime_;


        //- Diffusive solar load model parameters
        scalar C_;
        scalar Y_;

        //- Sun direction model
        sunDirModel sunDirectionModel_;

        //- Sun load model
        sunLModel sunLoadModel_;


        //- Disallow default bitwise copy construct
        solarCalculator(const solarCalculator&);

        //- Disallow default bitwise assignment
        void operator=(const solarCalculator&);


    // Private members

        //- Init
        void init();


public:

    // Declare name of the class and its debug switch
    ClassName("solarCalculator");


    // Constructors

        //- Construct from dictionary
        solarCalculator(const dictionary&, const fvMesh&);


    //- Destructor
    ~solarCalculator();


    // Member Functions

        // Access

            //- const acess to direction
            const vector direction() const
            {
                return direction_;
            }

            //- Non-const access to direction
            vector direction()
            {
                return direction_;
            }

             //- Return direct solar irradiation
            scalar directSolarRad()
            {
                return directSolarRad_;
            }

            //- Return diffuse solar irradiation
            scalar diffuseSolarRad()
            {
                return diffuseSolarRad_;
            }

            //- Return C consntant
            scalar C()
            {
                return C_;
            }

            //- Return Y consntat
            scalar Y()
            {
                return Y_;
            }

            //- Return beta consntat
            scalar beta()
            {
                return beta_;
            }

            //- Return Sun direction model
            sunDirModel sunDirectionModel() const
            {
                return sunDirectionModel_;
            }

            //- Return Sun load model
            sunLModel sunLoadModel() const
            {
                return sunLoadModel_;
            }

             //- Return groundReflectivity
            scalar groundReflectivity()
            {
                return groundReflectivity_;
            }


        //- Recalculate
        void correctSunDirection();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
