REM !/bin/sh
SET PATH=%PATH%;..\..\win32-bin
REM insert your multicast address here ...
SET ADDR=inet:224.0.0.1:12123
SET MICORC=NUL

start .\server 1 -ORBIIOPAddr %ADDR% -POAImplName Hello
start .\server 2 -ORBIIOPAddr %ADDR% -POAImplName Hello

pause 1


.\client %ADDR%

