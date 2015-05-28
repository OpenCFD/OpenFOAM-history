    word dictName("noiseDict");
    if (args.optionFound("dict"))
    {
        dictName = args["dict"];
    }

    IOdictionary dict
    (
        IOobject
        (
            dictName,
            runTime.system(),
            runTime,
            IOobject::MUST_READ
        )
    );

    // Reference pressure
    scalar pRef = dict.lookupOrDefault("pRef", 0.0);

    // Number of samples in sampling window, default = 2^16
    label N = 65536;
    if (dict.readIfPresent("N", N))
    {
        bool powerOf2 = ((N != 0) && !(N & (N - 1)));
        if (!powerOf2)
        {
            FatalIOErrorIn(args.executable().c_str(), dict)
                << "N: Number of samples in sampling windows must be a "
                << "power of 2"
                << exit(FatalIOError);
        }
    }

    // Lower frequency of frequency band, default = 25 Hz
    scalar fl = 25;
    if (dict.readIfPresent("fl", fl))
    {
        if (fl < 0)
        {
            FatalIOErrorIn(args.executable().c_str(), dict)
                << "fl: lower frequency bound must be greater than zero"
                << exit(FatalIOError);

        }
    }

    // Upper frequency of frequency band, default = 10 kHz
    scalar fu = 10000;
    if (dict.readIfPresent("fu", fu))
    {
        if (fu < 0)
        {
            FatalIOErrorIn(args.executable().c_str(), dict)
                << "fu: upper frequency bound must be greater than zero"
                << exit(FatalIOError);

        }
        if (fu < fl)
        {
            FatalIOErrorIn(args.executable().c_str(), dict)
                << "fu: upper frequency bound must be greater than lower "
                << "frequency bound (fl)"
                << exit(FatalIOError);

        }
    }

    // Start time, default = 0
    scalar startTime = dict.lookupOrDefault("startTime", 0);

    // Frequency data output interval, default = 1
    // N/2 data points are returned from the FFT, which can be a vey large
    // number.  This is usually OK for point samples, but can lead to the
    // generation of a very large number of files if operating on surfaces
    label fftWriteInterval = 1;
    if (dict.readIfPresent("fftWriteInterval", fftWriteInterval))
    {
        if (fftWriteInterval <= 0)
        {
            FatalIOErrorIn(args.executable().c_str(), dict)
                << "fftWriteInterval: FFT write interval must be "
                << "greater than zero"
                << exit(FatalIOError);
        }
    }
