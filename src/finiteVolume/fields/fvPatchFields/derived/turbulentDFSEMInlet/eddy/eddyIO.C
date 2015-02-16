/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenCFD Ltd.
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

#include "eddy.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::eddy::eddy(Istream& is)
:
    position_(is),
    sigma_(is),
    alphaAve_(readScalar(is)),
    Rpg_(is),
    c1_(readScalar(is))
{
    is.check("Foam::eddy::eddy(Foam::Istream&)");
}


// * * * * * * * * * * * * * * * * * Operators * * * * * * * * * * * * * * * //

void Foam::eddy::operator=(const eddy& e)
{
    position_ = e.position_;
    sigma_ = e.sigma_;
    alphaAve_ = e.alphaAve_;
    Rpg_ = e.Rpg_;
    c1_ = e.c1_;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>(Istream& is, eddy& e)
{
    is.check
    (
        "Foam::Istream& Foam::operator>>"
        "(Foam::Istream&, Foam::eddy&)"
    );

    is  >> e.position_
        >> e.sigma_
        >> e.alphaAve_
        >> e.Rpg_
        >> e.c1_;

    is.check
    (
        "Foam::Istream& Foam::operator>>"
        "(Foam::Istream&, Foam::eddy&)"
    );

    return is;
}


Foam::Ostream& Foam::operator<<(Ostream& os, const eddy& e)
{
    os.check
    (
        "Foam::Ostream& Foam::operator<<"
        "(Foam::Ostream&, const Foam::eddy&)"
    );

    os  << e.position_
        << e.sigma_
        << e.alphaAve_
        << e.Rpg_
        << e.c1_;

    os.check
    (
        "Foam::Ostream& Foam::operator<<"
        "(Foam::Ostream&, const Foam::eddy&)"
    );

    return os;
}


// ************************************************************************* //
