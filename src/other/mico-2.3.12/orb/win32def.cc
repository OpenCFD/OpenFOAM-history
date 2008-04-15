// (W) 1998 Martin Sander
// modified by Sorin Mustaca - February 2002

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int main(int argc,char** argv)
{
  if (argc != 4) {
    fprintf(stderr,"Need 3 parameters:\n");
    fprintf(stderr,"Usage: %s <[-noheader][-header]> <dll name> <dumpbin file>\nWill output the results to <dllname>.def",argv[0]);

    fprintf(stderr,"\nUsage: win32def.exe -header mico239 mico239.dmp\nWill output the results to mico239.def");
    return 10;
  }
  bool header=false;
  if (stricmp(argv[1], "-noheader") == 0) {
    header = false;
    fprintf(stderr, "\nNo header is created");
  }
  else {
    if (stricmp(argv[1], "-header") == 0) {
      header = true;
      fprintf(stderr, "\nHeader is created");
    }
  }
  char f[255];
  _snprintf(f,254,"%s.def",argv[2]);

  FILE* dmpout=fopen(f,"wt");
  if (dmpout == NULL) {
    fprintf(stderr,"Can not create output file %s",f);
    return 11;
  }
  if (header) {
    fprintf(dmpout,"LIBRARY\t\t%s\n",argv[2]);
    fprintf(dmpout,"EXPORTS\n");
  }

  FILE* dumpbin = fopen(argv[3],"r");

  if (dumpbin == NULL) {
    fprintf(stderr,"cannot read \042%s\042\n",argv[3]);
    return 20;
  }

  int i = 0;

  while (!feof(dumpbin)) {

    char Line[3000];

    memset(Line,0,sizeof(Line));
    fgets(Line,sizeof(Line)-1,dumpbin);

    int dummy1,dummy2;
    char dummy3[3000];
    char name[3000];

    int s = sscanf(Line,"%x %x %s notype () External | %s",&dummy1,&dummy2,&dummy3,name);
    if (s != 4) {
      s = sscanf(Line,"%x %x %s notype External | %s",&dummy1,&dummy2,&dummy3,name);
    }

#if 0
    //necessary if compiling with static runtime library
    // Quite a hack
    // ignore std members because redefinitions
    if(strstr(name,"@std@")!=NULL && NULL==strstr(name,"@CORBA@") && NULL==strstr(name,"MICO")
        && NULL==strstr(name,"FixedBase"))
        continue;
#endif    
    if ((s == 4) &&
	(name[0] == '?') &&
	(strncmp(name,"??_G",4)) &&
	(strncmp(name,"??_E",4)) &&
	(strncmp(name,"??_D",4)) &&
	(strncmp(name,"??_C",4)) &&
        (stricmp(dummy3, "UNDEF")) &&
	(!strstr(name,"@?$"))
	) {
      fprintf(dmpout,"\t\t%s\n",name);
      i++;
      if(i%10 ==0)
          fprintf(stderr,"\r%d symbols",i);
    }
    // sander begin
    if ((s == 4) &&
	(!strncmp(name,"_CORBA",6)) &&
	(stricmp(dummy3,"UNDEF"))
	) {

      fprintf(dmpout,"\t\t%s\n",name+1);
      fprintf(stderr,"\r%d symbols",i++);

    }
    // sander end
  }

  fprintf(stderr,"\n");

  fclose(dumpbin);
  fclose(dmpout);
  
  return 0;
}
