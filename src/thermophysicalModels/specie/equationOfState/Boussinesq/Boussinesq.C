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

#include "Boussinesq.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
Foam::Boussinesq<Specie>::Boussinesq(Istream& is)
:
    Specie(is),
    rho0_(readScalar(is)),
    Tref_(readScalar(is)),
    beta_(readScalar(is))
{
    is.check
    (
        "Boussinesq<Specie>::"
        "Boussinesq(Istream& is)"
    );
}


template<class Specie>
Foam::Boussinesq<Specie>::Boussinesq
(
    const dictionary& dict
)
:
    Specie(dict),
    rho0_(readScalar(dict.subDict("equationOfState").lookup("rho0"))),
    Tref_(readScalar(dict.subDict("equationOfState").lookup("Tref"))),
    beta_(readScalar(dict.subDict("equationOfState").lookup("beta")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
void Foam::Boussinesq<Specie>::write(Ostream& os) const
{
    Specie::write(os);
    dictionary dict("equationOfState");
    dict.add("rho0", rho0_);
    dict.add("Tref", Tref_);
    dict.add("beta", beta_);

    os  << indent << dict.dictName() << dict;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Specie>
Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const Boussinesq<Specie>& pg
)
{
    os  << static_cast<const Specie&>(pg)
        << token::SPACE << pg.rho0_
        << token::SPACE << pg.Tref_
        << token::SPACE << pg.beta_;

    os.check
    (
        "Ostream& operator<<"
        "(Ostream& os, const Boussinesq<Specie>& st)"
    );
    return os;
}


// ************************************************************************* //
