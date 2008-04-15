
set MICORC=NUL
set PATH=..\..\..\win32-bin;%PATH%
set ADDR = 127.0.0.1

echo "starting Naming Service daemon ..."
start ..\..\..\win32-bin\nsd -ORBNoResolve -ORBDebug All -ORBIIOPAddr inet:127.0.0.1:12456

echo "starting Bank server ..."
start .\server -ORBNoResolve -ORBInitRef NameService=corbaloc::127.0.0.1:12456/NameService

pause
echo "running client ..."
.\client  -ORBNoResolve -ORBInitRef NameService=corbaloc::127.0.0.1:12456/NameService
