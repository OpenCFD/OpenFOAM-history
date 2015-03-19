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

Application
    Test-FieldMapper

Description
    Test for basic field mapping.

\*---------------------------------------------------------------------------*/

#include "Field.H"
#include "mapDistributeBase.H"
#include "argList.H"
#include "Time.H"
#include "distributedDirectFieldMapper.H"
#include "globalIndex.H"
#include "polyMesh.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

    #include "setRootCase.H"
    #include "createTime.H"


    // Test mapDistributeBase
    // ~~~~~~~~~~~~~~~~~~~~~~

    {
        const label nElems = 1;
        globalIndex globalNumbering(nElems);

        // Select some elements I want local. E.g. element 0 from every
        // processor
        labelList wantElements(Pstream::nProcs());
        forAll(wantElements, procI)
        {
            wantElements[procI] = globalNumbering.toGlobal(procI, 0);
        }

        List<Map<label> > compactMap;
        const mapDistributeBase distMap
        (
            globalNumbering,
            wantElements,
            compactMap
        );

        const distributedDirectFieldMapper mapper(wantElements, distMap);

        // Locally have one element
        Field<label> myData(1, 111*Pstream::myProcNo());
        Pout<< "myData:" << myData << endl;

        // Map by pulling in the data
        Field<label> allProcData(myData, mapper);
        Pout<< "allProData:" << allProcData << endl;
    }


    // Read a mesh and get all cellCentres onto the master
    {
        #include "createPolyMesh.H"

        // Get all cell centres on the master
        globalIndex globalNumbering(mesh.nCells());

        labelList wantElements;
        if (Pstream::master())
        {
            wantElements = identity(globalNumbering.size());
        }

        List<Map<label> > compactMap;
        const mapDistributeBase distMap
        (
            globalNumbering,
            wantElements,
            compactMap
        );

        const distributedDirectFieldMapper mapper(wantElements, distMap);

        Field<point> allCellCentres(mesh.cellCentres(), mapper);
        Pout<< "allCellCentres:" << allCellCentres << endl;
    }


    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
