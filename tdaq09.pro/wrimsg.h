#ifndef _wrimsg_
#define _wrimsg_

struct wriparam
{
   int runno;
   int nocrates;
   int evsperfile;
   int crateid[100];
};

struct trip
{
  int evid;
  int when;
  int type;
};

struct writstat
{
  int status;
  int bandwidth;
  int totfiles;
  int totevs;
};
#endif
