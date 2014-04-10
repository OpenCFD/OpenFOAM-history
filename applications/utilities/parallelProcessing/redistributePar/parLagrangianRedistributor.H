/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenFOAM Foundation
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
    Foam::parLagrangianRedistributor

Description
    Lagrangian field redistributor.

    Runs in parallel. Redistributes from procMesh to baseMesh.

SourceFiles
    parLagrangianRedistributor.C
    parLagrangianRedistributorRedistributeFields.C

\*---------------------------------------------------------------------------*/

#ifndef parLagrangianRedistributor_H
#define parLagrangianRedistributor_H

#include "PtrList.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class mapDistributePolyMesh;
class mapDistributeBase;
class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class parLagrangianRedistributor Declaration
\*---------------------------------------------------------------------------*/

class parLagrangianRedistributor
{
    // Private data

        //- Destination mesh reference
        const fvMesh& baseMesh_;

        //- Processor mesh reference
        const fvMesh& procMesh_;

        //- Distribution map reference
        const mapDistributePolyMesh& distMap_;

        //- For every cell the destination processor
        labelList destinationProcID_;

        //- For every cell the destination cell
        labelList destinationCell_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        parLagrangianRedistributor(const parLagrangianRedistributor&);

        //- Disallow default bitwise assignment
        void operator=(const parLagrangianRedistributor&);


public:

    // Constructors

        //- Construct from components
        parLagrangianRedistributor
        (
            const fvMesh& baseMesh,
            const fvMesh& procMesh,
            const mapDistributePolyMesh& distMap
        );


    // Member Functions

        //- Find all clouds (on all processors) and for each cloud all
        //  the objects. Result will be synchronised on all processors
        static void findClouds
        (
            const fvMesh&,
            wordList& cloudNames,
            List<wordList>& objectNames
        );

        //- Read, redistribute and write lagrangian positions
        autoPtr<mapDistributeBase> redistributeLagrangianPositions
        (
            const word& cloudName
        ) const;

        //- Pick up any fields of a given type
        template<class Type>
        static wordList filterObjects
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );

        //- Read, redistribute and write all/selected lagrangian fields
        template<class Type>
        void redistributeLagrangianFields
        (
            const mapDistributeBase& map,
            const word& cloudName,
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;

        //- Read, redistribute and write all/selected lagrangian fieldFields
        template<class Type>
        void redistributeLagrangianFieldFields
        (
            const mapDistributeBase& map,
            const word& cloudName,
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "parLagrangianRedistributorRedistributeFields.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
