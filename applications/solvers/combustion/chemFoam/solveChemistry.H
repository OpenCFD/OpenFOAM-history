    dtChem = chemistry.solve(runTime.deltaT().value());
    scalar Sh = chemistry.Sh()()[0]/rho[0];
    integratedHeat += ShScaleFact*Sh*runTime.deltaT().value();
