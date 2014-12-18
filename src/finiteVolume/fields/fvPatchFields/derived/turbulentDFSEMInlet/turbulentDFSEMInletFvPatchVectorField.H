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

Class
    Foam::turbulentDFSEMInletFvPatchVectorField

Group
    grpInletBoundaryConditions

Description

SeeAlso

SourceFiles
    turbulentDFSEMInletFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef turbulentDFSEMInletFvPatchVectorField_H
#define turbulentDFSEMInletFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"
#include "indexedOctree.H"
#include "treeDataPrimitivePatch.H"
#include "cachedRandom.H"
#include "eddy.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class treeDataFace;
class cartesianCS;

/*---------------------------------------------------------------------------*\
           Class turbulentDFSEMInletFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class turbulentDFSEMInletFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Convenience typedef for tree type
        typedef treeDataPrimitivePatch<polyPatch> treeType;

        //- Mean velocity in patch (negative) normal direction
        scalar UMean_;

        //- Typical length scale, e.g. half channel height
        scalar delta_;

        //- Ratio of sum of eddy volumes to eddy box volume; default = 1
        scalar d_;

        //- Von Karman constant
        scalar kappa_;

        //- Turbulence kinetoc energy
        scalarField k_;

        //- Turbulence dissipation rate
        scalarField epsilon_;

        //- Reynolds stress tensor
        symmTensorField R_;

        //- List of eddies
        List<eddy> eddies_;

        //- Patch normal into the domain
        vector patchNormal_;

        //- Eddy box volume
        scalar v0_;

        //- Eddy box bounds
        vector bounds_;

        //- Octree to search for nearest patch face to point
        autoPtr<indexedOctree<treeType> > treePtr_;

        //- Random number generator
        cachedRandom rndGen_;

        //- Eddy box co-ordinate system
        autoPtr<cartesianCS> boxCoordSystemPtr_;

        //- Length scale per patch face
        scalarField sigmax_;

        //- Global number of eddies
        label nEddy_;

        //- Current time index (used for updating)
        label curTimeIndex_;


    // Private Member Functions

        //- Check the patch
        void checkPatch();

        //- Create octree to search for nearest patch face
        void createTree();

        //- Initialise the eddy box
        void initialiseEddyBox();

        //- Initialise eddies
        void initialiseEddies();

        //- Set a new eddy position
        void setNewPosition(point& p, label& faceI, bool localOnly);

        //- Convect the eddies
        void convectEddies(const scalar deltaT);

        //- Calculate the velocity fluctuation at a point
        vector uDashEddy(const point& globalX) const;


public:

   //- Runtime type information
   TypeName("turbulentDFSEMInlet");


   // Constructors

        //- Construct from patch and internal field
        turbulentDFSEMInletFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        turbulentDFSEMInletFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given turbulentDFSEMInletFvPatchVectorField
        //  onto a new patch
        turbulentDFSEMInletFvPatchVectorField
        (
            const turbulentDFSEMInletFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        turbulentDFSEMInletFvPatchVectorField
        (
            const turbulentDFSEMInletFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new turbulentDFSEMInletFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        turbulentDFSEMInletFvPatchVectorField
        (
            const turbulentDFSEMInletFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new turbulentDFSEMInletFvPatchVectorField(*this, iF)
            );
        }

    virtual ~turbulentDFSEMInletFvPatchVectorField();


    // Member functions


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap(const fvPatchFieldMapper& m);

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchVectorField& ptf,
                const labelList& addr
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
