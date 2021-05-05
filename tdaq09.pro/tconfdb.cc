#include "Cicaconfdb.h"

main()
{
  icaconfdb myconf(143);

  dedpar mypa;

  trigpar mytri;

  myconf.setcurrun(-19);
  myconf.insert();
  mypa.mask1=0xffffffff;
  mypa.mask2=0xffffffff;
  mypa.chans=0xffffffff;
  mypa.pol= 1;

  myconf.fep_insert(&mypa);

  mytri.mode=6;
  mytri.trigmode=1;
  
  myconf.trgp_insert(&mytri);
}
