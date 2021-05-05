#include <iostream.h>
#include "Cresoitem.h"

#define kMaxttl 10

//static resoitem *resoitem::startoflist=0;

resoitem::resoitem(resoitem **startoflist)
{
   resoitem * temp= *startoflist;

   if (!temp) 
    { 
        
        *startoflist = this;
        prev =0;
        next = 0;
    }
   else
    {
       temp->prev = this;
       prev = 0;
       next  =  temp;
       *startoflist = this;
    }
}

resoitem::~resoitem()
{ 
  if (prev)
    prev->next = next;
}
   
resolist::resolist()
{
   thelist=NULL;
}

resolist::~resolist()
{
   resoitem * temp= thelist;
   resoitem * next;
   while (temp)
   {
     next = temp->next;
     delete temp;
     temp = next;
   }
}

void
resolist::Addresoitem(in_addr newAddr,long newId,long newclass,char *newstatus, 
          long p1, long p2, long p3, long p4)
{
   resoitem * temp= thelist;

   while (temp)
   {
    if ((temp->resId == newId) && (temp->resclass == newclass)) break;
    temp=temp->next;
   }
   
   if (!temp) 
    { 
       temp = new resoitem(&thelist);
        
     //cout << "Created new resource" << endl;
    }
  else 
     //cout << "Found resource" << temp->resId <<  endl;
  bcopy (&newAddr,&(temp->resoAddr),sizeof(newAddr)); 
  temp->resclass = newclass;
  bcopy (newstatus,temp->status,80); 
  temp->ttl = kMaxttl;
  temp->resId=newId;
  temp->par1=p1;
  temp->par2=p2;
  temp->par3=p3;
  temp->par4=p4;
}


int
resolist::printlist()
{
   resoitem *temp=thelist;
 
  ttlupdate();
  while(temp)
  {
   if (temp->ttl<=0) cout << "Expired! ";
 
   cout << "class " << temp->resclass 
        << " Addr: " << inet_ntoa(temp->resoAddr)
        << " status: " << temp->status
        << " ttl: " << temp->ttl 
        << " par1: " << temp->par1
        << " par2: " << temp->par2
        << endl;

   temp=temp->next;
  }
  cout << endl;
}

int
resolist::ttlupdate()
{
  resoitem *temp=thelist;

  while(temp)
  {
   if (temp->ttl!=0) temp->ttl--;
   temp=temp->next;
  }
   
}
