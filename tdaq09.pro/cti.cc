#include <time.h>
main()
{
  time_t mtime=time(0);

  printf("%s",ctime(&mtime));
}
