#ifndef _wdtim_
#define _wdtim_


class TimExpExc
{
  public:
   int own;
   TimExpExc(int o):own(o){};
};

class icawd
{
  long mTim;

  public:

   icawd(long a);
   ~icawd();

   void start(); 
//   void DoTask();
};

#endif
