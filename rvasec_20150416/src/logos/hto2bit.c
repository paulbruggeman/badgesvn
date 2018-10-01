#include <stdio.h>


/*

   c .h 8 bit to 2 bit/pixel converter

   Author: Paul Bruggeman
   paul@Killercats.com

 cc -o hto2bit hto2bit.c
*/

#include "hackrva.h"

#define CMAP() hackrva_data_cmap
#define PIXEL() hackrva_data

#define OUTNAME "hackrva2"

/*
#include "rvasec.h"

#define CMAP() rvasec_data_cmap
#define PIXEL() rvasec_data

#define OUTNAME "rvasec2"
*/

main() {
    int r, c;

    fprintf(stdout, "#define %s_width %d\n", OUTNAME, width);
    fprintf(stdout, "#define %s_height %d\n", OUTNAME, height);

    /* output header */
    fprintf(stdout, "const static char %s_data_cmap[16][3] = {\n", OUTNAME);
    for (r=0; r<4; r++) {
        fprintf(stdout, "{ %d, %d, %d },\n", 
                (unsigned char)CMAP()[r][0],
                (unsigned char)CMAP()[r][1],
                (unsigned char)CMAP()[r][3]);
	};
    fprintf(stdout, "};\n");

    fprintf(stdout, "const static char %s_data[] = {\n", OUTNAME);
    for (r=0; r<height; r++) {
        for (c=0; c<width; c+=4) {
            //fprintf(stdout, "%d,", ((unsigned char)(PIXEL()[r*c]) >> 4) );
            fprintf(stdout, "0x%02x,", 
                (unsigned char)( ( (PIXEL()[r*width+c  ] & 0x3) << 6) | ( (PIXEL()[r*width+c+1]) & 0x3) << 4 ) |
                                 ( (PIXEL()[r*width+c+2] & 0x3) << 2) | ( (PIXEL()[r*width+c+3]) & 0x3)      );
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "};\n");

}
