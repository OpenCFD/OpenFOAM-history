/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "solidChemistryModel.H"
#include "reactingMixture.H"
#include "zeroGradientFvPatchFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CompType, class SolidThermo>
Foam::solidChemistryModel<CompType, SolidThermo>::
solidChemistryModel
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    chemistryModel<CompType, SolidThermo>(mesh, phaseName),
    reactingCells_(mesh.nCells(), true)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CompType, class SolidThermo>
Foam::solidChemistryModel<CompType, SolidThermo>::
~solidChemistryModel()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CompType, class SolidThermo>
void Foam::solidChemistryModel<CompType, SolidThermo>::setCellReacting
(
    const label cellI,
    const bool active
)
{
    reactingCells_[cellI] = active;
}

// ************************************************************************* //
