REM !/bin/sh

SET MICORC=NUL
SET ADDR=inet:localhost:12123
REM 
REM  there are two ORB options to specify the code sets the application
REM  uses for chars and wide chars.
REM  -ORBNativeCS specifies the char code set (which defaults to ISO-8859-1),
REM  -ORBNativeWCS specifies the wide char code set (defaults to UTF-16).
REM  the values of these options are shell-like patterns that must match
REM  the 'description' field of a code set in the OSF code set registry
REM  (see admin/code_set_registry.txt and admin/mico_code_set_registry.txt).
REM 
set PATH=%PATH%;..\..\win32-bin
REM  server speaks ISO-8859-1 (Latin1)
start .\server -ORBIIOPAddr %ADDR% -ORBNativeCS "*8859-1*" 

pause 1


REM  client speaks HTML
.\client %ADDR% -ORBNativeCS "*HTML*"

