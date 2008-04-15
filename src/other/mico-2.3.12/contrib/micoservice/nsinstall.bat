rem install Naming Service: nsd.exe

rem path where MICO was compiled ( binaries )
set MICODIR=f:\dcm\mico\win32-bin

rem path where micoservice.exe and configuration file are
set INSTALL=f:\dcm\micoservice

echo %MICODIR%\nsd.exe -ORBConfFile %MICODIR%\mico-nsd.conf > nsd.conf

rem Installing service 'MICO Service(NamingService)'
micoservice  NamingService -i %INSTALL%\nsd.conf

rem Starting 'MICO Service(NamingService)'
micoservice  NamingService --start
