REM !/bin/sh

SET MICORC=NUL
start .\req_server 
pause 1
echo "Sorry no sensible way of measuring time on win32"
echo "request based communication:"
req_client

start .\stream_server 

pause 1

echo "stream based communication:"
stream_client


