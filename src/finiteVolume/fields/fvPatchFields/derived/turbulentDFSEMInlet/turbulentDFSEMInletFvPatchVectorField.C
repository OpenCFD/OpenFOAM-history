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

\*---------------------------------------------------------------------------*/

#include "turbulentDFSEMInletFvPatchVectorField.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "momentOfInertia.H"
#include "cartesianCS.H"
#include "treeDataFace.H"
#include "globalIOFields.H"
#include "globalIndex.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

const Foam::globalIndex&
Foam::turbulentDFSEMInletFvPatchVectorField::globalFaces() const
{
    if (!globalFacesPtr_.valid())
    {
        globalFacesPtr_.reset
        (
            new globalIndex
            (
                patch().boundaryMesh().mesh().nFaces()
            )
        );
    }
    return globalFacesPtr_();
}


void Foam::turbulentDFSEMInletFvPatchVectorField::interpolateBoundary()
{
    // Construct a spatial interpolation (using constant/boundaryData
    // points), read the available list of sampled data ('u',R') and
    // interpolate in time. (similar to timeVaryingMappedFixedValue)
    // In general this is overkill and both fields are static.
    // In this case we probably shouldn't be holding the interpolation
    // structure and the current field values ('*Start_', '*End') for
    // the current time.


    // Initialise interpolation (2D planar interpolation by triangulation)
    if (mapperPtr_.empty())
    {
        vectorGlobalIOField samplePoints
        (
            IOobject
            (
                "points",
                this->db().time().caseConstant(),
                "boundaryData"/this->patch().name(),
                this->db(),
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE,
                false
            )
        );

        const fileName samplePointsFile = samplePoints.filePath();

        if (debug)
        {
            Info<< "turbulentDFSEMInletFvPatchVectorField :"
                << " Read " << samplePoints.size() << " sample points from "
                << samplePointsFile << endl;
        }


        // tbd: run-time selection
        bool nearestOnly =
        (
           !mapMethod_.empty()
         && mapMethod_ != "planarInterpolation"
        );

        // Allocate the interpolator
        mapperPtr_.reset
        (
            new pointToPointPlanarInterpolation
            (
                samplePoints,
                this->patch().patch().faceCentres(),
                perturb_,
                nearestOnly
            )
        );

        // Read the times for which data is available
        const fileName samplePointsDir = samplePointsFile.path();
        sampleTimes_ = Time::findTimes(samplePointsDir);

        if (debug)
        {
            Info<< "turbulentDFSEMInletFvPatchVectorField : In directory "
                << samplePointsDir << " found times "
                << pointToPointPlanarInterpolation::timeNames(sampleTimes_)
                << endl;
        }
    }


    // Find current time in sampleTimes
    label lo = -1;
    label hi = -1;

    bool foundTime = mapperPtr_().findTime
    (
        sampleTimes_,
        startSampleTime_,
        this->db().time().value(),
        lo,
        hi
    );

    if (!foundTime)
    {
        FatalErrorIn
        (
            "turbulentDFSEMInletFvPatchVectorField::interpolateBoundary()"
        )   << "Cannot find starting sampling values for current time "
            << this->db().time().value() << nl
            << "Have sampling values for times "
            << pointToPointPlanarInterpolation::timeNames(sampleTimes_) << nl
            << "In directory "
            <<  this->db().time().constant()/"boundaryData"/this->patch().name()
            << "\n    on patch " << this->patch().name()
            << " of field " << this->dimensionedInternalField().name()
            << exit(FatalError);
    }


    // Update sampled data fields

    if (lo != startSampleTime_)
    {
        startSampleTime_ = lo;

        if (startSampleTime_ == endSampleTime_)
        {
            // No need to re-read since start=end values
            if (debug)
            {
                Pout<< "interpolateBoundary : Setting startValues to "
                    << "(already read) boundaryData"
                       /this->patch().name()
                       /sampleTimes_[startSampleTime_].name()
                    << endl;
            }
            UMeanStart_ = UMeanEnd_;
            RStart_ = REnd_;
        }
        else
        {
            if (debug)
            {
                Pout<< "interpolateBoundary : Reading startValues from "
                    << "boundaryData"
                       /this->patch().name()
                       /sampleTimes_[lo].name()
                    << endl;
            }

            UMeanStart_ =
                interpolateBoundaryData<scalar>("U", startSampleTime_);
            RStart_ =
                interpolateBoundaryData<symmTensor>("R", startSampleTime_);
        }
    }

    if (hi != endSampleTime_)
    {
        endSampleTime_ = hi;

        if (endSampleTime_ == -1)
        {
            // endTime no longer valid. Might as well clear *End values.=
            if (debug)
            {
                Pout<< "interpolateBoundary : Clearing *End values" << endl;
            }
            UMeanEnd_.clear();
            REnd_.clear();
        }
        else
        {
            if (debug)
            {
                Pout<< "interpolateBoundary : Reading *End values from "
                    << "boundaryData"
                       /this->patch().name()
                       /sampleTimes_[endSampleTime_].name()
                    << endl;
            }

            UMeanEnd_ = interpolateBoundaryData<scalar>("U", endSampleTime_);
            REnd_ = interpolateBoundaryData<symmTensor>("R", endSampleTime_);
        }
    }


    // Now we have cached start- and end-time values for UMean and R just use
    // linear interpolation
    if (endSampleTime_ == -1)
    {
        // Only start value
        if (debug)
        {
            Pout<< "interpolateBoundary : Sampled, non-interpolated values"
                << " from start time:" << sampleTimes_[startSampleTime_].name()
                << nl;
        }
        UMean_ = UMeanStart_;
        R_ = RStart_;
    }
    else
    {
        scalar start = sampleTimes_[startSampleTime_].value();
        scalar end = sampleTimes_[endSampleTime_].value();

        scalar s = (this->db().time().value() - start)/(end - start);

        if (debug)
        {
            Pout<< "interpolateBoundary : Sampled, interpolated values"
                << " between start time:"
                << sampleTimes_[startSampleTime_].name()
                << " and end time:" << sampleTimes_[endSampleTime_].name()
                << " with weight:" << s << endl;
        }

        UMean_ = (1 - s)*UMeanStart_ + s*UMeanEnd_;
        R_ = (1 - s)*RStart_ + s*REnd_;
    }
}


void Foam::turbulentDFSEMInletFvPatchVectorField::checkPatch()
{
    const vectorField nf(patch().nf());

    // Patch normal points into domain
    patchNormal_ = -gAverage(nf);

    // Check that patch is planar
    scalar error = max(magSqr(patchNormal_ + nf));

    if (error > SMALL)
    {
        WarningIn
        (
            "void Foam::turbulentDFSEMInletFvPatchVectorField::"
            "checkPatch() const"
        )
            << "Patch " << patch().name() << " is not planar"
            << endl;
    }

    patchNormal_ /= mag(patchNormal_) + ROOTVSMALL;
}


void Foam::turbulentDFSEMInletFvPatchVectorField::createTree()
{
    treeBoundBox bb(patch().patch().points(), patch().patch().meshPoints());
    bb.inflate(0.01);

    if (!treePtr_.valid())
    {
        treePtr_.reset
        (
            new indexedOctree<treeType>
            (
                treeType
                (
                    false,
                    patch().patch(),
                    indexedOctree<treeDataFace>::perturbTol()
                ),
                bb,                         // overall search domain
                8,                          // maxLevel
                10,                         // leaf size
                3.0                         // duplicity
            )
        );
    }
}


void Foam::turbulentDFSEMInletFvPatchVectorField::initialiseEddyBox()
{
    const polyPatch& pp = patch().patch();
    const scalarField& magSf = patch().magSf();

    // Inialise eddy box extents
    forAll(*this, faceI)
    {
        scalar& s = sigmax_[faceI];

        // Length scale in x direction (based on eq. 14)
        //s = pow(k_[faceI], 1.5)/(epsilon_[faceI] + ROOTVSMALL);
        s = kappa_*delta_;
        s = min(s, Foam::sqrt(magSf[faceI]));
    }

    // Maximum extent across all processors
    scalar maxSigmax = gMax(sigmax_);

    // Patch orientation calculated using moment of inertia
    vector cM;
    scalar mass;
    tensor J(tensor::zero);
    // Supply 'true' to reduce parallel results
    momentOfInertia::massPropertiesPatch(pp, 1, mass, cM, J, true);

    // Transformation from global to patch
    // - eigen vectors listed in largest eigen value order
    // - smallest should be in the streamwise direction (normal to patch)
    tensor R = eigenVectors(J);

    // Determine limits of patch in patch local system
    const pointField transformedPoints(pp.localPoints() & R.T());
    vector minP = min(transformedPoints);
    vector maxP = max(transformedPoints);
    reduce(minP, minOp<vector>());
    reduce(maxP, maxOp<vector>());

    // Eddy box is +/- maxSigmax on either side of patch
    // - note: patch located at box mid-plane
    vector origin = (minP & R) - patchNormal_*maxSigmax;

    // Assign cooridinate system for eddy box
    // - x: streamwise direction
    // - y,z: wall normal direction
    if (((R.x()^R.y()) & patchNormal_) > 0)
    {
        boxCoordSystemPtr_.reset
        (
            new cartesianCS("eddyBox", origin, R.y(), patchNormal_)
        );
    }
    else
    {
        boxCoordSystemPtr_.reset
        (
            new cartesianCS("eddyBox", origin, R.x(), patchNormal_)
        );
    }

    // Eddy box bounds in box co-ordinate system
    vector minPeb = boxCoordSystemPtr_->localPosition(minP & R);
    vector maxPeb = boxCoordSystemPtr_->localPosition(maxP & R);
    vector dxeb = boxCoordSystemPtr_->localVector(2*maxSigmax*patchNormal_);
    bounds_ = cmptMag(maxPeb - minPeb + dxeb);

    // Eddy box volume
    v0_ = bounds_.x()*bounds_.y()*bounds_.z();

    if (debug)
    {
        Info<< "Patch: " << patch().patch().name() << " eddy box:" << nl
            << "    volume    : " << v0_ << nl
            << "    bounds    : " << bounds_ << nl
            << "    origin    : " << origin << nl
            << "    e1        : " << boxCoordSystemPtr_->R().e1() << nl
            << "    e2        : " << boxCoordSystemPtr_->R().e2() << nl
            << "    e3        : " << boxCoordSystemPtr_->R().e3() << nl
            << "    normal    : " << patchNormal_ << nl
            << "    R         : " << R << nl
            << "    Cf        : " << gAverage(patch().Cf()) << nl
            << "    minP      : " << minP << nl
            << "    maxP      : " << maxP << nl
            << "    minP&R    : " << (minP & R) << nl
            << "    maxP&R    : " << (maxP & R) << nl
            << "    maxSigmax : " << maxSigmax << nl
            << endl;
    }
}


void Foam::turbulentDFSEMInletFvPatchVectorField::initialiseEddies()
{
    DynamicList<eddy> eddies(size());

    // Initialise eddy properties
    scalar sumVolEddy = 0;
    scalar sumVolEddyAllProc = 0;

    while (sumVolEddyAllProc/v0_ < d_)
    {
        // Get new eddy on one of the processors
        pointIndexHit pos(setNewPosition(false));

        label faceI = pos.index();

        if (faceI != -1)
        {
            eddy e
            (
                faceI,
                pos.hitPoint(),
                sigmax_[faceI],
                R_[faceI],
                patchNormal_,
                rndGen_
            );
            eddies.append(e);
            sumVolEddy += e.boxVolume();
        }
        // else eddy on remote processor

        sumVolEddyAllProc = returnReduce(sumVolEddy, sumOp<scalar>());
    }
    eddies_.transfer(eddies);

    nEddy_ = eddies_.size();

    if (debug)
    {
        Pout<< "Patch:" << patch().patch().name();

        if (Pstream::parRun())
        {
            Pout<< " processor:" << Pstream::myProcNo();
        }

        Pout<< " seeded:" << nEddy_ << " eddies" << endl;
    }

    reduce(nEddy_, sumOp<label>());

    if (nEddy_ > 0)
    {
        Info<< "Patch: " << patch().patch().name() << " seeded "
            << nEddy_ << " eddies" << endl;
    }
    else
    {
        WarningIn
        (
            "void Foam::turbulentDFSEMInletFvPatchVectorField::"
            "initialiseEddies()"
        )
            << "Patch: " << patch().patch().name()
            << " on field " << dimensionedInternalField().name()
            << ": No eddies seeded - please check your set-up" << endl;
    }

    forAll(eddies_, eddyI)
    {
        eddies_[eddyI].initialise(nEddy_, v0_);
    }
}


Foam::pointIndexHit Foam::turbulentDFSEMInletFvPatchVectorField::setNewPosition
(
    const bool localOnly
)
{
    // Initialise to miss
    pointIndexHit pos(false, vector::max, -1);

    while (true)
    {
        // Generate a position within eddy box
        if (localOnly)
        {
            pos.rawPoint() = cmptMultiply(rndGen_.sample01<vector>(), bounds_);
        }
        else
        {
            if (Pstream::master())
            {
                pos.rawPoint() =
                    cmptMultiply(rndGen_.sample01<vector>(), bounds_);
            }
            Pstream::scatter(pos.rawPoint());
        }

        // Snap point to inlet plane at x = 0
        point pDash = pos.rawPoint();
        pDash.x() = 0.5*bounds_.x();

        // Position in global system
        pDash = boxCoordSystemPtr_->globalPosition(pDash);

        point q(2*bounds_.x(), pos.rawPoint().y(), pos.rawPoint().z());
        q = boxCoordSystemPtr_->globalPosition(q);
        vector dx = q - pDash;

        // Find patch face
        pointIndexHit sample = treePtr_->findLineAny(pDash-dx, pDash+dx);

        if (Pstream::parRun() && !localOnly)
        {
            label globalHit = returnReduce
            (
                (
                    sample.hit()
                  ? globalFaces().toGlobal(sample.index())
                  : -1
                ),
                maxOp<label>()
            );

            if (globalHit != -1)
            {
                if (globalFaces().isLocal(globalHit))
                {
                    pos.setHit();
                    pos.setIndex(sample.index());
                }
                return pos;
            }
        }
        else if (sample.hit())
        {
            pos.setHit();
            pos.setIndex(sample.index());
            return pos;
        }
    }
}


void Foam::turbulentDFSEMInletFvPatchVectorField::convectEddies
(
    const scalar deltaT
)
{
    const label nEddy = eddies_.size();

    label nRecycled = 0;

    forAll(eddies_, eddyI)
    {
        eddy& e = eddies_[eddyI];
        const label eFaceI = e.patchFaceI();

        e.move(deltaT*UMean_[eFaceI]);

        const scalar position0 = e.position().x();

        // Check to see if eddy has exited downstream box plane
        if (position0 > bounds_.x())
        {
            // Spawn new eddy on this processor
            pointIndexHit pos(setNewPosition(true));

            label faceI = pos.index();

            e = eddy
                (
                    faceI,
                    pos.hitPoint(),
                    sigmax_[faceI],
                    R_[faceI],
                    patchNormal_,
                    rndGen_
                );
            e.initialise(nEddy, v0_);

            // Include any overshoot when setting eddy x position to maintain
            // similar effective eddy density
            scalar overshoot = position0 - bounds_.x();
            overshoot -= floor(overshoot/bounds_.x())*bounds_.x();
            e.move(overshoot);

            nRecycled++;
        }
    }

    reduce(nRecycled, sumOp<label>());

    if (nRecycled > 0)
    {
        Info<< "Patch: " << patch().patch().name() << " recycled "
            << nRecycled << " eddies" << endl;
    }
}


Foam::vector Foam::turbulentDFSEMInletFvPatchVectorField::uDashEddy
(
    const point& globalX
)
{
    vector uDash(vector::zero);

    // Convert patch face centre to eddy box system
    point xp = boxCoordSystemPtr_->localPosition(globalX);

    forAll(eddies_, k)
    {
        const eddy& e = eddies_[k];

        uDash += e.uDash(xp, rndGen_);
    }

    return boxCoordSystemPtr_->globalVector(uDash);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::turbulentDFSEMInletFvPatchVectorField::
turbulentDFSEMInletFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    delta_(0),
    d_(0),
    kappa_(0),

    perturb_(1e-5),
    mapMethod_("planarInterpolation"),
    startSampleTime_(-1),
    endSampleTime_(-1),

    eddies_(0),
    patchNormal_(vector::zero),
    v0_(0),
    bounds_(vector::zero),
    treePtr_(),
    rndGen_(1234, -1),
    boxCoordSystemPtr_(),
    sigmax_(size(), 0),
    nEddy_(0),
    curTimeIndex_(-1)
{}


Foam::turbulentDFSEMInletFvPatchVectorField::
turbulentDFSEMInletFvPatchVectorField
(
    const turbulentDFSEMInletFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    delta_(ptf.delta_),
    d_(ptf.d_),
    kappa_(ptf.kappa_),

    perturb_(ptf.perturb_),
    mapMethod_(ptf.mapMethod_),
    mapperPtr_(NULL),
    startSampleTime_(-1),
    endSampleTime_(-1),
    UMean_(ptf.UMean_, mapper),
    R_(ptf.R_, mapper),

    eddies_(ptf.eddies_),
    patchNormal_(ptf.patchNormal_),
    v0_(ptf.v0_),
    bounds_(ptf.bounds_),
    treePtr_(),
    rndGen_(1234, -1),
    boxCoordSystemPtr_(),
    sigmax_(ptf.sigmax_, mapper),
    nEddy_(0),
    curTimeIndex_(-1)
{}


Foam::turbulentDFSEMInletFvPatchVectorField::
turbulentDFSEMInletFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF, dict),
    delta_(readScalar(dict.lookup("delta"))),
    d_(dict.lookupOrDefault("d", 1)),
    kappa_(dict.lookupOrDefault("kappa", 0.41)),

    perturb_(dict.lookupOrDefault("perturb", 1e-5)),
    mapMethod_(dict.lookup("mapMethod")),
    mapperPtr_(NULL),
    startSampleTime_(-1),
    endSampleTime_(-1),

    eddies_(),
    patchNormal_(vector::zero),
    v0_(0),
    bounds_(vector::zero),
    treePtr_(),
    rndGen_(1234, -1),
    boxCoordSystemPtr_(),
    sigmax_(size(), 0),
    nEddy_(0),
    curTimeIndex_(-1)
{}


Foam::turbulentDFSEMInletFvPatchVectorField::
turbulentDFSEMInletFvPatchVectorField
(
    const turbulentDFSEMInletFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    delta_(ptf.delta_),
    d_(ptf.d_),
    kappa_(ptf.kappa_),

    perturb_(ptf.perturb_),
    mapMethod_(ptf.mapMethod_),
    mapperPtr_(NULL),
    startSampleTime_(-1),
    endSampleTime_(-1),
    UMean_(ptf.UMean_),
    R_(ptf.R_),

    eddies_(ptf.eddies_),
    patchNormal_(ptf.patchNormal_),
    v0_(ptf.v0_),
    bounds_(ptf.bounds_),
    treePtr_(),
    rndGen_(1234, -1),
    boxCoordSystemPtr_(),
    sigmax_(ptf.sigmax_),
    nEddy_(0),
    curTimeIndex_(-1)
{}


Foam::turbulentDFSEMInletFvPatchVectorField::
turbulentDFSEMInletFvPatchVectorField
(
    const turbulentDFSEMInletFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    delta_(ptf.delta_),
    d_(ptf.d_),
    kappa_(ptf.kappa_),

    perturb_(ptf.perturb_),
    mapMethod_(ptf.mapMethod_),
    mapperPtr_(NULL),
    startSampleTime_(-1),
    endSampleTime_(-1),
    UMean_(ptf.UMean_),
    R_(ptf.R_),

    eddies_(ptf.eddies_),
    patchNormal_(ptf.patchNormal_),
    v0_(ptf.v0_),
    bounds_(ptf.bounds_),
    treePtr_(),
    rndGen_(1234, -1),
    boxCoordSystemPtr_(),
    sigmax_(ptf.sigmax_),
    nEddy_(0),
    curTimeIndex_(-1)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::turbulentDFSEMInletFvPatchVectorField::~
turbulentDFSEMInletFvPatchVectorField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::turbulentDFSEMInletFvPatchVectorField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    fixedValueFvPatchField<vector>::autoMap(m);

    if (UMeanStart_.size())
    {
        UMeanStart_.autoMap(m);
        UMeanEnd_.autoMap(m);
        RStart_.autoMap(m);
        REnd_.autoMap(m);
    }

    // Clear interpolator
    mapperPtr_.clear();
    startSampleTime_ = -1;
    endSampleTime_ = -1;

    sigmax_.autoMap(m);
}


void Foam::turbulentDFSEMInletFvPatchVectorField::rmap
(
    const fvPatchVectorField& ptf,
    const labelList& addr
)
{
    fixedValueFvPatchField<vector>::rmap(ptf, addr);

    const turbulentDFSEMInletFvPatchVectorField& dfsemptf =
        refCast<const turbulentDFSEMInletFvPatchVectorField>(ptf);

    UMeanStart_.rmap(dfsemptf.UMeanStart_, addr);
    UMeanEnd_.rmap(dfsemptf.UMeanEnd_, addr);

    RStart_.rmap(dfsemptf.RStart_, addr);
    REnd_.rmap(dfsemptf.REnd_, addr);

    // Clear interpolator
    mapperPtr_.clear();
    startSampleTime_ = -1;
    endSampleTime_ = -1;

    sigmax_.rmap(dfsemptf.sigmax_, addr);
}


void Foam::turbulentDFSEMInletFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    // Interpolate UMean and R from table data (read from constant/boundaryData)
    // (optionally interpolate in time as well)
    interpolateBoundary();

    if (curTimeIndex_ == -1)
    {
        // Ensure globalFaces constructed consistently in parallel
        (void)globalFaces();

        checkPatch();

        createTree();

        initialiseEddyBox();

        initialiseEddies();
    }


    if (curTimeIndex_ != db().time().timeIndex())
    {
        const scalar deltaT = db().time().deltaTValue();

        // Move eddies using mean velocity
        convectEddies(deltaT);

        // Set velocity
        vectorField& U = *this;
        U = UMean_*patchNormal_;

        const pointField& Cf = patch().Cf();

        scalar invRootN = 1/Foam::sqrt(scalar(nEddy_));

        forAll(U, faceI)
        {
            U[faceI] += invRootN*uDashEddy(Cf[faceI]);
        }

        // Re-scale to ensure correct flow rate
        scalar fCorr = gSum(U & -patch().Sf())/gSum(UMean_*patch().magSf());
        U *= fCorr;

        if (debug)
        {
            Info<< "Patch:" << patch().patch().name()
                << " min/max(U):" << gMin(U) << ", " << gMax(U) << endl;
        }

        curTimeIndex_ = db().time().timeIndex();

        if (debug && db().time().outputTime())
        {
            // Create a set of the eddies
            OFstream os(db().time().path()/"eddies.obj");
            forAll(eddies_, eddyI)
            {
                const point& localP = eddies_[eddyI].position();
                point p = boxCoordSystemPtr_->globalPosition(localP);

                os  << "v " << p.x() << " " << p.y() << " " << p.z() << nl;
            }
        }
    }

    fixedValueFvPatchVectorField::updateCoeffs();
}


void Foam::turbulentDFSEMInletFvPatchVectorField::write(Ostream& os) const
{
    fvPatchField<vector>::write(os);
    writeEntry("value", os);
    os.writeKeyword("UMean") << UMean_ << token::END_STATEMENT << nl;
    os.writeKeyword("delta") << delta_ << token::END_STATEMENT << nl;
    if (d_ != 1)
    {
        os.writeKeyword("d") << d_ << token::END_STATEMENT << nl;
    }
    if (kappa_ != 0.41)
    {
        os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
    }

    if (perturb_ != 1e-5)
    {
        os.writeKeyword("perturb") << perturb_ << token::END_STATEMENT << nl;
    }

    if
    (
       !mapMethod_.empty()
     && mapMethod_ != "planarInterpolation"
    )
    {
        os.writeKeyword("mapMethod") << mapMethod_
            << token::END_STATEMENT << nl;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
   makePatchTypeField
   (
       fvPatchVectorField,
       turbulentDFSEMInletFvPatchVectorField
   );
}


// ************************************************************************* //
