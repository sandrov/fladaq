#include "Cicadskfil.h"
//#include <sys/vfs.h>
#include <sys/stat.h>
#ifndef __APPLE__
#include <sys/statfs.h>
#endif
#include <unistd.h>
#include <sys/param.h>
#include <sys/mount.h>


icadskfil::icadskfil(int runNo,int maxFilSize,int localdisks)
{
   runnumber=runNo;
   mpresdisk=1;
   maxdisks=localdisks;
   maxSize=maxFilSize;
   takeno=1;
}

int 
icadskfil::Write(char *buf,int size)
{
//  return size;
  if (mFd >0)
    return  write(mFd,buf,size);
  else 
    return size;
}

int
icadskfil::checkSpace()
{
  char fsname[80];
  struct statfs mstatfs;

  sprintf(fsname,"/Data%d",mpresdisk);
  statfs(fsname,&mstatfs);
  if (mstatfs.f_bavail > (maxSize/1024)) return 0;
   
   mpresdisk = 1; 
   do { 
     sprintf(fsname,"/Data%d",mpresdisk);
     statfs(fsname,&mstatfs);
     if (mstatfs.f_bavail > (maxSize/1024)) return 0;
     mpresdisk++;
   }   
   while (mpresdisk<=maxdisks);

   mpresdisk=1;
  // return 1;
   return 0; //force disk writing
}

int 
icadskfil::moveToDone()
{
  //return 0;
  if (mFd>0)
  {
     struct stat mfstat;
     fstat (mFd,&mfstat);
     //printf("mstat st_size %d \n",mfstat.st_size);
     if (mfstat.st_size< maxSize) return 0;

     fclose(myf);

     
     char fname[256],newname[256];
     //sprintf(fname,"/Data%d/building/Run%3.3d_%3.3d.dat",
     sprintf(fname,"/Data%d/building/Run%6.6d_%6.6d.dat",
                                 mpresdisk,runnumber,takeno);
     //sprintf(newname,"/Data%d/completed/Run%3.3d_%3.3d.dat",
     sprintf(newname,"/Data%d/completed/Run%6.6d_%6.6d",
                                 mpresdisk,runnumber,takeno);
     printf("Closing file %s \n",fname);
     rename (fname,newname);
     takeno++;
     myf=NULL;
   }
  return 1;
}

int
icadskfil::moveToDone(int aa)
{
  if (mFd>0)
  {
     struct stat mfstat;
     fstat (mFd,&mfstat);
     //printf("mstat st_size %d \n",mfstat.st_size);

     fclose(myf);

    
     char fname[256],newname[256];
     //sprintf(fname,"/Data%d/building/Run%3.3d_%3.3d.dat",
     sprintf(fname,"/Data%d/building/Run%6.6d_%6.6d.dat",
                                 mpresdisk,runnumber,takeno);
     //sprintf(newname,"/Data%d/completed/Run%3.3d_%3.3d.dat",
     sprintf(newname,"/Data%d/completed/Run%6.6d_%6.6d",
                                 mpresdisk,runnumber,aa);
     printf("Closing file %s \n",fname);
     rename (fname,newname);
     takeno++;
     myf=NULL;
   }
  return 1;
}

FILE *
icadskfil::opennewfile()
{
  printf("opening new file\n");
  fileopen=0;
  mFd = -1;
  myf=NULL;
  if (runnumber != -1 )
  if (checkSpace()==0)
  {
     char fname[256];
     //sprintf(fname,"/Data%d/building/Run%3.3d_%3.3d.dat",
     sprintf(fname,"/Data%d/building/Run%6.6d_%6.6d.dat",
                                 mpresdisk,runnumber,takeno);
     printf("Filename %s\n",fname);
     myf=fopen(fname,"w+");
  
     if (myf)
     {
       fileopen=1;
       mFd = fileno(myf);
     }
     else
     {
       fileopen=0;
       mFd = -1;
     }
  }
  return myf;
}
