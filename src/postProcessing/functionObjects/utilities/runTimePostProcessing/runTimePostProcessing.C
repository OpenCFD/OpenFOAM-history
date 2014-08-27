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

\*---------------------------------------------------------------------------*/

// OpenFOAM includes
#include "runTimePostProcessing.H"
#include "dictionary.H"
#include "pathline.H"
#include "surface.H"
#include "text.H"
#include "Time.H"

// VTK includes
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(runTimePostProcessing, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::runTimePostProcessing::initialiseScene(vtkRenderer* renderer)
{
    // set the background
    renderer->GradientBackgroundOn();
    const vector& backgroundColour = colours_["background"];
    const vector& backgroundColour2 = colours_["background2"];
    renderer->SetBackground
    (
        backgroundColour.x(),
        backgroundColour.y(),
        backgroundColour.z()
    );
    renderer->SetBackground2
    (
        backgroundColour2.x(),
        backgroundColour2.y(),
        backgroundColour2.z()
    );

    camera_.initialise(renderer);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::runTimePostProcessing::runTimePostProcessing
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    functionObjectState(obr, name),
    camera_(),
    colours_(),
    lines_(),
    surfaces_(),
    text_(),
    obr_(obr),
    active_(true)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::runTimePostProcessing::~runTimePostProcessing()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::runTimePostProcessing::read(const dictionary& dict)
{
    Info<< type() << " " << name_ << ": reading post-processing data" << endl;

    const dictionary& colourDict = dict.subDict("colours");
    vector backgroundColour(colourDict.lookup("background"));
    vector backgroundColour2
    (
        colourDict.lookupOrDefault("background2", backgroundColour)
    );
    vector textColour(colourDict.lookup("text"));
    vector surfaceColour(colourDict.lookup("surface"));
    vector edgeColour(colourDict.lookup("edge"));
    vector lineColour(colourDict.lookup("line"));

    colours_.insert("background", backgroundColour);
    colours_.insert("background2", backgroundColour2);
    colours_.insert("text", textColour);
    colours_.insert("surface", surfaceColour);
    colours_.insert("edge", edgeColour);
    colours_.insert("line", lineColour);

    const dictionary& outputDict = dict.subDict("output");
    outputDict.lookup("name") >> output_.name_;
    outputDict.lookup("width") >> output_.width_;
    outputDict.lookup("height") >> output_.height_;


    label i = 0;

    const dictionary& linesDict = dict.subDict("lines");
    label nLines = linesDict.toc().size();
    lines_.setSize(nLines);
    forAllConstIter(dictionary, linesDict, iter)
    {
        if (!iter().isDict())
        {
            FatalIOErrorIn
            (
                "void Foam::runTimePostProcessing::read(const dictionary&)",
                linesDict
            )
                << "lines must be specified in dictionary format"
                << exit(FatalIOError);
        }

        const dictionary& lineDict(iter().dict());
        word lineType = lineDict.lookup("type");

        lines_.set
        (
            i++,
            pathline::New(*this, iter().dict(), colours_, lineType)
        );
    }

    const dictionary& surfacesDict = dict.subDict("surfaces");
    label nSurfaces = surfacesDict.toc().size();
    surfaces_.setSize(nSurfaces);
    i = 0;
    forAllConstIter(dictionary, surfacesDict, iter)
    {
        if (!iter().isDict())
        {
            FatalIOErrorIn
            (
                "void Foam::runTimePostProcessing::read(const dictionary&)",
                surfacesDict
            )
                << "surfaces must be specified in dictionary format"
                << exit(FatalIOError);
        }

        const dictionary& surfDict(iter().dict());
        word surfaceType = surfDict.lookup("type");

        surfaces_.set
        (
            i++,
            surface::New(*this, iter().dict(), colours_, surfaceType)
        );
    }

    const dictionary& textDict = dict.subDict("text");
    label nText = textDict.toc().size();
    text_.setSize(nText);
    i = 0;
    forAllConstIter(dictionary, textDict, iter)
    {
        if (!iter().isDict())
        {
            FatalIOErrorIn
            (
                "void Foam::runTimePostProcessing::read(const dictionary&)",
                textDict
            )
                << "text must be specified in dictionary format"
                << exit(FatalIOError);
        }

        text_.set
        (
            i++,
            new text(*this, iter().dict(), colours_)
        );
    }

    camera_.read(dict.subDict("camera"));
}


void Foam::runTimePostProcessing::execute()
{
    // do nothing
}


void Foam::runTimePostProcessing::end()
{
    // do nothing
}


void Foam::runTimePostProcessing::timeSet()
{
    // do nothing
}


void Foam::runTimePostProcessing::write()
{
    if (!Pstream::master())
    {
        return;
    }

    Info<< type() << " " << name_ <<  " output:" << nl
        << "    Constructing scene" << endl;

    // initialise render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->OffScreenRenderingOn();
    renderWindow->SetSize(output_.width_, output_.height_);
    renderWindow->SetAAFrames(10);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    initialiseScene(renderer);
    renderWindow->AddRenderer(renderer);

    fileName prefix("postProcessing"/name_/obr_.time().timeName());
    mkDir(prefix);

    while (camera_.loop(renderer))
    {
        // add the text
        forAll(text_, i)
        {
            text_[i].addGeometryToScene(camera_.frameIndex(), renderer);
        }

        if (camera_.addObjects())
        {
            // add the lines
            forAll(lines_, i)
            {
                lines_[i].addGeometryToScene(camera_.frameIndex(), renderer);
            }

            // add the surfaces
            forAll(surfaces_, i)
            {
                surfaces_[i].addGeometryToScene(camera_.frameIndex(), renderer);
            }
        }

        // add the lines
        forAll(lines_, i)
        {
            lines_[i].updateActors(camera_.frameIndex());
        }

        // add the surfaces
        forAll(surfaces_, i)
        {
            surfaces_[i].updateActors(camera_.frameIndex());
        }

        // saveimage to file
        camera_.saveImage(renderWindow, prefix, output_.name_);
    }
}


// ************************************************************************* //
