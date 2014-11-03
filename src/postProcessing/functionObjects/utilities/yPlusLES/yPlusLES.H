/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2013-2014 OpenFOAM Foundation
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
    Foam::yPlusLES

Group
    grpUtilitiesFunctionObjects

Description
    This function object evaluates and outputs turbulence y+ for LES models.
    The field is stored on the mesh database so that it can be retrieved and
    used for other applications.

    Example of function object specification to calculate the y+ (LES):
    \verbatim
    yPlusLES1
    {
        type        yPlusLES;
        functionObjectLibs ("libutilityFunctionObjects.so");
        ...
    }
    \endverbatim

    \heading Function object usage
    \table
        Property     | Description             | Required    | Default value
        type         | Type name: yPlusLES     | yes         |
        phiName      | Name of flux field      | no          | phi
        UName        | Name of velocity field  | no          | U
        resultName   | Name of y+ field        | no          | <function name>
        log          | Log to standard output  | no          | yes
    \endtable


SourceFiles
    yPlusLES.C
    IOyPlusLES.H

\*---------------------------------------------------------------------------*/

#ifndef yPlusLES_H
#define yPlusLES_H

#include "functionObjectFile.H"
#include "volFieldsFwd.H"
#include "Switch.H"
#include "OFstream.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class fvMesh;

/*---------------------------------------------------------------------------*\
                          Class yPlusLES Declaration
\*---------------------------------------------------------------------------*/

class yPlusLES
:
    public functionObjectFile
{
    // Private data

        //- Name of this set of yPlusLES objects
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Name of mass/volume flux field (optional, default = phi)
        word phiName_;

        //- Name of velocity field
        word UName_;

        //- Result name
        word resultName_;

        //- Switch to send output to Info as well as to file
        Switch log_;


    // Private Member Functions

        //- File header information
        virtual void writeFileHeader(const label i);

        //- Calculate incompressible form of y+
        void calcIncompressibleYPlus
        (
            const fvMesh& mesh,
            const volVectorField& U,
            volScalarField& yPlus
        );

        //- Calculate compressible form of y+
        void calcCompressibleYPlus
        (
            const fvMesh& mesh,
            const volVectorField& U,
            volScalarField& yPlus
        );

        //- Disallow default bitwise copy construct
        yPlusLES(const yPlusLES&);

        //- Disallow default bitwise assignment
        void operator=(const yPlusLES&);


public:

    //- Runtime type information
    TypeName("yPlusLES");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        yPlusLES
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    //- Destructor
    virtual ~yPlusLES();


    // Member Functions

        //- Return name of the set of yPlusLES
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the yPlusLES data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Calculate the yPlusLES and write
        virtual void write();

        //- Update for changes of mesh
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh
        virtual void movePoints(const polyMesh&)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
