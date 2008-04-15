REM !/bin/sh

SET MICORC=NUL
SET ADDR=inet:localhost:12456
REM  run BOA daemon
start micod -ORBIIOPAddr %ADDR% --forward
pause 1


REM  register server
imr create Account shared server "IDL:Account:1.0#foobar"   -ORBImplRepoAddr %ADDR%

REM  run client
.\client %ADDR%
