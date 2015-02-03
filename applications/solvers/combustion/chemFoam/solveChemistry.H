    dtChem = chemistry.solve(runTime.deltaT().value());
    scalar Sh = chemistry.Sh()()[0]/rho[0];
    integratedHeat += ShCoeff*Sh*runTime.deltaT().value();
