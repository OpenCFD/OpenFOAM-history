/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2015 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify i
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::externalCoupled

Group
    grpJobControlFunctionObjects

Description
    This functionObject provides an interface to an external application.
    Values are transferred as plain text files, where OpenFOAM data is written
    as (one line per face):

        # Patch: <patch name>
        <fld1> <fld2> .. <fldn>             //face0
        <fld1> <fld2> .. <fldn>             //face1
        ..
        <fld1> <fld2> .. <fldn>             //faceN

    where <fld> depends on the bc type:
    - externalCoupledMixed: output of transferData
    - mixed: value, normalGradient, valueFraction
    - other: value

    The values are received as the constituent pieces of the `mixed'
    condition, i.e.

        # Patch: <patch name>
        <value> <gradient> <valueFraction>  ..      //face0
        <value> <gradient> <valueFraction>  ..      //face1
        ...
        <value> <gradient> <valueFraction>  ..      //faceN

    Data is sent/received as a single file for all patches from the directory

        $FOAM_CASE/<commsDir>

    At start-up, the boundary creates a lock file, i.e..

        OpenFOAM.lock

    ... to signal the external source to wait.  During the boundary condition
    update, boundary values are written to file, e.g.

        <fileName>.ou

    The lock file is then removed, instructing the external source to take
    control of the program execution.  When ready, the external program
    should create the return values, e.g. to file

        <fileName>.in

    ... and then re-instate the lock file.  The boundary condition will then
    read the return values, and pass program execution back to OpenFOAM.


    Example of function object specification:
    \verbatim
    externalCoupled
    {
        type            externalCoupled;
        ...
        log             yes;
        commsDir        "${FOAM_CASE}/comms";
        initByExternal  yes;

        regions
        {
            region0
            {
                TPatchGroup             // Name of patch(group)
                {
                    readFields  (p);    // List of fields to read
                    writeFields (T);    // List of fields to write
                }
            }
        }
    }
    \endverbatim

    This writes (on the master processor) the directory:
        comms/region0/TPatchGroup/
    with contents:
        patchPoints     (collected points)
        patchFaces      (collected faces)
        p.in            (input file of p, written be external application)
        T.out           (output file of T, written by OpenFOAM)


SourceFiles
    externalCoupled.C
    IOexternalCoupled.H

\*---------------------------------------------------------------------------*/

#ifndef externalCoupled_H
#define externalCoupled_H

#include "NamedEnum.H"
#include "DynamicList.H"
#include "wordReList.H"
#include "labelList.H"
#include "scalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class polyMesh;
class mapPolyMesh;
class IFstream;
class fvMesh;

/*---------------------------------------------------------------------------*\
                      Class externalCoupled Declaration
\*---------------------------------------------------------------------------*/

class externalCoupled
{
    // Private data

        //- Name of the abort file unless otherwise specified
        word name_;

        const objectRegistry& obr_;

        //- Path to communications directory
        fileName commsDir_;

        //- Interval time between checking for return data [s]
        label waitInterval_;

        //- Time out time [s]
        label timeOut_;

        //- Calculation frequency
        label calcFrequency_;

        //- Flag to indicate values are initialised by external application
        bool initByExternal_;

        //- Log flag
        bool log_;

        //- Names of regions
        DynamicList<word> regionNames_;

        // Per region the indices of the group information
        HashTable<labelList> regionToGroups_;

        // Per group the names of the patches/patchGroups
        DynamicList<wordRe> groupNames_;

        // Per group the indices of the patches
        DynamicList<labelList> groupPatchIDs_;

        // Per group the names of the fields to read
        DynamicList<wordList> groupReadFields_;

        // Per group the names of the fields to write
        DynamicList<wordList> groupWriteFields_;


        //- Initialised flag
        bool initialised_;


    // Private Member Functions

        //- Return the file path to the communications directory for the region
        static fileName groupDir
        (
            const fileName& commsDir,
            const word& regionName,
            const wordRe& groupName
        );

        //- Return the file path to the base communications directory
        fileName baseDir() const;

        //- Return the file path to the lock file
        fileName lockFile() const;

        //- Create lock file
        void createLockFile() const;

        //- Remove lock file
        void removeLockFile() const;

        //- Remove files written by OpenFOAM
        void removeWriteFiles() const;

        //- Remove files written by external code
        void removeReadFiles() const;

        //- Wait for response from external source
        void wait() const;


        //- Read data for a single region, single field
        template<class Type>
        bool readData
        (
            const fvMesh& mesh,
            const wordRe& groupName,
            const labelList& patchIDs,
            const word& fieldName
        );
        //- Read data for all regions, all fields
        void readData();

        //- Write data for a single region, single field
        template<class Type>
        bool writeData
        (
            const fvMesh& mesh,
            const wordRe& groupName,
            const labelList& patchIDs,
            const word& fieldName
        ) const;

        //- Write data for all regions, all fields
        void writeData() const;

        void initialise();  //const fileName& transferFile);

        //- Read (and distribute) scalar columns from file. Every processor gets
        //  nRows (= patch size) of these.
        void readColumns
        (
            const label nRows,
            const label nColumns,
            autoPtr<IFstream>& masterFilePtr,
            List<scalarField>& data
        ) const;

        //- Helper: append data from all processors onto master
        template<class Type>
        static tmp<Field<Type> > gatherAndCombine(const Field<Type>& fld);


        //- Disallow default bitwise copy construc
        externalCoupled(const externalCoupled&);

        //- Disallow default bitwise assignmen
        void operator=(const externalCoupled&);


public:

    //- Runtime type information
    TypeName("externalCoupled");

    //- Name of lock file
    static word lockName;

    //- Name of patch key, e.g. '# Patch:' when looking for start of patch data
    static string patchKey;

    // Constructors

        //- Construct for given objectRegistry and dictionary.
        externalCoupled
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFilesUnused = false
        );


    //- Destructor
    virtual ~externalCoupled();


    // Member Functions

        //- Return name of the abort file
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the dictionary settings
        virtual void read(const dictionary&);

        //- Execute, check existence of abort file and take action
        virtual void execute();

        //- Execute at the final time-loop, used for cleanup
        virtual void end();

        //- Called when time was set at the end of the Time::operator++
        virtual void timeSet();

        //- Execute, check existence of abort file and take action
        virtual void write();

        //- Update for changes of mesh - does nothing
        virtual void updateMesh(const mapPolyMesh&)
        {}

        //- Update for changes of mesh - does nothing
        virtual void movePoints(const polyMesh&)
        {}

        //- Write geometry for the group/patch
        static void writeGeometry
        (
            const fvMesh& mesh,
            const fileName& commsDir,
            const wordRe& groupName
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "externalCoupledTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
