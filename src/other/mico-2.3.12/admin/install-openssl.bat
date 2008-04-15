@echo off

if ""== "%1" goto print:
 


@set MICO_PATH=%CD%
@set OPENSSL_PATH=%1
rem @set OPENSSL_PATH=E:\corba\openssl\openssl-0.9.6g

@echo Installing from '%OPENSSL_PATH%' into  '%MICO_PATH%'

@echo Checking for headers in %OPENSSL_PATH%\inc32\openssl

@set cnt=0 

@for %%i in (%OPENSSL_PATH%\inc32\openssl\*.h) do (
@echo Checking %%i header.
@	if %%~zi EQU 0 (
			@echo "Invalid size for header file %%i"
			@set cnt=0
			@exit
		) else (
			@echo ... Size is %%~zi ... OK	        
			@set cnt=1
		)
)

@if %cnt% EQU 0 (
@echo Invalid headers in '%OPENSSL_PATH%'
@exit
) else (
	@echo Checked OPENSSL headers .... OK
	)

@echo Checking for binaries in %OPENSSL_PATH%/out32dll
@if exist %OPENSSL_PATH%/out32dll/libeay32.dll (
	 @echo "Checked libeay32.dll"
) else (
	@echo ERROR: Could not find libeay32.dll
	@exit
)

@if exist %OPENSSL_PATH%/out32dll/libeay32.lib  (
	@echo "Checked libeay32.lib"
) else (
	@echo ERROR: Could not find libeay32.lib
	@exit
)


@if exist %OPENSSL_PATH%/out32dll/ssleay32.dll (
	 @echo "Checked ssleay32.dll"
) else (
	@echo ERROR: Could not find ssleay32.dll
	@exit
)

@if exist %OPENSSL_PATH%/out32dll/ssleay32.lib  (
	@echo "Checked ssleay32.lib"
) else (
	@echo ERROR: Could not find ssleay32.lib
	@exit
)


@mkdir %MICO_PATH%\contrib\openssl\openssl 2>output
@mkdir %MICO_PATH%\win32-bin 2>output
@mkdir %MICO_PATH%\win32-bin\lib 2>output

@echo Copying files into %MICO_PATH%\contrib\openssl
@for %%i in (%OPENSSL_PATH%\inc32\openssl\*.h) do @copy /Y %%i %MICO_PATH%\contrib\openssl\openssl 2>output
@for %%i in (%OPENSSL_PATH%\out32dll\*.lib) do @copy /Y %%i %MICO_PATH%\win32-bin\lib 2>output
@for %%i in (%OPENSSL_PATH%\out32dll\*.dll) do @copy /Y %%i %MICO_PATH%\win32-bin 2>output


@cd /d %MICO_PATH%
goto exit

:print
@echo THIS SCRIPT COPIES THE FILES FROM A FRESH BUILD OF OPENSSL LIBRARY
@echo TO THE CORRECT DIRECTORIES INSIDE MICO TREE
@echo THIS MEANS:
@echo  "$(OPENSSL_PATH)/include/openssl -> $(MICO_PATH)/contrib/openssl"
@echo  "$(OPENSSL_PATH)/out32dll/*.dll -> $(MICO_PATH)/win32-bin"
@echo  "$(OPENSSL_PATH)/out32dll/*.lib -> $(MICO_PATH)/win32-bin/lib"
@echo Syntax:
@echo "install-openssl.bat <OPENSSL_ROOT_PATH>"

:exit
