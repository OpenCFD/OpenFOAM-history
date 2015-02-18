/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenCFD Ltd
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
Foam::wordList
Foam::reactionsSensitivityAnalysis<chemistryType>::createFileNames
(
    const dictionary& dict
) const
{
    DynamicList<word> names(4);

    const word production("production");
    const word consumption("consumption");
    const word productionInt("productionInt");
    const word consumptionInt("consumptionInt");

    names.append(consumption);
    names.append(consumptionInt);
    names.append(production);
    names.append(productionInt);

    return names;
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::writeFileHeader
(
    const label i
)
{
    if (i == 0)
    {
        // consumption
        writeHeader(file(i), "consumption");
    }
    else if (i == 1)
    {
        // consumption Integral
        writeHeader(file(i), "consumptionInt");
    }
    else if (i == 2)
    {
        // production
        writeHeader(file(i), "production");
    }
    else if (i == 3)
    {
        // production integral between dumps
        writeHeader(file(i), "productionInt");
    }
    else
    {
        FatalErrorIn
        (
            "void Foam::reactionsSensitivityAnalysis::writeFileHeader"
            "("
            "   const label"
            ")"
        )
            << "Unhandled file index: " << i
            << abort(FatalError);
    }

    writeCommented(file(i), "Time");
    file(i) << tab;

    forAll (speciesNames_, k)
    {
        file(i) << tab << speciesNames_[k] << tab;
    }

    file(i) << nl << endl;
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

    forAll (production_, specieI)
    {
        forAll (production_[specieI], reactionI)
        {
            RR = basicChemistry.calculateRR(reactionI, specieI);

            if (RR[0] > 0.0)
            {
                production_[specieI][reactionI] = RR[0];
                productionInt_[specieI][reactionI] =+ dt*RR[0];
            }
            else if (RR[0] < 0.0)
            {
                consumption_[specieI][reactionI] = RR[0];
                consumptionInt_[specieI][reactionI] =+ dt*RR[0];
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

    file(0) << "time : " << mesh_.time().value() << tab << nl;
    file(0) << "delta T : " << mesh_.time().deltaT().value() << nl << nl;
    file(2) << "time : " << mesh_.time().value() << tab << nl;
    file(2) << "delta T : " << mesh_.time().deltaT().value() << nl << nl;

    file(1) << "start time : " << startTime_ << tab
            << "end time :" <<  endTime_ << nl;

    file(3) << "start time : " << startTime_ << tab
            << "end time :" <<  endTime_ << nl;

    for
    (
        label reactionI = 0; reactionI < nReactions_; reactionI++
    )
    {
        file(0) << reactionI << tab;
        file(1) << reactionI << tab;
        file(2) << reactionI << tab;
        file(3) << reactionI << tab;

        forAll (speciesNames_, i)
        {
            file(2) << production_[i][reactionI] << tab;
            file(0) << consumption_[i][reactionI] << tab;
            file(3) << productionInt_[i][reactionI] << tab;
            file(1) << consumptionInt_[i][reactionI] << tab;
            consumptionInt_[i][reactionI] = 0.0;
            productionInt_[i][reactionI] = 0.0;
        }
        file(0) << nl;
        file(1) << nl;
        file(2) << nl;
        file(3) << nl;
    }
    file(0) << nl << nl;
    file(1) << nl << nl;
    file(2) << nl << nl;
    file(3) << nl << nl;
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
    functionObjectFile(obr, name, createFileNames(dict)),
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
    nReactions_(0)
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

        forAll (speciesNames_, i)
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

            forAll (production_, i)
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
}


template<class chemistryType>
void Foam::reactionsSensitivityAnalysis<chemistryType>::execute()
{
    const basicChemistryModel& chemistry =
        mesh_.lookupObject<basicChemistryModel>
        (
            "chemistryProperties"
        );

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
        functionObjectFile::write();

        writeSpeciesRR();

        startTime_ = endTime_;

    }
}


// ************************************************************************* //
