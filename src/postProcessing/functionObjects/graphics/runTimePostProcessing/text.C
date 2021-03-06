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

\*---------------------------------------------------------------------------*/

// OpenFOAM includes
#include "text.H"
#include "runTimePostProcessing.H"

// VTK includes
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::text::text
(
    const runTimePostProcessing& parent,
    const dictionary& dict,
    const HashPtrTable<DataEntry<vector>, word>& colours
)
:
    geometryBase(parent, dict, colours),
    string_(dict.lookup("string")),
    position_(dict.lookup("position")),
    size_(readScalar(dict.lookup("size"))),
    colour_(NULL),
    bold_(readBool(dict.lookup("bold")))
{
    if (dict.found("colour"))
    {
        colour_.reset(DataEntry<vector>::New("colour", dict).ptr());
    }
    else
    {
        colour_.reset(colours["text"]->clone().ptr());
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::text::~text()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::text::addGeometryToScene
(
    const scalar position,
    vtkRenderer* renderer
)
{
    if (!visible_)
    {
        return;
    }

    vtkSmartPointer<vtkTextActor> actor = vtkSmartPointer<vtkTextActor>::New();

    actor->SetInput(string_.c_str());
    actor->GetTextProperty()->SetFontFamilyToArial();
    actor->GetTextProperty()->SetFontSize(size_);
    actor->GetTextProperty()->SetJustificationToLeft();
    actor->GetTextProperty()->SetVerticalJustificationToBottom();
    actor->GetTextProperty()->SetBold(bold_);

    const vector colour = colour_->value(position);
    actor->GetTextProperty()->SetColor(colour[0], colour[1], colour[2]);
    actor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
    actor->GetPositionCoordinate()->SetValue
    (
        position_.first(),
        position_.second()
    );

    renderer->AddActor2D(actor);
}


void Foam::text::updateActors(const scalar position)
{
    // do nothing - all handled by addGeometryToScene
}


// ************************************************************************* //
