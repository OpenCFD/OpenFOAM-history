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
#include "camera.H"
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

template<>
const char* Foam::NamedEnum<Foam::camera::modeType, 2>::names[] =
{
    "static",
    "flightPath"
};

const Foam::NamedEnum<Foam::camera::modeType, 2> modeTypeNames_;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::camera::initialise(vtkRenderer* renderer)
{
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


void Foam::camera::setCamera(vtkRenderer* renderer, const bool override) const
{
    if (mode_ == mtFlightPath || override)
    {
        vtkCamera* camera = renderer->GetActiveCamera();

        if (!parallelProjection_)
        {
            camera->SetViewAngle(viewAngle_->value(currentFrameI_));
        }

        const vector up = up_->value(currentFrameI_);
        const vector position = position_->value(currentFrameI_);
        const point focalPoint = focalPoint_->value(currentFrameI_);

        camera->SetViewUp(up.x(), up.y(), up.z());
        camera->SetPosition(position.x(), position.y(), position.z());
        camera->SetFocalPoint(focalPoint.x(), focalPoint.y(), focalPoint.z());
        camera->Modified();
    }
}


Foam::string Foam::camera::frameIndexStr() const
{
    string str = Foam::name(currentFrameI_);
    str.insert(0, 4 - str.length(), '0');

    return str;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::camera::camera()
:
    mode_(mtStatic),
    position_(NULL),
    focalPoint_(NULL),
    up_(NULL),
    zoom_(NULL),
    viewAngle_(NULL),
    clipBox_(),
    parallelProjection_(true),
    nFrameTotal_(1),
    currentFrameI_(0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::camera::~camera()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::label Foam::camera::frameIndex() const
{
    return currentFrameI_;
}


void Foam::camera::read(const dictionary& dict)
{
    nFrameTotal_ = dict.lookupOrDefault("nFrameTotal", 1);
    currentFrameI_ = dict.lookupOrDefault("startFrameIndex", 0);
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
            FatalErrorIn("void Foam::camera::read(const dictionary&)")
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
        zoom_.reset(new Constant<scalar>("viewAngle", 35.0));
    }
}


bool Foam::camera::loop(vtkRenderer* renderer)
{
    // note: 1-based loop
    if (currentFrameI_ < nFrameTotal_)
    {
        currentFrameI_++;

        setCamera(renderer, false);

        return true;
    }

    return false;
}


bool Foam::camera::addObjects() const
{
    static bool initialised = false;

    if (!initialised)
    {
        initialised = true;
        return true;
    }

    return false;
}


void Foam::camera::saveImage
(
    vtkRenderWindow* renderWindow,
    const fileName& prefix,
    const word& outputName
) const
{
    if (!renderWindow)
    {
        return;
    }

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
    fileName fName(prefix/outputName + '.' + frameIndexStr() + ".png");
    writer->SetFileName(fName.c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());

    Info<< "    Generating image: " << fName << endl;

    writer->Write();
}


// ************************************************************************* //
