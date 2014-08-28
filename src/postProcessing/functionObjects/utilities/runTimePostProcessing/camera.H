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
    Foam::camera

Description

SourceFiles
    camera.C

\*---------------------------------------------------------------------------*/

#ifndef camera_H
#define camera_H

// OpenFOAM includes
#include "dictionary.H"
#include "DataEntry.H"
#include "vector.H"
#include "point.H"
#include "boundBox.H"
#include "NamedEnum.H"

// VTK includes
#include "vtkSmartPointer.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class vtkRenderer;
class vtkRenderWindow;

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class camera Declaration
\*---------------------------------------------------------------------------*/

class camera
{
public:

    enum modeType{mtStatic, mtFlightPath};

    NamedEnum<modeType, 2> modeTypeNames_;


private:

    // Private data


    // Private Member Functions

        //- Disallow default bitwise copy construct
        camera(const camera&);

        //- Disallow default bitwise assignment
        void operator=(const camera&);


protected:

    // Protected data

        //- Mode
        modeType mode_;

        //- Position
        autoPtr<DataEntry<point> > position_;

        //- Focal point
        autoPtr<DataEntry<point> > focalPoint_;

        //- Up direction
        autoPtr<DataEntry<vector> > up_;

        //- Zoom level
        autoPtr<DataEntry<scalar> > zoom_;

        //- View angle
        autoPtr<DataEntry<scalar> > viewAngle_;

        //- Clipping box
        boundBox clipBox_;

        //- Parallel projection flag
        bool parallelProjection_;

        //- Number of frames
        label nFrameTotal_;

        //- Index of current frame
        label currentFrameI_;


    // Protected Member Functions

        void setCamera(vtkRenderer* rendere, const bool override) const;

        string frameIndexStr() const;


public:

    // Constructors

        //- Construct null
        camera();


    //- Destructor
    virtual ~camera();


    // Member Functions

        // Access

            //- Return the current frame index
            label frameIndex() const;

            void read(const dictionary& dict);

            void initialise(vtkRenderer* renderer);

            //- Main control loop
            bool loop(vtkRenderer* renderer);

            //- Flag to indicate whether to add objects to scene
            bool addObjects() const;

            //- Save image to file
            void saveImage
            (
                vtkRenderWindow* renderWindow,
                const fileName& prefix,
                const word& outputName
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
