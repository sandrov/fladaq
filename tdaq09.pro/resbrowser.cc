#include <iostream.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <time.h>

#include "Cudpux.h"
#include "Ctcpux.h"
#include "Cresoitem.h"
void *brows(void *);

#include "Cicapertsk.h"
class mb : icapertsk
{
  resolist *mlis;
  
  public: 
  void DoPerTask() {   mlis->printlist(); };

  mb(int period,resolist *aa):mlis(aa),icapertsk(period)
  { start();};
};

main()
{

   resolist mres;
   pthread_t mth;

   mb showlist(1000000,&mres);

   pthread_create(&mth,NULL,brows,(void *)&mres);

   Ctcp htp(8888);

   while (1)
   {
      Ctcp *mcli = htp.Accept();
  
      char mbuf[1024];

      char lastr;
      do 
       { 
        lastr = mbuf[0];
        mcli->Receive(mbuf,1);
        printf("%c",mbuf[0]);
      } while ((mbuf[0]!='\r')||(lastr!='\n'));
      mcli->Receive(mbuf,1);

      strcpy (mbuf,"HTTP/1.0 200 OK\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"MIME-Version 1.0\n");
      mcli->Send(mbuf,strlen(mbuf));

      //strcpy (mbuf,"Connection: close\n");
      //mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"Content-Type: text/html\n\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 3.2//EN\">\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"<HTML>\n");
      mcli->Send(mbuf,strlen(mbuf));

      mcli->Send("<head><title>icab status</title>",32);

      strcpy (mbuf,"<meta http-equiv=""refresh"" content=""5""> ");
      //mcli->Send(mbuf,strlen(mbuf));


      strcpy(mbuf,"<style type=\"text/css\">\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"//<!--\n");
      //mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"body {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"h1   {  font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 16pt; font-weight: bold}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"h2   {  font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 12pt; font-weight: bold}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"th   {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt; font-weight: bold; background-color: #D3DCE3;}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"td   {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt;}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"form   {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"A:link    {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt; text-decoration: none; color: blue}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"A:visited {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt; text-decoration: none; color: blue}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,".nav {  font-family: Verdana, Arial, Helvetica, sans-serif; color: #000000}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"A:hover   {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt; text-decoration: underline; color: red}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"A:hover   {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt; text-decoration: underline; color: red}\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"//-->\n");
      //mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"</style></head>\n");
      mcli->Send(mbuf,strlen(mbuf));
      
      
      strcpy(mbuf,"<body bgcolor=\"#F5F5F5\" text=\"#000000\"> \n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy(mbuf,"<h1> Icarus t600 Builder</h1> <p>\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      time_t mtime=time(0);
      strcpy(mbuf,ctime(&mtime));
      strcat(mbuf,"<p><p><p>\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"<table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<td bgcolor=\"#d3dcd3\">\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<h2>Run#0 <font color=\"#0C9C04\">stopped</font></h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td bgcolor=\"#a4bdb6\">");

      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<h2>Evs 7453</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</h2></td></tr></table>");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy(mbuf,"<p><p><p>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<form method=post action=\"cmd\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<input type=submit value=\"start\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<input type=input value=\"crossing muons\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<input type=submit value=\"system reboot\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"</form>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<p><p><p>\n");
      mcli->Send(mbuf,strlen(mbuf));




      resoitem *mp = mres.thelist;
      int howmany =0;
      while (mp) { howmany++; mp=mp->next;}
      
      /*strcpy (mbuf,"<frameset frameborder=\"no\" rows=\"*");
      for (int ii=0;ii<howmany-1;ii++)
       strcat (mbuf,",*");

      strcat (mbuf,"\">\n");
      mcli->Send(mbuf,strlen(mbuf));
      */
      
      
      strcpy (mbuf,"<table border=\"0\" width=\"100%\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<tr valign=top>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td align = left>");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"<h2>Cpu status</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));


      mp = mres.thelist;

      while (mp)
      {
         struct hostent *theh;
         //strcpy(mbuf,"<frame scrolling=\"no\" src=\"http://");
         strcpy (mbuf,"<tr><td bgcolor=#DDDDDD>");
         if (mp->ttl >0 )
           strcat(mbuf,"<font color=#000000>");
         else
           strcat(mbuf,"<font color=#ff0000>");
         //strcat(mbuf,inet_ntoa(mp->resoAddr));
         theh = gethostbyaddr(inet_ntoa(mp->resoAddr),strlen(inet_ntoa(mp->resoAddr)),AF_INET);
         if (theh) strcat(mbuf,theh->h_name);
         else strcat(mbuf,inet_ntoa(mp->resoAddr));
         //strcat(mbuf,mp->resoAddr);
         strcat(mbuf,"</font></td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy (mbuf,"<td bgcolor=#CCCCCC>");
         if (mp->ttl >0 )
           strcat(mbuf,mp->status);
         else
           strcat(mbuf,"Expired");
         strcat(mbuf,"</td></tr>\n");
         mcli->Send(mbuf,strlen(mbuf));
         mp=mp->next;
      }
 
      //strcpy(mbuf,"</frameset>\n<noframes></noframes>");
      //mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"</table></td>");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"<td>&nbsp</td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td align=top>");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"<h2>Writers status</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf," <table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</table>\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"</td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</tr>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</table>");
      mcli->Send(mbuf,strlen(mbuf));

      mcli->Send("\n\n",2);

      strcpy (mbuf,"<h2> Last Log Message</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf," <table border=\"0\" bgcolor=\"#ffffff\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf," <td bgcolor=\"#cccccc\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"Wed Apr 25 17:01:27 2001 ");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</td>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td>Writer 6 changed disk to /Data2 </td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</tr></table>\n");
      mcli->Send(mbuf,strlen(mbuf));

      strcpy (mbuf,"</body></html>\n");
      mcli->Send(mbuf,strlen(mbuf));
      
      mcli->Disconnect();
       //while ( mcli->Receive(mbuf,1)>0)
       // printf("%c",mbuf[0]);

      sleep(2);
      delete mcli;
   }      

}

void *
brows(void *marg)
{
  resolist *mres = (resolist *)marg;
   Cudp brs(2244);
   char mbuf[1518];

   while(1)
   {
     if ( int recv = brs.Receive(mbuf,256)>0) 
     {
       // cout << "received " << endl;
        in_addr rad;
        memcpy(&rad.s_addr,&brs.clientAddr.sin_addr.s_addr,sizeof (rad.s_addr));
        printf("rec %s\n",mbuf);
        mres->Addresoitem(rad,brs.clientAddr.sin_addr.s_addr,*((long *)mbuf),&mbuf[8]);
     }
    else cout << "none " << recv<< endl;
     mres->ttlupdate();
//     mres->printlist();
   }
}


      
