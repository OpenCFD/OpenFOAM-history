REM !/bin/sh

SET MICORC=NUL
SET IRADDR=inet:localhost:44459

start ..\..\ir\ird -ORBIIOPAddr %IRADDR%

pause 2

..\..\idl\idl --no-codegen-c++ --feed-ir -ORBIfaceRepoAddr %IRADDR% account.idl
..\..\idl\idl --no-codegen-c++ --feed-ir -ORBIfaceRepoAddr %IRADDR% browser.idl

.\diiproxy -ORBIfaceRepoAddr %IRADDR%
