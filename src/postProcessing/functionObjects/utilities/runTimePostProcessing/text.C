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
    const HashTable<vector, word>& colours
)
:
    geometryBase(parent, dict, colours),
    string_(dict.lookup("string")),
    position_(dict.lookup("position")),
    size_(readScalar(dict.lookup("size"))),
    colour_(dict.lookupOrDefault("colour", vector(-1, 0, 0))),
    bold_(readBool(dict.lookup("bold")))
{
    if (colour_[0] < 0)
    {
        colour_ = colours["text"];
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::text::~text()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::text::addGeometryToScene(const label frameI, vtkRenderer* renderer)
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
    actor->GetTextProperty()->SetColor
    (
        colour_[0],
        colour_[1],
        colour_[2]
    );
    actor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
    actor->GetPositionCoordinate()->SetValue
    (
        position_.first(),
        position_.second()
    );

    renderer->AddActor2D(actor);
}


void Foam::text::updateActors(const label frameI)
{
    // do nothing - all handled by addGeometryToScene
}


// ************************************************************************* //
