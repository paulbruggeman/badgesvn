/*#include "badge_common.h"
#include "Common/Compiler.h"

void b_strcpy(char* dest, const char* src)
{
    unsigned char offset = 0;
    // looking for null
    while(*(src + offset))
    {
        *dest = *(src + offset);
        dest++;
        offset++;
    }
    *dest = 0;
}

void intTime_to_charTime(char* c_time, unsigned int i_time)
{
    unsigned char hours = i_time/10000;
    unsigned char minutes = (i_time - (hours * 10000))/100 ;
    c_time[0] = 48 + (unsigned char) hours / 10;
    c_time[1] = 48 + (unsigned char) hours % 10;
    c_time[2] = ':';
    c_time[3] = 48 + (unsigned char) minutes / 10;
    c_time[4] = 48 + (unsigned char) minutes % 10;
    c_time[5] = 0;
}

void setTimeString(b_rtccTime time, char *t_string)
{
//    t_string[0] = 48 + ((time.hour >>  4) & 0xF);
//    t_string[1] = 48 + ((time.hour      ) & 0xF);
    setBase10String(time.hour, t_string);
    t_string[2] = ':';
    setBase10String(time.min, &t_string[3]);
//    t_string[3] = 48 + ((time.min >>  4) & 0xF);
//    t_string[4] = 48 + ((time.min      ) & 0xF);
}


void setBase10String(unsigned char num, char *t_string)
{
    t_string[0] = 48 + ((num >>  4) & 0xF);
    t_string[1] = 48 + ((num) & 0xF);
}

void bcdDecrement(unsigned char *num)
{
    //not wrapping around
    if( (*num &0xF) != 0)
    {
        //don't touch top bits, but subtract from bottom
        *num = (*num & 0xf0) | ((*num-1) & 0xf);
    }
    else
    {
        //shift down and subtract one, don't want carry, shift back
        *num = (((*num >> 4) - 1) & 0xf) << 4;
        *num |= 0x9;
    }
}

void bcdIncrement(unsigned char *num)
{
    //not wrapping around
    if( (*num &0xF) != 9)
    {
        *num = (*num & 0xf0) | ((*num+1) & 0xf);
    }
    else
    {
        //shift down and add one, don't want carry, shift back
        *num = (((*num >> 4) + 1) & 0xf) << 4;
    }
}

//not BCD, just separated into string, you may be looking for utoa
void uINT_to_chars(unsigned int in, char *out)
{
    out[0] = (in >>24);
    out[1] = (in >>16) & 0xf;
    out[2] = (in >>8) & 0xf;
    out[3] = (in) & 0xf;
}*/

