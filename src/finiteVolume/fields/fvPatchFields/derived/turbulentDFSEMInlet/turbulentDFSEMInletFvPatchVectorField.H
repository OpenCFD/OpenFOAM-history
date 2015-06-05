/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenFOAM Foundation
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
    Velocity boundary condition from synthesised eddies for use in LES

    Reference:
    \verbatim
        Poletto, R., Craft, T., and Revell, A.,
        "A New Divergence Free Synthetic Eddy Method for the Reproduction
         of Inlet Flow Conditions for LES",
        Flow Turbulence Combust (2013) 91:519-539
    \endverbatim

    \table
        Property  | Description         | Required         | Default value
        value     | restart value       | yes              |
        UMean     | velocity mean       | yes              |
        delta     | local length scale  | yes              |
        d         | fill fraction       | no               | 1
        kappa     | Von Karman constant | no               | 0.41
        mapMethod | interpolation to obtain initial R | no | planarInterpolation
        perturb   | point perturbation for initial R | no  | 1e-5
    \endtable


SeeAlso
    timeVaryingMappedFixedValueFvPatchField

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
#include "pointIndexHit.H"
#include "instantList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class cartesianCS;
class globalIndex;
class pointToPointPlanarInterpolation;

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
        const scalar UMean_;

        //- Typical length scale, e.g. half channel height
        const scalar delta_;

        //- Ratio of sum of eddy volumes to eddy box volume; default = 1
        const scalar d_;

        //- Von Karman constant
        const scalar kappa_;

        //- Global numbering for faces
        mutable autoPtr<globalIndex> globalFacesPtr_;


        // Table reading of Reynolds stress tensor
        // (see timeVaryingMappedFixedValue)

            //- Fraction of perturbation (fraction of bounding box) to add
            scalar perturb_;

            //- Interpolation scheme to use
            word mapMethod_;

            //- 2D interpolation (for 'planarInterpolation' mapMethod)
            autoPtr<pointToPointPlanarInterpolation> mapperPtr_;

            //- List of boundaryData time directories
            instantList sampleTimes_;

            //- Current starting index in sampleTimes
            label startSampleTime_;

            //- Current end index in sampleTimes
            label endSampleTime_;

            //- R at startSampleTime_
            symmTensorField RStart_;

            //- R at endSampleTime_
            symmTensorField REnd_;

            //- Interpolated for current Time
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

        //- Demand-driven access to global mesh-face numbering
        const globalIndex& globalFaces() const;

        //- Check 
        void checkTable();

        //- Check the patch
        void checkPatch();

        //- Create octree to search for nearest patch face
        void createTree();

        //- Initialise the eddy box
        void initialiseEddyBox();

        //- Initialise eddies
        void initialiseEddies();

        //- Set a new eddy position. In parallel guarantees only one processor
        //  selected
        pointIndexHit setNewPosition(bool localOnly);

        //- Convect the eddies
        void convectEddies(const scalar deltaT);

        //- Calculate the velocity fluctuation at a point
        vector uDashEddy(const point& globalX);


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
