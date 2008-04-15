#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#ifndef MAX_PATH
#define MAX_PATH 256
#endif
int  main(int argc, char** argv)
{

	char dir[MAX_PATH]="*.*";
	int expectedNr = 56;

	if(argc != 3)
	{
		printf("\nSyntax: cntfiles.exe [<directory>=\"./\"] <expected NR of files>");
		printf("\n\nDefaulting to %s and %d files",dir,expectedNr);
	}
	else
	{
		strncpy(dir,argv[1],MAX_PATH-1);
		dir[MAX_PATH-1] = 0;
		strcat(dir,"\\*.*");
		printf("\nReading %s",dir);


		expectedNr = atoi(argv[2]);

		if(expectedNr == 0)//error
		{
			printf("\nError reading number of files to be expected");
			return -1;
		}
	}
	
	struct _finddata_t file;
	long hFile;
	long cnt=0;

	if( (hFile = _findfirst( dir, &file )) == -1L )
		printf("\nNo files in directory %s !",dir );
	else
	{ 
		
		while( _findnext( hFile, &file ) == 0 )
		{
			if(file.attrib & _A_SUBDIR )
				continue;

			cnt++;
		}

		_findclose( hFile );


	}
	
	printf("\n%d files found\n\n",cnt);

	FILE* f= fopen("makevars.win32.ssl","wt");
	if(f == NULL)
	{
		printf("\nCouldn't create file makevars.win32.ssl");
		return -1;
	}
	
	static const size_t SZ=1024;
	char str[SZ]="";

	sprintf(str,"\n#AUTOMATICALLY GENERATED FILE. DO NOT MODIFY\
				 \n#%d files were found in %s.Expecting %d",cnt,dir,expectedNr);

	if(cnt == expectedNr)
	{
		 strcat(str,"\n#Enabling OPENSSL build\n\nHAVE_SSL = 1\n");
	}
	else
	{
		 strcat(str,"\n#Disabling OPENSSL build\n\n# HAVE_SSL = 0\n");	
	}

	printf("%s",str);
	fprintf(f,"%s",str);
	fclose(f);


	return cnt;
}