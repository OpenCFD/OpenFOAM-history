REM !/bin/sh
set path=%path%;..\..\..\win32-bin
SET MICORC=NUL
REM  run Micod
del /f /q /temp/micod.ref Bank.ref
start micod --forward --ref %temp%/micod.ref
pause 2

REM  Add imr info
imr -ORBImplRepoIOR file://%temp%/micod.ref create Bank poa server IDL:Bank:1.0
imr -ORBImplRepoIOR file://%temp%/micod.ref activate Bank
pause 2

REM  run client
.\client
pause 1
