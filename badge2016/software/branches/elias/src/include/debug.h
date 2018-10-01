#ifndef debug_h
#define debug_h


/*
 level -> 0 ... N
 0 is lots of noise
 > 0 should be much less so to avoid swamping 
   USB serial or just overflowing it 
   with too much printing

   set level of noise output with:
   void debugLevelSet(int level);

   when release is done all macros are cleared
   removing any DEBUG prints from the rom
*/

#define MAXDEBUGBUFFER 80

#define DEBUGPRINT
#ifdef DEBUGPRINT
  #define DEBUGSTRLVL(L, X) debugString(L, &(X))
  #define DEBUGSTR(X) debugString(0, &(X))
  #define DEBUGCHAR(X) debugChar(0, (X))
#else
  #define DEBUGSTRLVL(L, X) debugString(0, &(X))
  #define DEBUGSTR(X)
  #define DEBUGCHAR(X)
#endif

#define MAXDEBUGBUFFER 80

enum {
   DNOISE=0,
   DNOTICE,
   DWARN,
   DERROR,
};

extern unsigned char G_debugBuffer[];
extern int G_debugBufferFilled;

void debugLevelSet(int level);
void debugString(int level, unsigned char *string);
void debugChar(int level, unsigned char outChar);

#endif
