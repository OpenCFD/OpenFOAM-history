@echo Checking OpenSSL install

@if NOT EXIST win32-bin\libeay32.dll goto error
@if NOT EXIST win32-bin\lib\libeay32.lib  goto error
@if NOT EXIST win32-bin\ssleay32.dll  goto error
@if NOT EXIST win32-bin\lib\ssleay32.lib goto error


@echo Found SSL Libraries installed
@admin\cntfiles.exe contrib\openssl\openssl 56 	

@goto end


:error
@echo OPENSSL libraries not found. Disabling OpenSSL
@echo #AUTOMATICALLY GENERATED FILE. DO NOT MODIFY > makevars.win32.ssl
@echo #No library files were found in win32-bin.Disabling OPENSSL build >> makevars.win32.ssl
@echo #>> makevars.win32.ssl
@echo #HAVE_SSL = 0 >> makevars.win32.ssl

:end
@