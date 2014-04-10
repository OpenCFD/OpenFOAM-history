/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
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

#include "forceCoeffs.H"
#include "dictionary.H"
#include "Time.H"
#include "Pstream.H"
#include "IOmanip.H"
#include "fvMesh.H"
#include "dimensionedTypes.H"
#include "volFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(forceCoeffs, 0);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::forceCoeffs::writeFileHeader(const label i)
{
    if (i == 0)
    {
        // force coeff data

        writeHeader(file(i), "Force coefficients");
        writeHeaderValue(file(i), "liftDir", liftDir_);
        writeHeaderValue(file(i), "dragDir", dragDir_);
        writeHeaderValue(file(i), "pitchAxis", pitchAxis_);
        writeHeaderValue(file(i), "magUInf", magUInf_);
        writeHeaderValue(file(i), "lRef", lRef_);
        writeHeaderValue(file(i), "Aref", Aref_);
        writeHeaderValue(file(i), "CofR", coordSys_.origin());
        writeCommented(file(i), "Time");
        writeTabbed(file(i), "Cm");
        writeTabbed(file(i), "Cd");
        writeTabbed(file(i), "Cl");
        writeTabbed(file(i), "Cl(f)");
        writeTabbed(file(i), "Cl(r)");
        file(i)
            << tab << "Cm" << tab << "Cd" << tab << "Cl" << tab << "Cl(f)"
            << tab << "Cl(r)";
    }
    else if (i == 1)
    {
        // bin coeff data

        writeHeader(file(i), "Force coefficient bins");
        writeHeaderValue(file(i), "bins", nBin_);
        writeHeaderValue(file(i), "start", binMin_);
        writeHeaderValue(file(i), "delta", binDx_);
        writeHeaderValue(file(i), "direction", binDir_);

        vectorField binPoints(nBin_);
        writeCommented(file(i), "x co-ords  :");
        forAll(binPoints, pointI)
        {
            binPoints[pointI] = (binMin_ + (pointI + 1)*binDx_)*binDir_;
            file(i) << tab << binPoints[pointI].x();
        }
        file(i) << nl;

        writeCommented(file(i), "y co-ords  :");
        forAll(binPoints, pointI)
        {
            file(i) << tab << binPoints[pointI].y();
        }
        file(i) << nl;

        writeCommented(file(i), "z co-ords  :");
        forAll(binPoints, pointI)
        {
            file(i) << tab << binPoints[pointI].z();
        }
        file(i) << nl;

        writeCommented(file(i), "Time");

        for (label j = 0; j < nBin_; j++)
        {
            const word jn('(' + Foam::name(j) + ')');
            writeTabbed(file(i), "Cm" + jn);
            writeTabbed(file(i), "Cd" + jn);
            writeTabbed(file(i), "Cl" + jn);
        }
    }
    else
    {
        FatalErrorIn("void Foam::forces::writeFileHeader(const label)")
            << "Unhandled file index: " << i
            << abort(FatalError);
    }

    file(i)<< endl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::forceCoeffs::forceCoeffs
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    forces(name, obr, dict, loadFromFiles, false),
    liftDir_(vector::zero),
    dragDir_(vector::zero),
    pitchAxis_(vector::zero),
    magUInf_(0.0),
    lRef_(0.0),
    Aref_(0.0)
{
    read(dict);

    Info<< endl;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::forceCoeffs::~forceCoeffs()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::forceCoeffs::read(const dictionary& dict)
{
    if (active_)
    {
        forces::read(dict);

        // Directions for lift and drag forces, and pitch moment
        dict.lookup("liftDir") >> liftDir_;
        dict.lookup("dragDir") >> dragDir_;
        dict.lookup("pitchAxis") >> pitchAxis_;

        // Free stream velocity magnitude
        dict.lookup("magUInf") >> magUInf_;

        // Reference length and area scales
        dict.lookup("lRef") >> lRef_;
        dict.lookup("Aref") >> Aref_;

        if (writeFields_)
        {
            const fvMesh& mesh = refCast<const fvMesh>(obr_);

            tmp<volVectorField> tforceCoeff
            (
                new volVectorField
                (
                    IOobject
                    (
                        fieldName("forceCoeff"),
                        mesh.time().timeName(),
                        mesh,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh,
                    dimensionedVector("0", dimless, vector::zero)
                )
            );

            const_cast<fvMesh&>(mesh).regIOobject::store(tforceCoeff.ptr());

            tmp<volVectorField> tmomentCoeff
            (
                new volVectorField
                (
                    IOobject
                    (
                        fieldName("momentCoeff"),
                        mesh.time().timeName(),
                        mesh,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh,
                    dimensionedVector("0", dimless, vector::zero)
                )
            );

            const_cast<fvMesh&>(mesh).regIOobject::store(tmomentCoeff.ptr());
        }
    }
}


void Foam::forceCoeffs::execute()
{
    forces::calcForcesMoment();

    if (!active_)
    {
        return;
    }

    scalar pDyn = 0.5*rhoRef_*magUInf_*magUInf_;

    if (Pstream::master())
    {
        functionObjectFile::write();

        Field<vector> totForce(force_[0] + force_[1] + force_[2]);
        Field<vector> totMoment(moment_[0] + moment_[1] + moment_[2]);

        List<Field<scalar> > coeffs(3);
        coeffs[0].setSize(nBin_);
        coeffs[1].setSize(nBin_);
        coeffs[2].setSize(nBin_);

        // lift, drag and moment
        coeffs[0] = (totForce & liftDir_)/(Aref_*pDyn);
        coeffs[1] = (totForce & dragDir_)/(Aref_*pDyn);
        coeffs[2] = (totMoment & pitchAxis_)/(Aref_*lRef_*pDyn);

        scalar Cl = sum(coeffs[0]);
        scalar Cd = sum(coeffs[1]);
        scalar Cm = sum(coeffs[2]);

        scalar Clf = Cl/2.0 + Cm;
        scalar Clr = Cl/2.0 - Cm;

        file(0)
            << obr_.time().value() << tab << Cm << tab  << Cd
            << tab << Cl << tab << Clf << tab << Clr << endl;

        Info(log_)<< type() << " " << name_ << " output:" << nl
            << "    Cm    = " << Cm << nl
            << "    Cd    = " << Cd << nl
            << "    Cl    = " << Cl << nl
            << "    Cl(f) = " << Clf << nl
            << "    Cl(r) = " << Clr << endl;

        if (nBin_ > 1)
        {
            if (binCumulative_)
            {
                for (label i = 1; i < coeffs[0].size(); i++)
                {
                    coeffs[0][i] += coeffs[0][i-1];
                    coeffs[1][i] += coeffs[1][i-1];
                    coeffs[2][i] += coeffs[2][i-1];
                }
            }

            file(1)<< obr_.time().value();

            forAll(coeffs[0], i)
            {
                file(1)
                    << tab << coeffs[2][i]
                    << tab << coeffs[1][i]
                    << tab << coeffs[0][i];
            }

            file(1) << endl;
        }
        Info(log_)<< endl;

        // write state information
        {
            dictionary propsDict;
            propsDict.add("Cm", Cm);
            propsDict.add("Cd", Cd);
            propsDict.add("Cl", Cl);
            propsDict.add("Cl(f)", Clf);
            propsDict.add("Cl(r)", Clr);
            setProperty("forceCoeffs", propsDict);
        }
    }

    if (writeFields_)
    {
        const volVectorField& force =
            obr_.lookupObject<volVectorField>(fieldName("force"));

        const volVectorField& moment =
            obr_.lookupObject<volVectorField>(fieldName("moment"));

        volVectorField& forceCoeff =
            const_cast<volVectorField&>
            (
                obr_.lookupObject<volVectorField>(fieldName("forceCoeff"))
            );

        volVectorField& momentCoeff =
            const_cast<volVectorField&>
            (
                obr_.lookupObject<volVectorField>(fieldName("momentCoeff"))
            );

        dimensionedScalar f0("f0", dimForce, Aref_*pDyn);
        dimensionedScalar m0("m0", dimForce*dimLength, Aref_*lRef_*pDyn);

        forceCoeff == force/f0;
        momentCoeff == moment/m0;
    }
}


void Foam::forceCoeffs::end()
{
    // Do nothing - only valid on write
}


void Foam::forceCoeffs::timeSet()
{
    // Do nothing - only valid on write
}


void Foam::forceCoeffs::write()
{
    if (!active_)
    {
        return;
    }

    if (writeFields_)
    {
        const volVectorField& forceCoeff =
            obr_.lookupObject<volVectorField>(fieldName("forceCoeff"));

        const volVectorField& momentCoeff =
            obr_.lookupObject<volVectorField>(fieldName("momentCoeff"));

        forceCoeff.write();
        momentCoeff.write();
    }
}


// ************************************************************************* //
