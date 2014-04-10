/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2014 OpenFOAM Foundation
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

#include "parLagrangianRedistributor.H"
#include "Time.H"
#include "IOobjectList.H"
#include "mapDistributePolyMesh.H"
#include "cloud.H"
#include "CompactIOField.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Container>
Foam::wordList Foam::parLagrangianRedistributor::filterObjects
(
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
)
{
    const word fieldClassName(Container::typeName);

    // Parallel synchronise
    wordList fieldNames(objects.names(fieldClassName));
    Pstream::combineGather(fieldNames, ListUniqueEqOp<word>());
    Pstream::combineScatter(fieldNames);

    if (!selectedFields.empty())
    {
        DynamicList<word> selectedNames(fieldNames.size());
        forAll(fieldNames, i)
        {
            if (selectedFields.found(fieldNames[i]))
            {
                selectedNames.append(fieldNames[i]);
            }
        }
        fieldNames.transfer(selectedNames);
    }
    return fieldNames;
}


template<class Type>
void Foam::parLagrangianRedistributor::redistributeLagrangianFields
(
    const mapDistributeBase& map,
    const word& cloudName,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    const wordList objectNames
    (
        filterObjects<IOField<Type> >
        (
            objects,
            selectedFields
        )
    );

    if (objectNames.size())
    {
        const word fieldClassName(IOField<Type>::typeName);

        Info<< "    Redistributing lagrangian "
            << fieldClassName << "s\n" << endl;

        forAll(objectNames, i)
        {
            Info<< "        " <<  objectNames[i] << endl;

            // Read if present
            IOField<Type> field
            (
                IOobject
                (
                    objectNames[i],
                    procMesh_.time().timeName(),
                    cloud::prefix/cloudName,
                    procMesh_,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE,
                    false
                ),
                0
            );

            map.distribute(field);


            IOField<Type>
            (
                IOobject
                (
                    objectNames[i],
                    baseMesh_.time().timeName(),
                    cloud::prefix/cloudName,
                    baseMesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                field
            ).write();
        }
    }
}


template<class Type>
void Foam::parLagrangianRedistributor::redistributeLagrangianFieldFields
(
    const mapDistributeBase& map,
    const word& cloudName,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    wordList objectNames
    (
        filterObjects<CompactIOField<Field<Type>, Type> >
        (
            objects,
            selectedFields
        )
    );

    // Append IOField names
    {
        const wordList ioFieldNames
        (
            filterObjects<IOField<Field<Type> > >
            (
                objects,
                selectedFields
            )
        );
        objectNames.append(ioFieldNames);
    }


    if (objectNames.size())
    {
        const word fieldClassName(CompactIOField<Field<Type>, Type>::typeName);

        Info<< "    Redistributing lagrangian "
            << fieldClassName << "s\n" << endl;

        forAll(objectNames, i)
        {
            Info<< "        " <<  objectNames[i] << endl;

            // Read if present
            CompactIOField<Field<Type>, Type > field
            (
                IOobject
                (
                    objectNames[i],
                    procMesh_.time().timeName(),
                    cloud::prefix/cloudName,
                    procMesh_,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE,
                    false
                ),
                0
            );

            // Distribute
            map.distribute(field);

            // Write
            CompactIOField<Field<Type>, Type>
            (
                IOobject
                (
                    objectNames[i],
                    baseMesh_.time().timeName(),
                    cloud::prefix/cloudName,
                    baseMesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                field
            ).write();
        }
    }
}

// ************************************************************************* //
