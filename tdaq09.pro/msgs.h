#ifndef _msgs_
#define _msgs_

#define uint int
#define ushort short

typedef struct 
    {
      int token;
      short firstboard;
      short last;
      short crateID;
      short spare;
   } initpar;

typedef struct
   {
   int token;
   uint mask1;
   uint mask2;
   uint chans;
   ushort thre;
   ushort fullscale;
   short offset;
   short coff[16];
   short cthre[16];
 } dedpar;

typedef struct
   {
      int token;
   ushort mode;
   ushort trigmode;
   ushort triga1;
   ushort triga2;
   ushort npre,npost,lat;
}  trigpar;


#endif
