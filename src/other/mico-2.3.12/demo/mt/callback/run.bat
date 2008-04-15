SET PATH=%PATH%;..\..\..\win32-bin

rem  run Server
start ./server -ORBIIOPAddr inet:localhost:8912

pause

rem  run client
client -ORBBindAddr inet:localhost:8912 -ORBIIOPAddr inet:localhost:8913

