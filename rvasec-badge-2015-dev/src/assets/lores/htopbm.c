#include <stdio.h>

/*

   basic c .h file to pbm converter
   Author: Paul Bruggeman
   paul@Killercats.com


pbm ascii is easy to interface to

 cc -o htopbm htopbm.c
 ./htopbm > drbob.pbm


example file: (the 4th line 15 == max pixel val in file)

P3
# feep.ppm
4 4
15
 0  0  0    0  0  0    0  0  0   15  0 15
 0  0  0    0 15  7    0  0  0    0  0  0
 0  0  0    0  0  0    0 15  7    0  0  0
15  0 15    0  0  0    0  0  0    0  0  0

*/

#include "TheCakeIsALie_2bit.h"
#define CMAP TheCakeIsALie_data_cmap
#define PIXEL TheCakeIsALie_data
#define WIDTH TheCakeIsALie_width
#define HEIGHT TheCakeIsALie_height

#define OUTNAME "TheCakeIsALie"

main() {
    int r, c;

    fprintf(stdout, "P3\n");
    fprintf(stdout, "%d %d\n", WIDTH, HEIGHT);
    fprintf(stdout, "255\n"); /* sure that works */

    for (r=0; r < HEIGHT; r++) {
        for (c=0; c < WIDTH; c++) {
            fprintf(stdout, "%u %u %u ", 
		(unsigned int)((unsigned char)CMAP[(unsigned char)(PIXEL[r * WIDTH + c])][0]), 
		(unsigned int)((unsigned char)CMAP[(unsigned char)(PIXEL[r * WIDTH + c])][1]), 
		(unsigned int)((unsigned char)CMAP[(unsigned char)(PIXEL[r * WIDTH + c])][2])
	    );
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "};\n");

}
