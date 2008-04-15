REM !/bin/sh

SET MICORC=NUL
REM  run Server
del /f /q hello.ref
start .\server
pause 2


REM  run client
.\client

