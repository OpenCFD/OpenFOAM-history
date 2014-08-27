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
#include "fieldVisualisationBase.H"
#include "runTimePostProcessing.H"

// VTK includes
#include "vtkArrowSource.h"
#include "vtkColorTransferFunction.h"
#include "vtkGlyph3D.h"
#include "vtkLookupTable.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkScalarBarActor.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextProperty.h"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

namespace Foam
{
    template<>
    const char* NamedEnum<fieldVisualisationBase::colourByType, 2>::names[] =
    {
        "colour",
        "field"
    };
}

const Foam::NamedEnum<Foam::fieldVisualisationBase::colourByType, 2>
    Foam::fieldVisualisationBase::colourByTypeNames;


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

Foam::label Foam::fieldVisualisationBase::nComponents
(
    const word& fieldName
) const
{
    label result = -1;

    testFieldType<scalar>(fieldName, result);
    testFieldType<vector>(fieldName, result);
    testFieldType<sphericalTensor>(fieldName, result);
    testFieldType<symmTensor>(fieldName, result);
    testFieldType<tensor>(fieldName, result);

    return result;
}


void Foam::fieldVisualisationBase::setColourMap(vtkLookupTable* lut) const
{
    label nColours = 256;

    lut->SetNumberOfColors(nColours);

    vtkSmartPointer<vtkColorTransferFunction> ctf =
        vtkSmartPointer<vtkColorTransferFunction>::New();

    // rainbow
    ctf->SetColorSpaceToHSV();
    ctf->AddRGBPoint(0, 0, 0, 1);
    ctf->AddRGBPoint(0.5, 0, 1, 0);
    ctf->AddRGBPoint(1, 1, 0, 0);

    // blue-whit-red
//    ctf->SetColorSpaceToDiverging();
//    ctf->AddRGBPoint(0.0, 0.231373, 0.298039, 0.752941);
//    ctf->AddRGBPoint(0.5, 0.865003, 0.865003, 0.865003);
//    ctf->AddRGBPoint(1.0, 0.705882, 0.0156863, 0.14902);

    // fire
//    ctf->SetColorSpaceToRGB();
//    ctf->AddRGBPoint(0, 0, 0, 0);
//    ctf->AddRGBPoint(0.4, 0.901961, 0, 0);
//    ctf->AddRGBPoint(0.8, 0.901961, 0.901961, 0);
//    ctf->AddRGBPoint(1, 1, 1, 1);

    // greyscale
//    ctf->SetColorSpaceToRGB();
//    ctf->AddRGBPoint(0, 0, 0, 0);
//    ctf->AddRGBPoint(1, 1, 1, 1);

    for (label i = 0; i < nColours; i++)
    {
        double* c = ctf->GetColor(scalar(i)/scalar(nColours));
        lut->SetTableValue(i, c[0], c[1], c[2], 1.0);
    }
}


void Foam::fieldVisualisationBase::addScalarBar
(
    vtkRenderer* renderer,
    vtkLookupTable* lut
) const
{
    // add scalar bar legend
    if (!scalarBar_.visible_)
    {
        return;
    }

    vtkSmartPointer<vtkScalarBarActor> sbar =
        vtkSmartPointer<vtkScalarBarActor>::New();
    sbar->SetLookupTable(lut);
    sbar->SetTitle(scalarBar_.title_.c_str());
    sbar->SetNumberOfLabels(scalarBar_.numberOfLabels_);

    const vector& textColour = colours_["text"];

    sbar->GetTitleTextProperty()->SetColor
    (
        textColour[0],
        textColour[1],
        textColour[2]
    );
    sbar->GetTitleTextProperty()->SetFontSize(scalarBar_.fontSize_);
    sbar->GetTitleTextProperty()->ShadowOff();
    sbar->GetTitleTextProperty()->BoldOn();
    sbar->GetTitleTextProperty()->ItalicOff();
    sbar->GetLabelTextProperty()->SetColor
    (
        textColour[0],
        textColour[1],
        textColour[2]
    );
    sbar->GetLabelTextProperty()->SetFontSize(scalarBar_.fontSize_);
    sbar->GetLabelTextProperty()->ShadowOff();
    sbar->GetLabelTextProperty()->BoldOff();
    sbar->GetLabelTextProperty()->ItalicOff();
    sbar->SetLabelFormat(scalarBar_.labelFormat_.c_str());

    sbar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
    sbar->GetPositionCoordinate()->SetValue
    (
        scalarBar_.position_.first(),
        scalarBar_.position_.second()
    );

    if (scalarBar_.vertical_)
    {
        sbar->SetOrientationToVertical();
        sbar->SetWidth(0.1);
        sbar->SetHeight(0.75);
    }
    else
    {
        sbar->SetOrientationToHorizontal();
        sbar->SetWidth(0.75);
        sbar->SetHeight(0.1);
    }

//    sbar->DrawFrameOn();
//    sbar->DrawBackgroundOn();
//    sbar->UseOpacityOff();
//    sbar->VisibilityOff();
    sbar->VisibilityOn();

    renderer->AddActor(sbar);
}


void Foam::fieldVisualisationBase::setField
(
    vtkPolyDataMapper* mapper,
    vtkRenderer* renderer
) const
{
    mapper->InterpolateScalarsBeforeMappingOn();

    switch (colourBy_)
    {
        case cbColour:
        {
            mapper->ScalarVisibilityOff();
            break;
        }
        case cbField:
        {
            // create look-up table for colours
            vtkSmartPointer<vtkLookupTable> lut =
                vtkSmartPointer<vtkLookupTable>::New();
            setColourMap(lut);

            label nComponents = this->nComponents(fieldName_);

            if (nComponents < 0)
            {
                FatalErrorIn
                (
                    "void Foam::fieldVisualisationBase::setField"
                    "("
                        "vtkPolyDataMapper*, "
                        "vtkRenderer*"
                    ") const"
                )
                    << "Unable to determine number of components for field "
                    << fieldName_ << abort(FatalError);
            }

            // configure the model
            mapper->ColorByArrayComponent(fieldName_.c_str(), nComponents);
            mapper->SetScalarRange(range_.first(), range_.second());
            mapper->SetScalarModeToUsePointFieldData();
            mapper->SetColorModeToMapScalars();
            mapper->SetLookupTable(lut);
            mapper->ScalarVisibilityOn();

            // add the bar
            addScalarBar(renderer, lut);
            break;
        }
    }

    mapper->Modified();
}



void Foam::fieldVisualisationBase::addGlyphs
(
    vtkPolyData* data,
    vtkActor* actor,
    vtkRenderer* renderer
) const
{
    label nComponents = this->nComponents(fieldName_);

    if (nComponents < 0)
    {
        FatalErrorIn
        (
            "void Foam::fieldVisualisationBase::addGlyphs"
            "("
                "vtkPolyData*, "
                "vtkRenderer*"
            ") const"
        )
            << "Unable to determine number of components for field "
            << fieldName_ << abort(FatalError);
    }

    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    glyphMapper->SetInputConnection(glyph->GetOutputPort());

    bool ok = true;
    if (nComponents == 1)
    {
        vtkSmartPointer<vtkSphereSource> sphere =
            vtkSmartPointer<vtkSphereSource>::New();
//        sphere->SetPhiResolution(30);
//        sphere->SetThetaResolution(30);

        glyph->SetSourceConnection(sphere->GetOutputPort());
        glyph->SetInputData(data);
        glyph->SetScaleFactor(0.1);
        glyph->SetColorModeToColorByScalar();
//        glyph->ScalingOff();
        glyph->ScalingOn();
    }
    else if (nComponents == 3)
    {
        vtkSmartPointer<vtkArrowSource> arrow =
            vtkSmartPointer<vtkArrowSource>::New();
        arrow->SetTipResolution(10);
        arrow->SetTipRadius(0.1);
        arrow->SetTipLength(0.35);
        arrow->SetShaftResolution(10);
        arrow->SetShaftRadius(0.03);

        glyph->SetSourceConnection(arrow->GetOutputPort());
        glyph->SetInputData(data);
        glyph->SetScaleFactor(0.1);
        glyph->SetVectorModeToUseVector();
//        glyph->SetVectorModeToUseNormal();
        glyph->SetColorModeToColorByVector();
//        glyph->ScalingOff();
        glyph->ScalingOn();
    }
    else
    {
        WarningIn
        (
            "void Foam::fieldVisualisationBase::addGlyphs"
            "("
                "vtkPolyData*, "
                "vtkRenderer*"
            ") const"
        )
            << "Glyphs can only be added to " << pTraits<scalar>::typeName
            << " and " << pTraits<vector>::typeName << " fields. "
            << " Field " << fieldName_ << " has " << nComponents
            << " components" << endl;

        ok = false;
    }

    if (ok)
    {
        glyph->Update();
        glyphMapper->Update();

        setField(glyphMapper, renderer);

        actor->SetMapper(glyphMapper);

        renderer->AddActor(actor);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fieldVisualisationBase::fieldVisualisationBase
(
    const runTimePostProcessing& parent,
    const dictionary& dict,
    const HashTable<vector, word>& colours
)
:
    parent_(parent),
    colours_(colours),
    fieldName_(dict.lookup("fieldName")),
    colourBy_(cbColour),
    range_()
{
    colourBy_ = colourByTypeNames.read(dict.lookup("colourBy"));

    switch (colourBy_)
    {
        case cbColour:
        {
            break;
        }
        case cbField:
        {
            dict.lookup("range") >> range_;
            break;
        }
    }

    const dictionary& sbarDict = dict.subDict("scalarBar");
    sbarDict.lookup("visible") >> scalarBar_.visible_;
    if (scalarBar_.visible_)
    {
        sbarDict.lookup("vertical") >> scalarBar_.vertical_;
        sbarDict.lookup("position") >> scalarBar_.position_;
        sbarDict.lookup("title") >> scalarBar_.title_;
        sbarDict.lookup("fontSize") >> scalarBar_.fontSize_;
        sbarDict.lookup("labelFormat") >> scalarBar_.labelFormat_;
        sbarDict.lookup("numberOfLabels") >> scalarBar_.numberOfLabels_;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fieldVisualisationBase::~fieldVisualisationBase()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::HashTable<Foam::vector, Foam::word>&
Foam::fieldVisualisationBase::colours() const
{
    return colours_;
}


const Foam::word& Foam::fieldVisualisationBase::fieldName() const
{
    return fieldName_;
}


// ************************************************************************* //
