#ifndef _buistat_
#define _buistat_

class buistat
{
   public:
  
    int gDone;
    long totalevs;
    long totalfrag;
    long totalstat;
    long bandwidth;
   
   buistat() { totalevs = 0; totalstat = 0; 
                 gDone=0; totalfrag = 0; bandwidth =0;};

};

#endif
