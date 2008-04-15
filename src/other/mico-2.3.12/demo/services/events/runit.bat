REM !/bin/sh
set path=..\..\win32-bin;%path% 
SET ADDR=inet:127.0.0.1:12456
SET MICORC=NUL
SET RC=-ORBImplRepoAddr %ADDR% -ORBNamingAddr %ADDR%

del /f /q NamingService-root.info

REM  run BOA daemon
echo "starting BOA daemon ..."
start micod -ORBIIOPAddr %ADDR% 

pause 1

REM  register server
echo "register name service ..."
imr create NameService poa nsd.exe  "IDL:omg.org/CosNaming/NamingContext:1.0#NameService" %RC%

echo "run event service ..."
start eventd %RC% 

pause 4

echo "run event demo server #1 ..."
start .\server %RC% 
pause 2

echo "run event demo server #2 ..."
start .\server2 %RC% 
pause 2


echo "run event demo server #3 ..."
start .\server3 %RC% 
pause 2

REM  run client
echo "and run client #1 ..."
.\client %RC%

echo "and run client #2 ..."
pause 5
.\client %RC%

echo "and run client #3 ..."
pause 5
.\client2 %RC%

