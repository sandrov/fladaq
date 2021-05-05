#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

main()
{

   in_addr in;

       struct hostent *hp = gethostbyname("icab25");
       char ** p = hp->h_addr_list;
       (void) memcpy(&in.s_addr, *p, sizeof (in.s_addr));

       printf("Connecting to  %s ... \n",inet_ntoa(in));
       printf("Long addr %x \n",inet_ntoa(in));
}

			    
