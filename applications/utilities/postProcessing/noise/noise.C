/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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

Application
    noise

Description
    Utility to perform noise analysis of pressure data using the noiseFFT
    library.


    \heading Usage

    Control settings are read from the $FOAM_CASE/system/noiseDict dictionary,
    or user-specified dictionary using the -dict option.  Presure data can be
    supplied as a single time history at a given point location, or a surface
    of time histories.

    Both operation types require:
    \verbatim
    pRef            101325;
    N               65536;
    fl              25;
    fu              10000;
    fftWriteInterval 1;
    dataMode        point;
    \endverbatim

    where
    \table
        Property    | Description                   | Required  | Default value
        pRef        | Reference pressure            | no        | 0
        N           | Number of samples in sampling window | no | 65536 (2^16)
        fl          | Lower frequency band          | no        | 25
        fu          | Upper frequency band          | no        | 10000
        fftWriteInterval | Output interval for FFT data | no    | 1
        dataMode    | Input data mode, 'point' or 'surface' | yes |
    \endtable

    Point-based pressure data is read using a CSV reader, and output in a
    graph format:
    \verbatim
    pointData
    {
        csvFileData
        {
            fileName        "pressureData";
            nHeaderLine     1;
            refColumn       0;
            componentColumns (1);
            separator       " ";
            mergeSeparators yes;
        }

        graphFormat     raw;
        windowOverlapPercent 0;
        nWindow         100; // fixed number of windows
    }
    \endverbatim

    where
    \table
        Property    | Description                   | Required  | Default value
        csvFileData | CSV file data dictionary - see CSV.H  | yes |
        graphFormat | Output graph format           | no        | raw
        windowOverlapPercent | Window overla percent| yes       |
        nWindow     | Number of sampling windows    | no        | 1, calculated
    \endtable

    Surface-based pressure data is specified using a surface reader, and output
    using a surface writer.  The reader type must support multiple time
    handling, e.g. the ensight format.  Output surfaces are written on a
    per-frequency basis:
    \verbatim
    surfaceData
    {
        reader          ensight;
        writer          ensight;
        inputFile       "postProcessing/faceSource1/surface/patch1/patch1.case";
        pName           p;
        windowOverlapPercent 50;
        // nWindow         0; // let code decide if not present
    }
    \endverbatim

    where
    \table
        Property    | Description                   | Required  | Default value
        reader      | Surface reader type           | yes       |
        writer      | Surface writer type           | yes       |
        inputFile   | Pressure data surface file    | yes       |
        pName       | Name of pressure field        | no        | p
        windowOverlapPercent | Window overla percent| yes       |
        nWindow     | Number of sampling windows    | no        | 1, calculated
    \endtable

    Current outputs include:
    - FFT of the pressure data (Pf)
    - narrow-band PFL (pressure-fluctuation level) spectrum (Lf)
    - one-third-octave-band PFL spectrum (Ldelta)
    - one-third-octave-band pressure spectrum (Pdelta)

Note:
  - If using the windowOverlapPercent entry, the code will automatically
    determine number of windows
  - However, if supplied, the nWindow entry will take precedence

SeeAlso
    CSV.H
    surfaceReader.H
    surfaceWriter.H
    noiseFFT.H

\*---------------------------------------------------------------------------*/

#include "noiseFFT.H"
#include "argList.H"
#include "Time.H"
#include "functionObjectFile.H"
#include "CSV.H"
#include "ListOps.H"
#include "NamedEnum.H"
#include "surfaceReader.H"
#include "surfaceWriter.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

enum dataMode
{
    POINT,
    SURFACE
};

namespace Foam
{
    template<>
    const char* NamedEnum<dataMode, 2>::names[] =
    {
        "point",
        "surface"
    };
}

static const NamedEnum<dataMode, 2> dataModeNames;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::scalar checkUniformTimeStep(const List<scalar>& t)
{
    // Check that a uniform time step has been applied
    scalar deltaT = -1.0;
    if (t.size() > 1)
    {
        for (label i = 1; i < t.size(); i++)
        {
            scalar dT = t[i] - t[i-1];
            if (deltaT < 0)
            {
                deltaT = dT;
            }

            if (mag(deltaT - dT) > SMALL)
            {
                FatalErrorIn("scalar checkUniformTimeStep(const List<scalar>&)")
                    << "Unable to process data with a variable time step"
                    << exit(FatalError);
            }
        }
    }
    else
    {
        FatalErrorIn("scalar checkUniformTimeStep(const List<scalar>&)")
            << "Unable to create FFT with a single value"
            << exit(FatalError);
    }

    return deltaT;
}


Foam::label findStartTimeIndex
(
    const instantList& allTimes,
    const scalar startTime
)
{
    forAll(allTimes, timeI)
    {
        const instant& i = allTimes[timeI];

        if (i.value() >= startTime)
        {
            return timeI;
        }
    }

    return 0;
}


void filterPointTimeData
(
    const CSV<scalar>& pData,
    const scalar startTime,
    scalarField& t,
    scalarField& p
)
{
    const scalarField t0(pData.x());
    const scalarField p0(pData.y());

    DynamicList<scalar> tf(t0.size());
    DynamicList<scalar> pf(t0.size());

    forAll(t0, timeI)
    {
        if (t0[timeI] >= startTime)
        {
            tf.append(t0[timeI]);
            pf.append(p0[timeI]);
        }
    }

    t.transfer(tf);
    p.transfer(pf);
}


void setWindows
(
    const dictionary& dict,
    const label nSampleTot,
    const label nSample,
    label& nWindow,
    label& nOverlap
)
{
    if (nSampleTot < nSample)
    {
        FatalErrorIn
        (
            "void setWindows"
            "("
                "const dictionary&, "
                "const label, "
                "const label, "
                "label&, "
                "label&"
            ")"
        )
            << "Block size N = " << nSample
            << " is larger than total number of data points = " << nSampleTot
            << exit(FatalError);
    }

    const scalar prc = readScalar(dict.lookup("windowOverlapPercent"));
    nOverlap = floor(prc/scalar(100)*nSample);
    label nWindowMax = floor((nSampleTot - nSample)/(nSample - nOverlap)) + 1;

    nWindow = nWindowMax;
    if (dict.readIfPresent("nWindow", nWindow))
    {
        if (nWindow > nWindowMax)
        {
            FatalErrorIn
            (
                "void setWindows"
                "("
                    "const dictionary&, "
                    "const label, "
                    "const label, "
                    "label&, "
                    "label&"
                ")"
            )
                << "Number of data points calculated with " << nWindow
                << " windows greater than the total number of data points"
                << nl
                << "    Block size, N = " << nSample << nl
                << "    Total number of data points = " << nSampleTot << nl
                << "    Maximum number of windows = " << nWindowMax << nl
                << "    Requested number of windows = " << nWindow
                << exit(FatalError);
        }
    }

    label nRequiredTimes = nSample + (nWindow - 1)*nOverlap;

    Info<< "Windowing:" << nl
        << "    Total samples              : " << nSampleTot << nl
        << "    Samples per window         : " << nSample << nl
        << "    Number of windows          : " << nWindow << nl
        << "    Overlap size               : " << nOverlap << nl
        << "    Required number of samples : " << nRequiredTimes
        << endl;
}


void readSurfaceData
(
    const autoPtr<surfaceReader>& reader,
    const scalarList& times,
    const label startTimeIndex,
    const label pIndex,
    const labelList& procFaceOffset,
    List<scalarField>& pData
)
{
    // Data is stored as pressure on surface at a given time.  Now we need to
    // pivot the data so that we have the complete pressure time history per
    // surface face.  In serial mode, this results in all pressure data being
    // loaded into memory (!)

    Info << "Reading pressure data" << endl;

    if (Pstream::parRun())
    {
        PstreamBuffers pBufs(Pstream::nonBlocking);

        // Procedure:
        // 1. Master processor reads pressure data for all faces for all times
        // 2. Master sends each processor a subset of faces
        // 3. Local processor reconstructs the full time history for the subset
        //    of faces
        // Note: reading all data on master to avoid potential NFS problems...

        const label myProcI = Pstream::myProcNo();
        const label nLocalFace =
            procFaceOffset[myProcI + 1] - procFaceOffset[myProcI];

        // Complete pressure time history data for subset of faces
        pData.setSize(nLocalFace);
        const label nTimes = times.size();
        forAll(pData, faceI)
        {
            pData[faceI].setSize(nTimes);
        }

        // Read and send data
        forAll(times, i)
        {
            pBufs.clear();

            if (Pstream::master())
            {
                label timeI = i + startTimeIndex;

                Info<< "    time: " << times[i] << endl;

                // Read pressure at all faces for time timeI
                scalarField p(reader->field(timeI, pIndex, scalar(0)));

                // Send subset of faces to each processor
                for (label procI = 0; procI < Pstream::nProcs(); procI++)
                {
                    label face0 = procFaceOffset[procI];
                    label nLocalFace =
                        procFaceOffset[procI + 1] - procFaceOffset[procI];

                    UOPstream toProc(procI, pBufs);
                    toProc << SubList<scalar>(p, nLocalFace, face0);
                }
            }

            pBufs.finishedSends();

            // Receive data from the master
            UIPstream fromProc(0, pBufs);

            scalarList pSlice(fromProc);

            forAll(pSlice, faceI)
            {
                pData[faceI][i] = pSlice[faceI];
            }
        }
    }
    else
    {
        const label nLocalFace = procFaceOffset[0];

        pData.setSize(nLocalFace);
        forAll(times, timeI)
        {
            forAll(pData, faceI)
            {
                pData[faceI].setSize(times.size());
            }
        }

        forAll(times, i)
        {
            label timeI = i + startTimeIndex;

            Info<< "    time: " << times[i] << endl;
            const scalarField p(reader->field(timeI, pIndex, scalar(0)));

            forAll(p, faceI)
            {
                pData[faceI][i] = p[faceI];
            }
        }
    }

    Info<< "Read "
        << returnReduce(pData.size(), sumOp<label>())
        << " pressure traces with "
        << times.size()
        << " time values" << nl << endl;
}


void writeSurfaceData
(
    const autoPtr<surfaceWriter>& writer,
    autoPtr<surfaceReader>& reader,
    const word& fName,
    const word& groupName,
    const word& title,
    const scalar freq,
    const scalarField& data,
    const labelList& procFaceOffset
)
{
    Info<< "    processing " << title << " for frequency " << freq << endl;

    fileName outDir
    (
        fileName("postProcessing")/"noise"/groupName/Foam::name(freq)
    );

    if (Pstream::parRun())
    {
        // Collect the surface data so that we can output the surfaces

        PstreamBuffers pBufs(Pstream::nonBlocking);

        if (!Pstream::master())
        {
            UOPstream toProc(0, pBufs);
            toProc << data;
        }

        pBufs.finishedSends();

        if (Pstream::master())
        {
            const meshedSurface& surf = reader->geometry();

            scalarField allData(surf.size());

            forAll(data, faceI)
            {
                // Master procFaceOffset is zero...
                allData[faceI] = data[faceI];
            }

            for (label procI = 1; procI < Pstream::nProcs(); procI++)
            {
                UIPstream fromProc(procI, pBufs);
                scalarList dataSlice(fromProc);
                forAll(dataSlice, i)
                {
                    label faceI = procFaceOffset[procI] + i;
                    allData[faceI] = dataSlice[i];
                }
            }

            fileName outFileName = writer->write
            (
                outDir,
                fName,
                surf.points(),
                surf.faces(),
                title,
                allData,
                false
            );
        }
    }
    else
    {
        const meshedSurface& surf = reader->geometry();

        writer->write
        (
            outDir,
            fName,
            surf.points(),
            surf.faces(),
            title,
            data,
            false
        );
    }
}


void processPointData
(
    const dictionary& dict,
    const scalar pRef,
    const label N,
    const scalar fl,
    const scalar fu,
    const scalar startTime,
    const label fftWriteInterval
)
{
    // Point data only handled by master
    if (!Pstream::master())
    {
        return;
    }

    Info<< "Reading data file" << endl;

    CSV<scalar> pData("pressure", dict, "Data");

    // Graph format
    word graphFormat = dict.lookupOrDefault<word>("graphFormat", "raw");

    // Time and pressure history data
    scalarField t, p;
    filterPointTimeData(pData, startTime, t, p);
    Info<< "    read " << t.size() << " values" << nl << endl;

    Info<< "Creating noise FFT" << endl;
    noiseFFT nfft(checkUniformTimeStep(t), p);

    // Determine the windowing
    label nWindow = 1;
    label nOverlap = 0;
    setWindows(dict, t.size(), N, nWindow, nOverlap);

    nfft -= pRef;

    fileName baseFileName(pData.fName().lessExt());

    graph Pf(nfft.RMSmeanPf(N, min(nfft.size()/N, nWindow), nOverlap));
    Info<< "    Creating graph for " << Pf.title() << endl;
    Pf.write(baseFileName + graph::wordify(Pf.title()), graphFormat);

    graph Lf(nfft.Lf(Pf));
    Info<< "    Creating graph for " << Lf.title() << endl;
    Lf.write(baseFileName + graph::wordify(Lf.title()), graphFormat);

    graph Ldelta(nfft.Ldelta(Lf, fl, fu));
    Info<< "    Creating graph for " << Ldelta.title() << endl;
    Ldelta.write(baseFileName + graph::wordify(Ldelta.title()), graphFormat);

    graph Pdelta(nfft.Pdelta(Pf, fl, fu));
    Info<< "    Creating graph for " << Pdelta.title() << endl;
    Pdelta.write(baseFileName + graph::wordify(Pdelta.title()), graphFormat);

    graph PSDf(nfft.PSDf(N, min(nfft.size()/N, nWindow), nOverlap));
    Info<< "    Creating graph for " << PSDf.title() << endl;
    PSDf.write(baseFileName + graph::wordify(PSDf.title()), graphFormat);

    graph PSD(nfft.PSD(PSDf));
    Info<< "    Creating graph for " << PSD.title() << endl;
    PSD.write(baseFileName + graph::wordify(PSD.title()), graphFormat);
}


void processSurfaceData
(
    const dictionary& dict,
    const scalar pRef,
    const label N,
    const scalar fl,
    const scalar fu,
    const scalar startTime,
    const label fftWriteInterval
)
{
    const fileName inputFile(dict.lookup("inputFile"));

    autoPtr<surfaceReader> reader;
    autoPtr<surfaceWriter> writer;

    // Index of pressure field data in surface reader
    label pIndex;

    // Time data
    scalarList times;
    label startTimeIndex = 0;
    scalar deltaT = 0;

    // Number of surface faces
    label nFace = 0;

    // Windowing
    label nWindow = 1;
    label nOverlap = 0;

    // All reading performed on the master processor only
    if (Pstream::master())
    {
        // Create the surface reader
        const word readerType(dict.lookup("reader"));
        reader.reset(surfaceReader::New(readerType, inputFile).ptr());

        // Find the index of the pressure data
        const word pName(dict.lookupOrDefault<word>("pName", "p"));
        const List<word> fieldNames(reader->fieldNames(0));
        pIndex = findIndex(fieldNames, pName);
        if (pIndex == -1)
        {
            FatalErrorIn
            (
                "void processSurfaceData"
                "("
                    "const dictionary&, "
                    "const scalar, "
                    "const label, "
                    "const scalar, "
                    "const scalar, "
                    "const scalar"
                ")"
            )
                << "Unable to find pressure field name " << pName
                << " in list of available fields: " << fieldNames
                << exit(FatalError);
        }

        // Create the surface writer
        // - Could be done later, but since this utility can process a lot of
        //   data we can ensure that the user-input is correct prior to doing
        //   the heavy lifting
        const word writerType(dict.lookup("writer"));
        dictionary optDict
        (
            dict.subOrEmptyDict("writeOptions").subOrEmptyDict(writerType)
        );
        writer.reset(surfaceWriter::New(writerType, optDict).ptr());

        // Set the time range
        const instantList allTimes = reader->times();
        startTimeIndex = findStartTimeIndex(allTimes, startTime);

        // Determine the windowing
        label nAvailableTimes = allTimes.size() - startTimeIndex;
        setWindows(dict, nAvailableTimes, N, nWindow, nOverlap);

        // Restrict times
        label nRequiredTimes = N + (nWindow - 1)*nOverlap;
        times.setSize(nRequiredTimes);
        forAll(times, timeI)
        {
            times[timeI] = allTimes[timeI + startTimeIndex].value();
        }
        deltaT = checkUniformTimeStep(times);

        const meshedSurface& surf = reader->geometry();
        nFace = surf.size();
    }

    Pstream::scatter(pIndex);
    Pstream::scatter(times);
    Pstream::scatter(startTimeIndex);
    Pstream::scatter(deltaT);
    Pstream::scatter(nFace);
    Pstream::scatter(nWindow);
    Pstream::scatter(nOverlap);

    // Container for pressure time history data per face
    List<scalarField> pData;

    // Processor procFaceOffsets
    labelList procFaceOffset;
    if (Pstream::parRun())
    {
        const label nProcs = Pstream::nProcs();
        const label nFacePerProc = floor(nFace/nProcs) + 1;

        procFaceOffset.setSize(nProcs + 1, 0);
        for (label i = 1; i < procFaceOffset.size(); i++)
        {
            procFaceOffset[i] = min(i*nFacePerProc, nFace);
        }
    }
    else
    {
        procFaceOffset.setSize(1, nFace);
    }


    // Read pressure data from file
    readSurfaceData
    (
        reader,
        times,
        startTimeIndex,
        pIndex,
        procFaceOffset,
        pData
    );

    // Process the pressure data, and store results as surface values per
    // frequency so that it can be output using the surface writer

    Info<< "Creating noise FFTs" << endl;

    // Storage for FFT data
    const label nLocalFace = pData.size();
    const scalarField freq1(noiseFFT::frequencies(N, deltaT));
    const label nFFT = freq1.size()/fftWriteInterval;
    List<scalarField> surfPf(nFFT);
    List<scalarField> surfLf(nFFT);
    List<scalarField> surfPSD(nFFT);
    forAll(surfPf, freqI)
    {
        surfPf[freqI].setSize(nLocalFace);
        surfLf[freqI].setSize(nLocalFace);
        surfPSD[freqI].setSize(nLocalFace);
    }

    // Storage for 1/3 octave data
    const scalarField freq2
    (
        noiseFFT::oneThirdOctaveFrequencies(freq1, deltaT, fl, fu)
    );
    List<scalarField> surfPdelta(freq2.size());
    List<scalarField> surfLdelta(freq2.size());
    forAll(surfPdelta, freqI)
    {
        surfPdelta[freqI].setSize(nLocalFace);
        surfLdelta[freqI].setSize(nLocalFace);
    }

    forAll(pData, faceI)
    {
        const scalarField& p = pData[faceI];

        noiseFFT nfft(deltaT, p);

        nfft -= pRef;
        graph Pf(nfft.RMSmeanPf(N, min(nfft.size()/N, nWindow), nOverlap));
        graph Lf(nfft.Lf(Pf));
        graph Pdelta(nfft.Pdelta(Pf, fl, fu));
        graph Ldelta(nfft.Ldelta(Lf, fl, fu));
        graph PSDf(nfft.PSDf(N, min(nfft.size()/N, nWindow), nOverlap));
        graph PSD(nfft.PSD(PSDf));

        // Store the frequency results in slot for face of surface
        forAll(surfPf, i)
        {
            label freqI = (i + 1)*fftWriteInterval - 1;
            surfPf[i][faceI] = Pf.y()[freqI];
            surfLf[i][faceI] = Lf.y()[freqI];
            surfPSD[i][faceI] = PSD.y()[freqI];
        }

        // Store the 1/3 octave results in slot for face of surface
        forAll(freq2, freqI)
        {
            surfPdelta[freqI][faceI] = Pdelta.y()[freqI];
            surfLdelta[freqI][faceI] = Ldelta.y()[freqI];
        }

        // Free the storage for p
//        p.clear();
    }

    Info<< "Writing fft surface data" << endl;

    forAll(surfPf, i)
    {
        label freqI = i*fftWriteInterval;
        const word& fName = inputFile.name(true);
        const word gName = "fft";
        writeSurfaceData
        (
            writer,
            reader,
            fName,
            gName,
            "Pf",
            freq1[freqI],
            surfPf[i],
            procFaceOffset
        );
        writeSurfaceData
        (
            writer,
            reader,
            fName,
            gName,
            "Lf",
            freq1[freqI],
            surfLf[i],
            procFaceOffset
        );
        writeSurfaceData
        (
            writer,
            reader,
            fName,
            gName,
            "PSD",
            freq1[freqI],
            surfPSD[i],
            procFaceOffset
        );
    }

    Info<< "Writing one-third octave surface data" << endl;

    forAll(freq2, i)
    {
        const word& fName = inputFile.name(true);
        const word gName = "oneThirdOctave";
        writeSurfaceData
        (
            writer,
            reader,
            fName,
            gName,
            "Pdelta",
            freq2[i],
            surfPdelta[i],
            procFaceOffset
        );
        writeSurfaceData
        (
            writer,
            reader,
            fName,
            gName,
            "Ldelta",
            freq2[i],
            surfLdelta[i],
            procFaceOffset
        );
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "addDictOption.H"
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createFields.H"

    const dataMode mode = dataModeNames.read(dict.lookup("dataMode"));

    switch (mode)
    {
        case POINT:
        {
            processPointData
            (
                dict.subDict("pointData"),
                pRef,
                N,
                fl,
                fu,
                startTime,
                fftWriteInterval
            );
            break;
        }
        case SURFACE:
        {
            processSurfaceData
            (
                dict.subDict("surfaceData"),
                pRef,
                N,
                fl,
                fu,
                startTime,
                fftWriteInterval
            );
            break;
        }
    }

    Info<< nl << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
