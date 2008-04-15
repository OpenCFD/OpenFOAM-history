set PATH=%PATH%;..\..\..\win32-bin
set ADDR=inet:localhost:12456

start server -ORBIIOPAddr %ADDR%
pause
client -ORBBindAddr %ADDR%

