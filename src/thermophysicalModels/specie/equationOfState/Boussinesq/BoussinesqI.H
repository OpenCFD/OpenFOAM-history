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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
inline Foam::Boussinesq<Specie>::Boussinesq
(
    const Specie& sp,  const scalar rho0, const scalar Tref, const scalar beta
)
:
    Specie(sp),
    rho0_(rho0),
    Tref_(Tref),
    beta_(beta)
{}


template<class Specie>
inline Foam::Boussinesq<Specie>::Boussinesq
(
    const Boussinesq& ipg
)
:
    Specie(ipg),
    rho0_(ipg.rho0_),
    Tref_(ipg.Tref_),
    beta_(ipg.beta_)
{}


template<class Specie>
inline Foam::Boussinesq<Specie>::Boussinesq
(
    const word& name,
    const Boussinesq<Specie>& ipg
)
:
    Specie(name, ipg),
    rho0_(ipg.rho0_),
    Tref_(ipg.Tref_),
    beta_(ipg.beta_)
{}


template<class Specie>
inline Foam::autoPtr<Foam::Boussinesq<Specie> >
Foam::Boussinesq<Specie>::clone() const
{
    return autoPtr<Boussinesq<Specie> >
    (
        new Boussinesq<Specie>(*this)
    );
}


template<class Specie>
inline Foam::autoPtr<Foam::Boussinesq<Specie> >
Foam::Boussinesq<Specie>::New
(
    Istream& is
)
{
    return autoPtr<Boussinesq<Specie> >
    (
        new Boussinesq<Specie>(is)
    );
}


template<class Specie>
inline Foam::autoPtr<Foam::Boussinesq<Specie> >
Foam::Boussinesq<Specie>::New
(
    const dictionary& dict
)
{
    return autoPtr<Boussinesq<Specie> >
    (
        new Boussinesq<Specie>(dict)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::scalar Foam::Boussinesq<Specie>::rho
(
    scalar p,
    scalar T
) const
{
    return rho0_ - beta_*(T - Tref_);
}


template<class Specie>
inline Foam::scalar Foam::Boussinesq<Specie>::psi
(
    scalar,
    scalar T
) const
{
    return 0.0;
}


template<class Specie>
inline Foam::scalar Foam::Boussinesq<Specie>::Z
(
    scalar,
    scalar
) const
{
    return 0.0;
}


template<class Specie>
inline Foam::scalar Foam::Boussinesq<Specie>::cpMcv
(
    scalar,
    scalar
) const
{
    return RR;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::Boussinesq<Specie>&
Foam::Boussinesq<Specie>::operator=
(
    const Boussinesq<Specie>& ipg
)
{
    Specie::operator=(ipg);

    rho0_ = ipg.rho0_;
    Tref_ = ipg.Tref_;
    beta_ = ipg.beta_;

    return *this;
}


template<class Specie>
inline void Foam::Boussinesq<Specie>::operator+=
(
    const Boussinesq<Specie>& ipg
)
{
    scalar molr1 = this->nMoles();
    Specie::operator+=(ipg);
    molr1 /= this->nMoles();
    scalar molr2 = ipg.nMoles()/this->nMoles();

    rho0_ = molr1*rho0_ + molr2*ipg.rho0_;
    Tref_ = molr1*Tref_ + molr2*ipg.Tref_;
    beta_ = molr1*beta_ + molr2*ipg.beta_;
}


template<class Specie>
inline void Foam::Boussinesq<Specie>::operator-=
(
    const Boussinesq<Specie>& ipg
)
{
    Specie::operator-=(ipg);
    rho0_ = ipg.rho0_;
    Tref_ = ipg.Tref_;
    beta_ = ipg.beta_;
}


template<class Specie>
inline void Foam::Boussinesq<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::Boussinesq<Specie> Foam::operator+
(
    const Boussinesq<Specie>& ipg1,
    const Boussinesq<Specie>& ipg2
)
{
    scalar nMoles = ipg1.nMoles() + ipg2.nMoles();
    scalar molr1 = ipg1.nMoles()/nMoles;
    scalar molr2 = ipg2.nMoles()/nMoles;

    return Boussinesq<Specie>
    (
        static_cast<const Specie&>(ipg1)
      + static_cast<const Specie&>(ipg2),
        molr1*ipg1.rho0_ + molr2*ipg2.rho0_,
        molr1*ipg1.Tref_ + molr2*ipg2.Tref_,
        molr1*ipg1.beta_ + molr2*ipg2.beta_
    );
}


template<class Specie>
inline Foam::Boussinesq<Specie> Foam::operator-
(
    const Boussinesq<Specie>& ipg1,
    const Boussinesq<Specie>& ipg2
)
{
    return Boussinesq<Specie>
    (
        static_cast<const Specie&>(ipg1)
      - static_cast<const Specie&>(ipg2),
        ipg1.rho0_ - ipg2.rho0_,
        ipg1.Tref_ - ipg2.Tref_,
        ipg1.beta_ - ipg2.beta_
    );
}


template<class Specie>
inline Foam::Boussinesq<Specie> Foam::operator*
(
    const scalar s,
    const Boussinesq<Specie>& ipg
)
{
    return Boussinesq<Specie>
    (
        s*static_cast<const Specie&>(ipg),
        ipg.rho0_,
        ipg.Tref_,
        ipg.beta_
    );
}


template<class Specie>
inline Foam::Boussinesq<Specie> Foam::operator==
(
    const Boussinesq<Specie>& pg1,
    const Boussinesq<Specie>& pg2
)
{
    return pg2 - pg1;
}


// ************************************************************************* //
