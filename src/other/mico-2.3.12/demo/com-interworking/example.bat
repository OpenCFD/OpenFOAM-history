REM !/bin/sh

REM Before call "example.bat" run ServerCOM /REGSERVER to register server

SET MICORC=NUL
SET ADDR=inet:localhost:12456
REM  run BOA daemon
start micod -ORBIIOPAddr %ADDR% --forward
pause 1


REM  register server
imr create Account shared account2\server.exe "IDL:Account:1.0#foobar"   -ORBImplRepoAddr %ADDR%
pause 1

REM start client COM
COMSide\ClientCOM.exe 
