#!/bin/sh

set ADDR=inet:127.0.0.1:9901
SET MICORC=NUL

#
# Run BOA demon
#

echo ' Run "BOA Demon"'

start micod -ORBIIOPAddr %ADDR% 
pause 2

echo ' Register "Naming Service"'

imr -ORBImplRepoAddr %ADDR% create Naming shared nsd.exe "IDL:omg.org/CosNaming/NamingContext:1.0#root" -ORBNamingAddr %ADDR%

echo ' Run "Property Service Demon"'

start propertyd -ORBImplRepoAddr %ADDR% -ORBNamingAddr %ADDR%  
pause 2

echo ' Create Properties'
set_properties -ORBImplRepoAddr %ADDR% -ORBNamingAddr %ADDR%

echo  Read Properties
echo 
get_properties -ORBImplRepoAddr %ADDR% -ORBNamingAddr %ADDR%



