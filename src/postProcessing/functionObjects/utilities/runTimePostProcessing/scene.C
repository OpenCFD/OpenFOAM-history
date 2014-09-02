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
#include "scene.H"
#include "Constant.H"

// VTK includes
#include "vtkCamera.h"
#include "vtkCubeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkPNGWriter.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkWindowToImageFilter.h"

#include "vtkLightKit.h"

template<>
const char* Foam::NamedEnum<Foam::scene::modeType, 2>::names[] =
{
    "static",
    "flightPath"
};

const Foam::NamedEnum<Foam::scene::modeType, 2> modeTypeNames_;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::scene::readCamera(const dictionary& dict)
{
    nFrameTotal_ = dict.lookupOrDefault("nFrameTotal", 1);
    startFrameI_ = dict.lookupOrDefault("startFrameIndex", 0);
    currentFrameI_ = startFrameI_;
    dict.lookup("parallelProjection") >> parallelProjection_;

    mode_ = modeTypeNames_.read(dict.lookup("mode"));

    word coeffsName = modeTypeNames_[mode_] + word("Coeffs");
    const dictionary& coeffs = dict.subDict(coeffsName);

    switch (mode_)
    {
        case mtStatic:
        {
            clipBox_ = boundBox(coeffs.lookup("clipBox"));
            const vector lookDir(vector(coeffs.lookup("lookDir")));
            position_.reset(new Constant<point>("position", -lookDir));
            const vector focalPoint(coeffs.lookup("focalPoint"));
            focalPoint_.reset(new Constant<point>("focalPoint", focalPoint));
            const vector up(coeffs.lookup("up"));
            up_.reset(new Constant<point>("up", up));
            break;
        }
        case mtFlightPath:
        {
            position_.reset(DataEntry<vector>::New("position", coeffs).ptr());
            focalPoint_.reset
            (
                DataEntry<point>::New("focalPoint", coeffs).ptr()
            );
            up_.reset(DataEntry<vector>::New("up", coeffs).ptr());
            break;
        }
        default:
        {
            FatalErrorIn("void Foam::scene::read(const dictionary&)")
                << "Unhandled enumeration " << modeTypeNames_[mode_]
                << abort(FatalError);
        }
    }

    if (dict.found("zoom"))
    {
        zoom_.reset(DataEntry<scalar>::New("zoom", dict).ptr());
    }
    else
    {
        zoom_.reset(new Constant<scalar>("zoom", 1.0));
    }

    if (dict.found("viewAngle"))
    {
        viewAngle_.reset(DataEntry<scalar>::New("viewAngle", dict).ptr());
    }
    else
    {
        viewAngle_.reset(new Constant<scalar>("viewAngle", 35.0));
    }
}


void Foam::scene::readColours(const dictionary& dict)
{
    colours_.insert
    (
        "background",
        DataEntry<vector>::New("background", dict).ptr()
    );

    if (dict.found("background2"))
    {
        colours_.insert
        (
            "background2",
            DataEntry<vector>::New("background2", dict).ptr()
        );
    }
    else
    {
        colours_.insert
        (
            "background2",
            DataEntry<vector>::New("background", dict).ptr()
        );
    }
    colours_.insert("text", DataEntry<vector>::New("text", dict).ptr());
    colours_.insert("surface", DataEntry<vector>::New("surface", dict).ptr());
    colours_.insert("edge", DataEntry<vector>::New("edge", dict).ptr());
    colours_.insert("line", DataEntry<vector>::New("line", dict).ptr());
}


void Foam::scene::initialise(vtkRenderer* renderer, const word& outputName)
{
    outputName_ = outputName;

    // set the background
    renderer->GradientBackgroundOn();
    const vector backgroundColour = colours_["background"]->value(position());
    const vector backgroundColour2 = colours_["background2"]->value(position());
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

    // depth peeling
    renderer->SetUseDepthPeeling(true);
    renderer->SetMaximumNumberOfPeels(4);
    renderer->SetOcclusionRatio(0);

    // set the camera
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetParallelProjection(parallelProjection_);
    renderer->SetActiveCamera(camera);

    setCamera(renderer, true);

    // initialise the extents
    if (mode_ == mtStatic)
    {
        const point& min = clipBox_.min();
        const point& max = clipBox_.max();
        vtkSmartPointer<vtkCubeSource> clipBox =
            vtkSmartPointer<vtkCubeSource>::New();
        clipBox->SetXLength(max.x() - min.x());
        clipBox->SetYLength(max.y() - min.y());
        clipBox->SetZLength(max.z() - min.z());
        clipBox->SetCenter
        (
            min.x() + 0.5*(max.x() - min.x()),
            min.y() + 0.5*(max.y() - min.y()),
            min.z() + 0.5*(max.z() - min.z())
        );
        vtkSmartPointer<vtkPolyDataMapper> clipMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        clipMapper->SetInputConnection(clipBox->GetOutputPort());

        vtkSmartPointer<vtkActor> clipActor = vtkSmartPointer<vtkActor>::New();
        clipActor->SetMapper(clipMapper);
        clipActor->VisibilityOn();
        renderer->AddActor(clipActor);

        renderer->ResetCamera();

        clipActor->VisibilityOff();
    }
}


void Foam::scene::setCamera(vtkRenderer* renderer, const bool override) const
{
    if (mode_ == mtFlightPath || override)
    {
        vtkCamera* camera = renderer->GetActiveCamera();

        if (!parallelProjection_)
        {
            camera->SetViewAngle(viewAngle_->value(position()));
        }

        const vector up = up_->value(position());
        const vector pos = position_->value(position());
        const point focalPoint = focalPoint_->value(position());

        camera->SetViewUp(up.x(), up.y(), up.z());
        camera->SetPosition(pos.x(), pos.y(), pos.z());
        camera->SetFocalPoint(focalPoint.x(), focalPoint.y(), focalPoint.z());
        camera->Modified();

        vtkSmartPointer<vtkLightKit> lightKit =
            vtkSmartPointer<vtkLightKit>::New();
        lightKit->AddLightsToRenderer(renderer);
    }
}


Foam::string Foam::scene::frameIndexStr() const
{
    string str = Foam::name(currentFrameI_);
    str.insert(0, 4 - str.length(), '0');

    return str;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::scene::scene(const objectRegistry& obr, const word& name)
:
    obr_(obr),
    name_(name),
    colours_(),
    mode_(mtStatic),
    position_(NULL),
    focalPoint_(NULL),
    up_(NULL),
    zoom_(NULL),
    viewAngle_(NULL),
    clipBox_(),
    parallelProjection_(true),
    nFrameTotal_(1),
    startFrameI_(0),
    currentFrameI_(0),
    outputName_("unknown")
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::scene::~scene()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::HashPtrTable<Foam::DataEntry<Foam::vector>, Foam::word>&
Foam::scene::colours() const
{
    return colours_;
}


Foam::label Foam::scene::frameIndex() const
{
    return currentFrameI_;
}


Foam::scalar Foam::scene::position() const
{
    return scalar(currentFrameI_)/scalar(nFrameTotal_);
}


void Foam::scene::read(const dictionary& dict)
{
    readCamera(dict.subDict("camera"));
    readColours(dict.subDict("colours"));
}


bool Foam::scene::loop(vtkRenderer* renderer)
{
    static bool initialised = false;

    setCamera(renderer, false);

    if (!initialised)
    {
        initialised = true;
        return true;
    }

    // save image from last iteration
    saveImage(renderer->GetRenderWindow());

    currentFrameI_++;

    if (currentFrameI_ < nFrameTotal_ + startFrameI_)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Foam::scene::saveImage(vtkRenderWindow* renderWindow) const
{
    if (!renderWindow)
    {
        return;
    }

    fileName prefix("postProcessing"/name_/obr_.time().timeName());
    mkDir(prefix);

    renderWindow->Render();

    // set-up off-screen rendering
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
        vtkSmartPointer<vtkWindowToImageFilter>::New();

    windowToImageFilter->SetInput(renderWindow);

//    add alpha channel for transparency
//    windowToImageFilter->SetInputBufferTypeToRGBA();
    windowToImageFilter->SetInputBufferTypeToRGB();

//    windowToImageFilter->ReadFrontBufferOff();
    windowToImageFilter->Update();

    // save the image
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    fileName fName(prefix/outputName_ + '.' + frameIndexStr() + ".png");
    writer->SetFileName(fName.c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());

    Info<< "    Generating image: " << fName << endl;

    writer->Write();
}


// ************************************************************************* //
