#include "Cresbrow.h"
#include "packs.h"
#include <time.h>
#include <iostream>
#include <iostream.h>

class mb : icapertsk
{
  resolist *mlis;
 
  public:
  void DoPerTask() {   mlis->printlist(); };
 
  mb(int period,resolist *aa):mlis(aa),icapertsk(period)
  { start();};
};                                                                              

class  mbp: icathr
{
  resbrow *mrb;
  public:
  mbp(resbrow *p):mrb(p){start();};
  void DoTask() { mrb->browse();};
};

resbrow::resbrow(int port=8888):Ctcp(port)
{
 start();
 mpr = new mb(50000000,&mres);
// mbr = new mbp(this);
}

resbrow::~resbrow()
{
}

void
resbrow::browse()
{
  Cudp brs(2244);
  char mbuf[1518];

   while(1)
   {
     if ( int recv = brs.Receive(mbuf,256)>0)
     {
     in_addr rad;
     long * mcla = (long *)mbuf;
     udpBrd *mpa = (udpBrd *)mbuf;
      memcpy(&rad.s_addr,&brs.clientAddr.sin_addr.s_addr,sizeof (rad.s_addr));
      mres.Addresoitem(rad,brs.clientAddr.sin_addr.s_addr,htonl(*mcla),&mbuf[8],
        htonl(mpa->param[0]),htonl(mpa->param[1]),
           htonl(mpa->param[2]),htonl(mpa->param[3]));
     }        
    else cout << "none " << recv<< endl;
     mres.ttlupdate();
   }
}                                                                               

void
resbrow::DoTask()
{
    while (1)
   {
      Ctcp *mcli = Accept();
 
      char mbuf[1024];
   try
   { 
      char lastr;
      do
       {
        lastr = mbuf[0];
        mcli->Receive(mbuf,1);
        printf("%c",mbuf[0]);
      } while ((mbuf[0]!='\r')||(lastr!='\n'));
      mcli->Receive(mbuf,1);
    
      httpHeader(mcli);
      htmlHeader(mcli);
      bodyHeader(mcli);

      evmStatus(mcli);
      cpuStatus(mcli);
      cpu2Status(mcli);
      wriStatus(mcli);
      logStatus(mcli);     
 
      bodyFooter(mcli);

    }
   catch(...)
   {
    printf("Some errors with client\n");
   }

      mcli->Disconnect();  
      delete mcli;                                                              

 }

} 

void
resbrow::bodyFooter(Ctcp *mcli)
{
   char mbuf[1024];

      strcpy (mbuf,"</body></html>\n");
      mcli->Send(mbuf,strlen(mbuf));
}
   
void
resbrow::logStatus(Ctcp *mcli)
{
   char mbuf[1024];

         strcpy (mbuf,"<h2> Last Log Message</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf," <table border=\"0\" bgcolor=\"#ffffff\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf," <td bgcolor=\"#dddddd\">");
      mcli->Send(mbuf,strlen(mbuf));
      //strcpy (mbuf,"Wed Apr 25 17:01:27 2001 "); 
      //     mcli->Send(mbuf,strlen(mbuf));
      //time_t mtime=time(0);
      //strcpy(mbuf,ctime(&mtime));
      //strcat(mbuf,"\n");
      //mcli->Send(mbuf,strlen(mbuf));
      if(mlog.fetchlast(mbuf))
      {
        time_t mtime=time(0);
        strcpy(mbuf,ctime(&mtime));
        strcat(mbuf,"\n");
        mcli->Send(mbuf,strlen(mbuf));
        strcpy (mbuf,"<td>No connection to database available ");
      }
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</td></tr></table>\n");
      mcli->Send(mbuf,strlen(mbuf));
}
 
void
resbrow::wriStatus(Ctcp *mcli)
{
   char mbuf[1024];
   
      resoitem *mp = mres.thelist;
         strcpy (mbuf,"<td align=top>");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf,"<h2>Wheel 2  status</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf," <table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));

        while (mp)
      {
        if(mp->resclass==2)
        {
         struct hostent *theh;
         //strcpy(mbuf,"<frame scrolling=\"no\" src=\"http://");
         strcpy (mbuf,"<tr><td bgcolor=#DDDDDD>");
         if (mp->ttl >0 )
           strcat(mbuf,"<font color=#000000>");
         else
           strcat(mbuf,"<font color=#ff0000>");
         //strcat(mbuf,inet_ntoa(mp->resoAddr));
         //theh = gethostbyaddr(inet_ntoa(mp->resoAddr),strlen(inet_ntoa(mp->resoAddr)),AF_INET);
         theh=0;
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
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         char strmp[200];
         strcpy(mbuf,"<td bgcolor=#EEEEEE>");
         sprintf(strmp,"%d",mp->par1);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#CCCCCC>");
         sprintf(strmp,"%d",mp->par2);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#EEEEEE>");
         sprintf(strmp,"%d",mp->par3);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#CCCCCC>");
         sprintf(strmp,"%d",mp->par4);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));

         strcpy(mbuf,"</tr>\n");
         mcli->Send(mbuf,strlen(mbuf));
        }
         mp=mp->next;
      }                                                                          

      strcpy (mbuf,"</table>\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf,"</td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</tr>");  
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</table>");
      mcli->Send(mbuf,strlen(mbuf));
 
      mcli->Send("\n\n",2); 
}
void
resbrow::cpu2Status(Ctcp *mcli)
{
   char mbuf[1024];

      resoitem *mp = mres.thelist;
      int howmany =0;
      while (mp) { howmany++; mp=mp->next;}                                          
      strcpy (mbuf,"<td align = left>");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf,"<h2>&nbsp </h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));                                            
      mp = mres.thelist;
 
      while (mp)
      {
        if(mp->resclass==1)
        if((htonl(mp->resoAddr.s_addr)&0xff)>49)
        {
         struct hostent *theh;
         //strcpy(mbuf,"<frame scrolling=\"no\" src=\"http://");
         strcpy (mbuf,"<tr><td bgcolor=#DDDDDD>");
         if (mp->ttl >0 )
           strcat(mbuf,"<font color=#000000>");
         else
           strcat(mbuf,"<font color=#ff0000>");
         //strcat(mbuf,inet_ntoa(mp->resoAddr));
         //theh = gethostbyaddr(inet_ntoa(mp->resoAddr),strlen(inet_ntoa(mp->resoAddr)),AF_INET);
         theh=0; if (theh) strcat(mbuf,theh->h_name);
         else strcat(mbuf,inet_ntoa(mp->resoAddr));
         //strcat(mbuf,mp->resoAddr);
         strcat(mbuf,"</font></td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy (mbuf,"<td bgcolor=#CCCCCC>");                                           if (mp->ttl >0 )
           strcat(mbuf,mp->status);
         else
           strcat(mbuf,"Expired");
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#EEEEEE>");
         char strmp[256];
         sprintf(strmp,"%d",mp->par1);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#DDDDDD>");
         sprintf(strmp,"%d",mp->par2);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         strcpy(mbuf,"<td bgcolor=#EEEEEE>");
         sprintf(strmp,"%d",mp->par3);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         strcat(mbuf,"</tr>\n");
         mcli->Send(mbuf,strlen(mbuf));
        }
         mp=mp->next;
      } 
      strcpy (mbuf,"</table></td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td>&nbsp</td>"); 
}

void
resbrow::cpuStatus(Ctcp *mcli)
{
   char mbuf[1024];

      resoitem *mp = mres.thelist;
      int howmany =0;
      while (mp) { howmany++; mp=mp->next;}                                          
      strcpy (mbuf,"<table border=\"0\" width=\"100%\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<tr valign=top>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td align = left>");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf,"<h2>Wheel 1  status</h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));                                            
      mp = mres.thelist;
 
      while (mp)
      {
        if(mp->resclass==1)
        if((htonl(mp->resoAddr.s_addr)&0xff)<49)
        {
         struct hostent *theh;
         //strcpy(mbuf,"<frame scrolling=\"no\" src=\"http://");
         strcpy (mbuf,"<tr><td bgcolor=#DDDDDD>");
         if (mp->ttl >0 )
           strcat(mbuf,"<font color=#000000>");
         else
           strcat(mbuf,"<font color=#ff0000>");
         //strcat(mbuf,inet_ntoa(mp->resoAddr));
         //theh = gethostbyaddr(inet_ntoa(mp->resoAddr),strlen(inet_ntoa(mp->resoAddr)),AF_INET);
         theh=0; if (theh) strcat(mbuf,theh->h_name);
         else strcat(mbuf,inet_ntoa(mp->resoAddr));
         //strcat(mbuf,mp->resoAddr);
         strcat(mbuf,"</font></td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy (mbuf,"<td bgcolor=#CCCCCC>");                                           if (mp->ttl >0 )
           strcat(mbuf,mp->status);
         else
           strcat(mbuf,"Expired");
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#EEEEEE>");
         char strmp[256];
         sprintf(strmp,"%d",mp->par1);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         mcli->Send(mbuf,strlen(mbuf));
         strcpy(mbuf,"<td bgcolor=#DDDDDD>");
         sprintf(strmp,"%d",mp->par2);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         strcpy(mbuf,"<td bgcolor=#EEEEEE>");
         sprintf(strmp,"%d",mp->par3);
         strcat(mbuf,strmp);
         strcat(mbuf,"</td>");
         strcat(mbuf,"</tr>\n");
         mcli->Send(mbuf,strlen(mbuf));
        }
         mp=mp->next;
      } 
      strcpy (mbuf,"</table></td>");
      mcli->Send(mbuf,strlen(mbuf));                                            
}

void
resbrow::evmStatus(Ctcp *mcli)
{
   char mbuf[1024];
     
      resoitem *mp = mres.thelist;
      while (mp)
      {
        if (mp->resclass == 3)
          if (mp->ttl>0)
            break;
       mp=mp->next;
      }
      strcpy (mbuf,"<table border=\"0\">");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<td bgcolor=\"#d3dcd3\">\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<h2>Run#");
      mcli->Send(mbuf,strlen(mbuf));
      if (mp)
      {
       lastID=mp->par1;
      }
      sprintf(mbuf,"%d ",lastID);
      mcli->Send(mbuf,strlen(mbuf));
      if (mp)
      {
        strcpy(mbuf,"<font color=\"#0C9C04\">");
        strcat(mbuf," running ");
      }
      else 
      {
        strcpy(mbuf,"<font color=\"#9C0C04\">");
        strcat(mbuf," stopped ");
      }
      mcli->Send(mbuf,strlen(mbuf));

      strcpy(mbuf,"</font></h2>\n");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"</td>");
      mcli->Send(mbuf,strlen(mbuf));
      strcpy (mbuf,"<td bgcolor=\"#a4bdb6\">");
 
      mcli->Send(mbuf,strlen(mbuf));
      strcpy(mbuf,"<h2>Evs ");
      mcli->Send(mbuf,strlen(mbuf));
      if (mp)
      {
        sprintf(mbuf,"%d",mp->par2);
        mcli->Send(mbuf,strlen(mbuf));
      }
      strcpy (mbuf,"</h2></td><td>");
      mcli->Send(mbuf,strlen(mbuf));
      if (mp)
       if (mp->par3)
       {
        sprintf(mbuf,"started on %s",ctime(&mp->par3));
        mcli->Send(mbuf,strlen(mbuf));
       }
      strcpy (mbuf,"</td></tr></table>");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy(mbuf,"<p><p><p>\n");
      mcli->Send(mbuf,strlen(mbuf));
   
      strcpy(mbuf,"<form method=post action=\"cmd\">");                               mcli->Send(mbuf,strlen(mbuf));
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

}
void
resbrow::bodyHeader(Ctcp *mcli)
{
   char mbuf[1024];

      strcpy(mbuf,"<body bgcolor=\"#F5F5F5\" text=\"#000000\"> \n");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy(mbuf,"<h1> Ccb Server status</h1> <p>\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      time_t mtime=time(0);
      strcpy(mbuf,ctime(&mtime));
      strcat(mbuf,"<p><p><p>\n");
      mcli->Send(mbuf,strlen(mbuf));
                                                                                
}
void
resbrow::htmlHeader(Ctcp *mcli)
{
   char mbuf[1024];
      strcpy (mbuf,"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 3.2//EN\">\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf,"<HTML>\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      mcli->Send("<head><title>icab status</title>",32);
 
      strcpy (mbuf,"<meta http-equiv=\"refresh\" content=\"30\"> ");
      mcli->Send(mbuf,strlen(mbuf));
 
 
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
}

void
resbrow::httpHeader(Ctcp *mcli)
{
   char mbuf[1024];
      strcpy (mbuf,"HTTP/1.0 200 OK\n");
      mcli->Send(mbuf,strlen(mbuf));                                                  strcpy (mbuf,"MIME-Version 1.0\n");
      mcli->Send(mbuf,strlen(mbuf));
 
      //strcpy (mbuf,"Connection: close\n");
      //mcli->Send(mbuf,strlen(mbuf));
 
      strcpy (mbuf,"Content-Type: text/html\n\n");
      mcli->Send(mbuf,strlen(mbuf));
}
