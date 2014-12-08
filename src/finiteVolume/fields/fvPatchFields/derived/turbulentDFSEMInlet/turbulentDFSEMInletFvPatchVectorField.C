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

\*---------------------------------------------------------------------------*/

#include "turbulentDFSEMInletFvPatchVectorField.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "momentOfInertia.H"
#include "treeBoundBox.H"
#include "cartesianCS.H"
#include "pointIndexHit.H"
#include "treeDataFace.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::turbulentDFSEMInletFvPatchVectorField::checkPatch()
{
    // check that patch is planar
    const vectorField nf(patch().nf());
    patchNormal_ = gAverage(nf);
    scalar error = max(magSqr(patchNormal_ - nf));

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

    // set tolerance
    tolerance_ = gSum(patch().magSf());
}


void Foam::turbulentDFSEMInletFvPatchVectorField::createTree()
{
    treeBoundBox bb(patch().patch().points());
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

    // inialise eddy box extents
    forAll(*this, faceI)
    {
        scalar& s = sigmax_[faceI];

        // length scale in x direction (based on eq. 14)
        s = pow(k_[faceI], 1.5)/(epsilon_[faceI] + ROOTVSMALL);
        s = min(s, kappa_*delta_);
        s = min(s, Foam::sqrt(magSf[faceI]));
    }

    // maximum extent across all processors
    scalar maxSigmax = gMax(sigmax_);

    // patch orientation calculated using moment of inertia
    vector cM;
    scalar mass;
    tensor J(tensor::zero);
    // supply 'true' to reduce parallel results
    momentOfInertia::massPropertiesPatch(pp, 1, mass, cM, J, true);

    // transformation from global to patch
    // - eigen vectors listed in largest eigen value order; largest
    //   assumed to be in the streamwise direction
    tensor R = eigenVectors(J);

    // determine limits of patch in patch axes
    const pointField transformedPoints(pp.localPoints() & R.T());
    vector minP = min(transformedPoints);
    vector maxP = max(transformedPoints);

    reduce(minP, minOp<vector>());
    reduce(maxP, maxOp<vector>());

    bounds_ = (maxP & R) - (minP & R) + 2*maxSigmax*patchNormal_;

    // eddy box volume
    v0_ = bounds_.x()*bounds_.y()*bounds_.z();

    vector origin = (minP & R) - patchNormal_*maxSigmax;

    // assign cooridinate system for eddy box
    // - x: streamwise direction
    // - y,z: wall normal direction
    boxCoordSystemPtr_.reset
    (
        new cartesianCS("eddyBox", origin, R.x(), patchNormal_)
    );

    if (debug)
    {
        Info<< "Patch: " << patch().patch().name() << " eddy box:" << nl
            << "    volume: " << v0_ << nl
            << "    bounds: " << bounds_ << nl
            << "    origin: " << origin << nl
            << "    e3    : " << R.x() << nl
            << "    e1    : " << patchNormal_ << nl
            << endl;
    }
}


void Foam::turbulentDFSEMInletFvPatchVectorField::initialiseEddies()
{
    DynamicList<eddy> eddies(size());

    // initialise eddy properties
    scalar sumVolEddy = 0;
    scalar sumVolEddyAllProc = 0;

    point position;
    label faceI = -1;
    while (sumVolEddyAllProc/v0_ < d_)
    {
        setNewPosition(position, faceI, false);

        if (faceI != -1)
        {
            eddy e(position, sigmax_[faceI], R_[faceI], patchNormal_, rndGen_);
            eddies.append(e);
            sumVolEddy += e.volume();
        }
        // else eddy on remote processor

        sumVolEddyAllProc = returnReduce(sumVolEddy, sumOp<scalar>());
    }
    eddies_.transfer(eddies);

    nEddy_ = eddies_.size();

    if (debug)
    {
        Pout<< "Patch:" << patch().patch().name()
            << " processor:" << Pstream::myProcNo()
            << " seeded:" <<  eddies_.size() << " eddies" << endl;
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


void Foam::turbulentDFSEMInletFvPatchVectorField::setNewPosition
(
    point& p,
    label& faceI,
    const bool localOnly
)
{
    List<label> procHits(Pstream::nProcs(), 0);

    while (true)
    {
        // local position in eddy box
        p = cmptMultiply(rndGen_.sample01<vector>(), bounds_);

        // snap point to inlet plane at x = 0
        point pDash = p;
        pDash.x() = 0.5*bounds_.x();

        // position in global system
        pDash = boxCoordSystemPtr_->globalPosition(pDash);

        pointIndexHit sample = treePtr_->findNearest(pDash, tolerance_);

        bool hit = sample.hit();

        if (!localOnly)
        {
            if (hit)
            {
                procHits[Pstream::myProcNo()] = 1;
            }
            else
            {
                procHits[Pstream::myProcNo()] = 0;
            }

            Pstream::listCombineGather(procHits, plusEqOp<label>());
            Pstream::listCombineScatter(procHits);

            label nHits = sum(procHits);

            if (nHits == 1)
            {
                faceI = sample.index();

                return;
            }
            else if (nHits > 1)
            {
                // more than 1 processor has hit this position
                // - select the processor with the lowest index
                label winningProcI = -1;
                forAll(procHits, procI)
                {
                    if (procHits[procI] != 0)
                    {
                        winningProcI = procI;
                        break;
                    }
                }

                if (Pstream::myProcNo() == winningProcI)
                {
                    faceI = sample.index();
                }
                else
                {
                    faceI = -1;
                }

                return;
            }
        }

        if (hit)
        {
            faceI = sample.index();
            return;
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

        e.move(deltaT*UMean_);

        // check to see if eddy has exited downstream box plane
        if (e.position().x() - 0.5*e.sigmaXMax() > bounds_.x())
        {
            scalar overshoot =
                e.position().x() - 0.5*e.sigma().x() - bounds_.x();

            // Spawn new eddy on this processor

            point newPosition = point::zero;
            label faceI = -1;
            setNewPosition(newPosition, faceI, true);

            e = eddy
                (
                    newPosition,
                    sigmax_[faceI],
                    R_[faceI],
                    patchNormal_,
                    rndGen_
                );
            e.initialise(nEddy, v0_);

            // include any overshoot when setting eddy x position
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
) const
{
    vector uDash(vector::zero);

    // convert patch face centre to eddy box system
    point xp = boxCoordSystemPtr_->localPosition(globalX);

    forAll(eddies_, k)
    {
        const eddy& e = eddies_[k];

        // eddy centre
        const point& xk = e.position();

        // length scales
        const vector& sigma = e.sigma();

        // intensities
        const vector& alpha = e.alpha();

        // relative distance inside eddy
        vector r = cmptDivide(xp - xk, sigma);

        scalar r2 = magSqr(r);

        if (r2 < 1)
        {
            // fluctuating velocity in principal axes system (eq. 8)
            vector uDashLoc = cmptMultiply(sigma, (1 - r2)*(r^alpha));

            // add contribution into eddy box system (eq. 10)
            uDash += e.c1()*(e.Rpg() & uDashLoc);
        }
    }

    return uDash;
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
    UMean_(0),
    delta_(0),
    d_(0),
    kappa_(0),
    k_(size(), 0),
    epsilon_(size(), 0),
    R_(size(), symmTensor::zero),
    eddies_(0),
    patchNormal_(vector::zero),
    v0_(0),
    bounds_(vector::zero),
    tolerance_(0),
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
    UMean_(ptf.UMean_),
    delta_(ptf.delta_),
    d_(ptf.d_),
    kappa_(ptf.kappa_),
    k_(ptf.k_, mapper),
    epsilon_(ptf.epsilon_, mapper),
    R_(ptf.R_, mapper),
    eddies_(ptf.eddies_),
    patchNormal_(ptf.patchNormal_),
    v0_(ptf.v0_),
    bounds_(ptf.bounds_),
    tolerance_(ptf.tolerance_),
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
    UMean_(readScalar(dict.lookup("UMean"))),
    delta_(readScalar(dict.lookup("delta"))),
    d_(dict.lookupOrDefault("d", 1)),
    kappa_(dict.lookupOrDefault("kappa", 0.41)),
    k_("k", dict, p.size()),
    epsilon_("epsilon", dict, p.size()),
    R_("R", dict, p.size()),
    eddies_(),
    patchNormal_(vector::zero),
    v0_(0),
    bounds_(vector::zero),
    tolerance_(0),
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
    UMean_(ptf.UMean_),
    delta_(ptf.delta_),
    d_(ptf.d_),
    kappa_(ptf.kappa_),
    k_(ptf.k_),
    epsilon_(ptf.epsilon_),
    R_(ptf.R_),
    eddies_(ptf.eddies_),
    patchNormal_(ptf.patchNormal_),
    v0_(ptf.v0_),
    bounds_(ptf.bounds_),
    tolerance_(ptf.tolerance_),
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
    UMean_(ptf.UMean_),
    delta_(ptf.delta_),
    d_(ptf.d_),
    kappa_(ptf.kappa_),
    k_(ptf.k_),
    epsilon_(ptf.epsilon_),
    R_(ptf.R_),
    eddies_(ptf.eddies_),
    patchNormal_(ptf.patchNormal_),
    v0_(ptf.v0_),
    bounds_(ptf.bounds_),
    tolerance_(ptf.tolerance_),
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
    k_.autoMap(m);
    epsilon_.autoMap(m);
    R_.autoMap(m);
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

    k_.rmap(dfsemptf.k_, addr);
    epsilon_.rmap(dfsemptf.epsilon_, addr);
    R_.rmap(dfsemptf.R_, addr);
    sigmax_.rmap(dfsemptf.sigmax_, addr);
}


void Foam::turbulentDFSEMInletFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    if (curTimeIndex_ == -1)
    {
        checkPatch();

        createTree();

        initialiseEddyBox();

        initialiseEddies();
    }


    if (curTimeIndex_ != db().time().timeIndex())
    {
        const scalar deltaT = db().time().deltaTValue();

        // move eddies using mean velocity
        convectEddies(deltaT);

        // set velocity
        vectorField& U = *this;
        U = -UMean_*patchNormal_;

        const pointField& Cf = patch().Cf();

        scalar invRootN = 1/Foam::sqrt(scalar(nEddy_));

        forAll(U, faceI)
        {
            U[faceI] += invRootN*uDashEddy(Cf[faceI]);
        }

        // rescale to ensure correct flow rate
        scalar fCorr = gSum(U & patch().Sf())/gSum(patch().magSf())/mag(UMean_);
        U *= fCorr;

        if (debug)
        {
            Info<< "patch:" << patch().patch().name()
                << " min/max(U):" << gMin(U) << ", " << gMax(U) << endl;
        }


        curTimeIndex_ = db().time().timeIndex();

        if (db().time().outputTime())
        {
            // create a set of the eddies
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
    os.writeKeyword("d") << d_ << token::END_STATEMENT << nl;
    os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
    k_.writeEntry("k", os);
    epsilon_.writeEntry("epsilon", os);
    R_.writeEntry("R", os);
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
