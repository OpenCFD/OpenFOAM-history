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

#include "pointSmoother.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(pointSmoother, 0);
    defineRunTimeSelectionTable(pointSmoother, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::pointSmoother::pointSmoother
(
    const dictionary& dict,
    const polyMesh& mesh
)
:
    mesh_(mesh)
{
    forAll(mesh.boundaryMesh(), patchI)
    {
        const polyPatch& pp(mesh.boundaryMesh()[patchI]);

        if (isA<processorPolyPatch>(pp))
        {
            processorPatchIDs_.insert(patchI);
        }
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::pointSmoother>
Foam::pointSmoother::New
(
    const dictionary& dict,
    const polyMesh& mesh
)
{
    word pointSmootherType(dict.lookup(typeName));

    Info<< "Selecting pointSmoother type " << pointSmootherType << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(pointSmootherType);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorIn("pointSmoother::New")
            << "Unknown " << typeName << " type "
            << pointSmootherType << endl << endl
            << "Valid " << typeName << " types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return cstrIter()(dict, mesh);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::pointSmoother::~pointSmoother()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

bool Foam::pointSmoother::isInternalOrProcessorFace(const label& faceI) const
{
    if (mesh().isInternalFace(faceI))
    {
        return true;
    }

    if (processorPatchIDs_[mesh().boundaryMesh().whichPatch(faceI)])
    {
        return true;
    }

    return false;
}



// ************************************************************************* //
