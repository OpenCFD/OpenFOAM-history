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
#include "vtkRenderWindow.h"
#include "vtkWindowToImageFilter.h"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::camera::initialise(vtkRenderer* renderer)
{
    // set the camera
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();

    if (parallelProjection_)
    {
        camera->ParallelProjectionOn();
    }
    else
    {
        camera->ParallelProjectionOff();
    }

    renderer->SetActiveCamera(camera);


    // initialise the extents
    if (minExtent_.valid())
    {
        clipBox_ = vtkSmartPointer<vtkCubeSource>::New();
        point min = minExtent_->value(currentFrameI_);
        point max = maxExtent_->value(currentFrameI_);

        clipBox_->SetXLength(max.x() - min.x());
        clipBox_->SetYLength(max.y() - min.y());
        clipBox_->SetZLength(max.z() - min.z());
        clipBox_->SetCenter
        (
            min.x() + 0.5*(max.x() - min.x()),
            min.y() + 0.5*(max.y() - min.y()),
            min.z() + 0.5*(max.z() - min.z())
        );

        clipMapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
        clipMapper_->SetInputConnection(clipBox_->GetOutputPort());

        clipActor_ = vtkSmartPointer<vtkActor>::New();
        clipActor_->VisibilityOff();
        clipActor_->SetMapper(clipMapper_);

        renderer->AddActor(clipActor_);
    }
}


void Foam::camera::setCamera(vtkRenderer* renderer) const
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
}


void Foam::camera::setExtents(vtkRenderer* renderer) const
{
    if (!minExtent_.valid())
    {
        renderer->ResetCamera();
        return;
    }

    point min = minExtent_->value(currentFrameI_);
    point max = maxExtent_->value(currentFrameI_);

    clipBox_->SetXLength(max.x() - min.x());
    clipBox_->SetYLength(max.y() - min.y());
    clipBox_->SetZLength(max.z() - min.z());
    clipBox_->SetCenter
    (
        min.x() + 0.5*(max.x() - min.x()),
        min.y() + 0.5*(max.y() - min.y()),
        min.z() + 0.5*(max.z() - min.z())
    );

    clipActor_->VisibilityOn();
    clipMapper_->Update();
    renderer->ResetCamera();
    clipActor_->VisibilityOff();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::camera::camera()
:
    position_(NULL),
    focalPoint_(NULL),
    up_(NULL),
    zoom_(NULL),
    viewAngle_(NULL),
    minExtent_(NULL),
    maxExtent_(NULL),
    parallelProjection_(true),
    nFrameTotal_(1),
    currentFrameI_(0),
    clipBox_(NULL)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::camera::~camera()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::camera::read(const dictionary& dict)
{
    position_.reset(DataEntry<vector>::New("position", dict).ptr());
    focalPoint_.reset(DataEntry<point>::New("focalPoint", dict).ptr());
    up_.reset(DataEntry<vector>::New("up", dict).ptr());

    dict.lookup("parallelProjection") >> parallelProjection_;

    if (dict.found("zoom"))
    {
        zoom_.reset(DataEntry<scalar>::New("zoom", dict).ptr());

        if (dict.found("viewAngle"))
        {
            viewAngle_.reset(DataEntry<scalar>::New("viewAngle", dict).ptr());
        }
        else
        {
            zoom_.reset(new Constant<scalar>("viewAngle", 35.0));
        }
    }
    else
    {
        zoom_.reset(new Constant<scalar>("zoom", 1.0));
    }

    if (dict.found("minExtent") && dict.found("maxExtent"))
    {
        Info<< "    Using user-defined extents" << endl;
        minExtent_.reset(DataEntry<point>::New("minExtent", dict).ptr());
        maxExtent_.reset(DataEntry<point>::New("maxExtent", dict).ptr());
    }
    else
    {
        Info<< "    Extents defined by geometry" << endl;
    }
}


bool Foam::camera::loop(vtkRenderer* renderer)
{
    if (currentFrameI_ < nFrameTotal_)
    {
        currentFrameI_++;

        setCamera(renderer);

        return true;
    }

    return false;
}


void Foam::camera::saveImage
(
    vtkRenderWindow* renderWindow,
    vtkRenderer* renderer,
    const fileName& prefix,
    const word& outputName
) const
{
    if (!renderWindow)
    {
        return;
    }

    setExtents(renderer);

    renderWindow->Render();

    renderer->GetActiveCamera()->Zoom(zoom_->value(currentFrameI_));

    // anti-aliasing -  recommendation that 5-6 is ok, but 10 doesn't appear
    // to be too expensive...
    renderWindow->SetAAFrames(10);

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
    fileName fName(prefix/outputName + Foam::name(currentFrameI_) + ".png");
    writer->SetFileName(fName.c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());

    Info<< "    Generating image: " << fName << endl;

    writer->Write();
}


// ************************************************************************* //
