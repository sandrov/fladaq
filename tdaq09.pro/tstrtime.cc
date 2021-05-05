#include <stdio.h>
#include <time.h>

main()
{
   struct tm *myt;
   struct tm myt2;


   time_t tim = time(0);

   myt = localtime(&tim);

   char mytstr[1024];

   strftime (mytstr,1024,"%Y%m%d%H%M%S",myt);

   printf("The time str %s\n",mytstr);
   strptime (mytstr,"%Y%m%d%H%M%S",&myt2);
   
   tim = mktime(&myt2); 
   printf ("converted back to %s\n",ctime(&tim));

}
   
