set ADDR=inet:localhost:12457
set path=%PATH%;..\..\..\win32-bin
start server -ORBIIOPAddr %ADDR%
client -ORBBindAddr %ADDR%

