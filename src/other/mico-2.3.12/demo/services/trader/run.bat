SET MICORC=NUL
set ADDR=inet:127.0.0.1:12456
set RC=-ORBImplRepoAddr %ADDR% -ORBTradingAddr %ADDR%
set RC2=-ORBImplRepoAddr %ADDR%
set path=..\..\win32-bin\;%path% 
echo "starting BOA daemon ..."
start micod -ORBIIOPAddr %ADDR% 

pause 1

echo "register trading service ..."
imr create Trading shared traderd.exe "IDL:omg.org/CosTrading/Lookup:1.0" %RC2%

pause 1

echo "and run client ..."
client %RC%
