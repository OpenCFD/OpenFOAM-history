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
    Foam::parFvFieldReconstructor

Description
    Finite volume reconstructor for volume and surface fields.

    Runs in parallel. Reconstructs from procMesh to baseMesh. baseMesh
    is non-zero cells on processor0 only.

SourceFiles
    parFvFieldReconstructor.C
    parFvFieldReconstructorReconstructFields.C

\*---------------------------------------------------------------------------*/

#ifndef parFvFieldReconstructor_H
#define parFvFieldReconstructor_H

#include "PtrList.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class mapDistributePolyMesh;
class mapDistributeBase;
class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class parFvFieldReconstructor Declaration
\*---------------------------------------------------------------------------*/

class parFvFieldReconstructor
{
    // Private data

        //- Reconstructed mesh reference
        const fvMesh& baseMesh_;

        //- Processor mesh reference
        const fvMesh& procMesh_;

        //- Distribution map reference
        const mapDistributePolyMesh& distMap_;

        //- Patch mappers
        PtrList<mapDistributeBase> patchFaceMaps_;

        //- Number of fields reconstructed
        label nReconstructed_;


    // Private Member Functions

        //- Construct per-patch addressing
        void createPatchFaceMaps();

        //- Disallow default bitwise copy construct
        parFvFieldReconstructor(const parFvFieldReconstructor&);

        //- Disallow default bitwise assignment
        void operator=(const parFvFieldReconstructor&);


public:

//        //- Mapper for sizing only - does not do any actual mapping.
//        class fvPatchFieldReconstructor
//        :
//            public fvPatchFieldMapper
//        {
//            label size_;
//
//        public:
//
//            // Constructors
//
//                //- Construct given size
//                fvPatchFieldReconstructor(const label size)
//                :
//                    size_(size)
//                {}
//
//
//            // Member functions
//
//                label size() const
//                {
//                    return size_;
//                }
//
//                bool direct() const
//                {
//                    return true;
//                }
//
//                bool hasUnmapped() const
//                {
//                    return false;
//                }
//
//                const labelUList& directAddressing() const
//                {
//                    return labelUList::null();
//                }
//        };


    // Constructors

        //- Construct from components
        parFvFieldReconstructor
        (
            const fvMesh& baseMesh,
            const fvMesh& procMesh,
            const mapDistributePolyMesh& distMap
        );


    // Member Functions

        //- Reconstruct volume internal field
        template<class Type>
        tmp<DimensionedField<Type, volMesh> >
        reconstructFvVolumeInternalField
        (
            const DimensionedField<Type, volMesh>&
        ) const;

        //- Read and reconstruct volume internal field
        template<class Type>
        tmp<DimensionedField<Type, volMesh> >
        reconstructFvVolumeInternalField(const IOobject& fieldIoObject) const;



        //- Reconstruct volume field
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> >
        reconstructFvVolumeField
        (
            const GeometricField<Type, fvPatchField, volMesh>& fld
        ) const;

        //- Read and reconstruct volume field
        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> >
        reconstructFvVolumeField(const IOobject& fieldIoObject) const;



        //- Reconstruct surface field
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        reconstructFvSurfaceField
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        ) const;

        //- Read and reconstruct surface field
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        reconstructFvSurfaceField(const IOobject& fieldIoObject) const;


        //- Read, reconstruct and write all/selected volume internal fields
        template<class Type>
        void reconstructFvVolumeInternalFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );

        //- Read, reconstruct and write all/selected volume fields
        template<class Type>
        void reconstructFvVolumeFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );

        //- Read, reconstruct and write all/selected surface fields
        template<class Type>
        void reconstructFvSurfaceFields
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "parFvFieldReconstructorReconstructFields.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
