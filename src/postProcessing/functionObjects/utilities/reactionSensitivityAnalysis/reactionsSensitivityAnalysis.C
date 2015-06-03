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

#include "reactionsSensitivityAnalysis.H"
#include "dictionary.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::createFiles()
{
    if (writeToFile() && !productionFilePtr_.valid())
    {
        productionFilePtr_ = createFile("production");
        writeLocalHeader("production", productionFilePtr_());

        consumptionFilePtr_ = createFile("consumption");
        writeLocalHeader("consumption", consumptionFilePtr_());

        productionIntFilePtr_ = createFile("productionInt");
        writeLocalHeader("production integral", productionIntFilePtr_());

        consumptionIntFilePtr_ = createFile("consumptionInt");
        writeLocalHeader("consumption integral", consumptionIntFilePtr_());
    }
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::writeLocalHeader
(
    const string& header,
    Ostream& os
) const
{
    writeHeader(os, header);
    writeCommented(os, "Time");

    forAll(speciesNames_, k)
    {
        writeTabbed(os, speciesNames_[k]);
    }

    os  << nl;
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::calculateSpeciesRR
(
    const basicChemistryModel& basicChemistry
)
{
    tmp<DimensionedField<scalar, volMesh> > RRt
    (
        new DimensionedField<scalar, volMesh>
        (
            IOobject
            (
                "RR",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
        )
    );

    DimensionedField<scalar, volMesh>& RR = RRt();

    scalar dt = mesh_.time().deltaT().value();

    endTime_ += dt;

    forAll(production_, specieI)
    {
        forAll(production_[specieI], reactionI)
        {
            RR = basicChemistry.calculateRR(reactionI, specieI);

            if (RR[0] > 0.0)
            {
                production_[specieI][reactionI] = RR[0];
                productionInt_[specieI][reactionI] += dt*RR[0];
            }
            else if (RR[0] < 0.0)
            {
                consumption_[specieI][reactionI] = RR[0];
                consumptionInt_[specieI][reactionI] += dt*RR[0];
            }
            else
            {
                production_[specieI][reactionI] = 0.0;
                consumption_[specieI][reactionI] = 0.0;
            }
        }
    }
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::writeSpeciesRR()
{
    if (!writeToFile())
    {
        return;
    }

    const scalar t = mesh_.time().value();
    const scalar deltaT = mesh_.time().deltaTValue();

    consumptionFilePtr_() << "time : " << t << tab << nl;
    consumptionFilePtr_() << "delta T : " << deltaT << nl << nl;
    productionFilePtr_() << "time : " << t << tab << nl;
    productionFilePtr_() << "delta T : " << deltaT << nl << nl;

    consumptionIntFilePtr_()
        << "start time : " << startTime_ << tab
        << "end time :" <<  endTime_ << nl;

    productionIntFilePtr_()
        << "start time : " << startTime_ << tab
        << "end time :" <<  endTime_ << nl;

    for (label reactionI = 0; reactionI < nReactions_; reactionI++)
    {
        consumptionFilePtr_() << reactionI << tab;
        consumptionIntFilePtr_() << reactionI << tab;
        productionFilePtr_() << reactionI << tab;
        productionIntFilePtr_() << reactionI << tab;

        forAll(speciesNames_, i)
        {
            productionFilePtr_() << production_[i][reactionI] << tab;
            consumptionFilePtr_() << consumption_[i][reactionI] << tab;
            productionIntFilePtr_() << productionInt_[i][reactionI] << tab;
            consumptionIntFilePtr_() << consumptionInt_[i][reactionI] << tab;

            consumptionInt_[i][reactionI] = 0.0;
            productionInt_[i][reactionI] = 0.0;
        }

        consumptionFilePtr_() << nl;
        consumptionIntFilePtr_() << nl;
        productionFilePtr_() << nl;
        productionIntFilePtr_() << nl;
    }

    consumptionFilePtr_() << nl << nl;
    consumptionIntFilePtr_() << nl << nl;
    productionFilePtr_() << nl << nl;
    productionIntFilePtr_() << nl << nl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class chemistryType>
Foam::reactionsSensitivityAnalysis<chemistryType>::reactionsSensitivityAnalysis
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectFile(obr, name),
    name_(name),
    mesh_(refCast<const fvMesh>(obr)),
    active_(true),
    production_(0),
    consumption_(0),
    productionInt_(0),
    consumptionInt_(0),
    startTime_(0),
    endTime_(0),
    speciesNames_(),
    nReactions_(0),
    productionFilePtr_(),
    consumptionFilePtr_(),
    productionIntFilePtr_(),
    consumptionIntFilePtr_()
{
    read(dict);

    if (mesh_.nCells() != 1)
    {
        FatalErrorIn
        (
            "Foam::reactionsSensitivityAnalysis::"
            "reactionsSensitivityAnalysis()"
        )   << "Function object only applicable to single cell cases "
            << endl;
    }

    if (mesh_.foundObject<basicChemistryModel>("chemistryProperties"))
    {
        const chemistryType& chemistry = refCast<const chemistryType>
        (
            mesh_.lookupObject<basicChemistryModel>("chemistryProperties")
        );

        speciesNames_.setSize
        (
            chemistry.thermo().composition().species().size()
        );

        forAll(speciesNames_, i)
        {
            speciesNames_[i] = chemistry.thermo().composition().species()[i];
        }

        nReactions_ = chemistry.nReaction();

        if (production_.size() == 0)
        {
            production_.setSize(chemistry.nSpecie());
            consumption_.setSize(production_.size());
            productionInt_.setSize(production_.size());
            consumptionInt_.setSize(production_.size());

            forAll(production_, i)
            {
                production_[i].setSize(nReactions_, 0.0);
                consumption_[i].setSize(nReactions_, 0.0);
                productionInt_[i].setSize(nReactions_, 0.0);
                consumptionInt_[i].setSize(nReactions_, 0.0);
            }
        }
    }
    else
    {
        FatalErrorIn
        (
            "void Foam::reactionsSensitivityAnalysis::"
            "reactionsSensitivityAnalysis()"
        )   << " Not chemistry model found "
            << " The object available are : " << mesh_.names()
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class chemistryType>
Foam::reactionsSensitivityAnalysis<chemistryType>::
~reactionsSensitivityAnalysis()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::read
(
    const dictionary& dict
)
{
    createFiles();
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::execute()
{
    const basicChemistryModel& chemistry =
        mesh_.lookupObject<basicChemistryModel>("chemistryProperties");

    calculateSpeciesRR(chemistry);
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::end()
{
    // Do nothing - only valid on write
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::timeSet()
{
    // Do nothing
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::write()
{
    if (!active_)
    {
        return;
    }

    if (Pstream::master())
    {
        writeSpeciesRR();

        startTime_ = endTime_;
    }
}


// ************************************************************************* //
