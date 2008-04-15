set PATH=%PATH%;..\..\win32-bin

SET MICORC=NUL
SET IADDR=inet:localhost:12123
SET UADDR=unix:/tmp/foo$$
SET LADDR=local:
start .\server -ORBNoResolve -ORBIIOPAddr %IADDR% -ORBNoCodeSets 

pause 1

echo Running various clients
echo "### same process:" 
.\client %LADDR% -ORBNoCodeSets

pause
echo "### same machine (pipe):"
.\client %UADDR% -ORBNoCodeSets

pause
echo "### same machine (TCP):"
.\client %IADDR% -ORBNoCodeSets
