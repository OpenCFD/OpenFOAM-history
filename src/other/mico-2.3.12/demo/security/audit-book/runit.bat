REM !/bin/sh

SET MICORC=NUL
SET ADDR=inet:localhost:12124
start /b .\server -ORBIIOPAddr %ADDR% 
pause 1


.\client %ADDR%
pause 1
