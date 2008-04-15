REM !/bin/sh
set path=..\..\..\win32-bin;%path%  
SET ADDR=inet:localhost:12456
REM  run Server
start .\server -ORBIIOPAddr %ADDR% 
pause 1


REM  run client
.\client -ORBBindAddr %ADDR%

