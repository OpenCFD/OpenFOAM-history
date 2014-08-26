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
    Foam::runTimePostProcessing

Description

SourceFiles
    runTimePostProcessing.C

\*---------------------------------------------------------------------------*/

#ifndef runTimePostProcessing_H
#define runTimePostProcessing_H

#include "functionObjectState.H"
#include "objectRegistry.H"
#include "mapPolyMesh.H"
#include "PtrList.H"
#include "HashTable.H"
#include "vector.H"
#include "Tuple2.H"
#include "camera.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class vtkRenderer;
class vtkRenderWindow;

namespace Foam
{

class dictionary;
class pathline;
class surface;
class text;

/*---------------------------------------------------------------------------*\
                   Class runTimePostProcessing Declaration
\*---------------------------------------------------------------------------*/

class runTimePostProcessing
:
    public functionObjectState
{
private:

    // Private data

        // Output
        struct outputType
        {
            word name_;
            label width_;
            label height_;
        };

        //- Output instance
        outputType output_;

        //- Camera instance
        camera camera_;

        //- Colours
        HashTable<vector, word> colours_;

        //- List of lines
        PtrList<pathline> lines_;

        //- List of surfaces
        PtrList<surface> surfaces_;

        //- List of text
        PtrList<text> text_;


        // Private Member Functions

            //- Initialise the scene
            void initialiseScene(vtkRenderer* renderer);

            //- Save the image to file
            void saveImage(vtkRenderWindow* renderWindow) const;


protected:

    // Protected data

        //- Reference to the database
        const objectRegistry& obr_;

        //- on/off switch
        bool active_;


public:

    //- Runtime type information
    TypeName("runTimePostProcessing");


    // Constructors

        //- Construct from dictionary
        runTimePostProcessing
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );

    //- Desructor
    virtual ~runTimePostProcessing();


    // Member Functions

        virtual const objectRegistry& obr() const
        {
            return obr_;
        }

        //- Read the field min/max data
        virtual void read(const dictionary&);

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Write
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
