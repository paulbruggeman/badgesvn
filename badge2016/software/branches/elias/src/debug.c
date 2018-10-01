#include "debug.h"

unsigned char G_debugBuffer[MAXDEBUGBUFFER+8];
int G_debugLevel=0;
int G_debugBufferFilled=0;

void debugLevelSet(int level)
{
    G_debugLevel = level;
}

void debugString(int level, unsigned char *string)
{
   int len;

   if (level >= G_debugLevel) {
	len = strlen(string);
	if (G_debugBufferFilled+len+1 < MAXDEBUGBUFFER) {
	   strcat(G_debugBuffer, string);
	   G_debugBufferFilled += len;
	}
   }
}

void debugChar(int level, unsigned char outChar)
{
   if (level >= G_debugLevel) {
	if (G_debugBufferFilled+1 < MAXDEBUGBUFFER) {
	   G_debugBuffer[G_debugBufferFilled++] = outChar;
	}
   }
}


