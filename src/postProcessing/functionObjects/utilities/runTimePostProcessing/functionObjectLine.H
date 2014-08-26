/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd
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
    Foam::functionObjectLine

Description

SourceFiles
    functionObjectLine.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjectLine_H
#define functionObjectLine_H

#include "pathline.H"
#include "fieldVisualisationBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class functionObjectLine Declaration
\*---------------------------------------------------------------------------*/

class functionObjectLine
:
    public pathline,
    public fieldVisualisationBase
{
private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        functionObjectLine(const functionObjectLine&);

        //- Disallow default bitwise assignment
        void operator=(const functionObjectLine&);


protected:

    // Protected data

        //- Name of function object result to render
        word functionObject_;

        //- Name of field
        word fieldName_;


    // Protected Member Functions



public:

    //- Run-time type information
    TypeName("line");


    // Constructors

        //- Construct from dictionary
        functionObjectLine
        (
            const runTimePostProcessing& parent,
            const dictionary& dict,
            const HashTable<vector, word>& colours
        );


    //- Destructor
    virtual ~functionObjectLine();


    // Member Functions

        //- Add tube(s) to scene
        virtual void addToScene(vtkRenderer* renderer);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
