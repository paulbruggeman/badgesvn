/*  GIMP header image file format (INDEXED): /home/paul/rvasec-badge-2015-dev/src/assets/lores/bowlball.h  */

static unsigned int width = 16;
static unsigned int height = 16;

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
pixel[0] = header_data_cmap[(unsigned char)data[0]][0]; \
pixel[1] = header_data_cmap[(unsigned char)data[0]][1]; \
pixel[2] = header_data_cmap[(unsigned char)data[0]][2]; \
data ++; }

static char header_data_cmap[256][3] = {
	{  3,  6,  2},
	{ 49, 51, 48},
	{ 84, 86, 83},
	{129,131,128},
	};
static char header_data[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,
	0,0,0,1,2,3,3,3,3,3,2,2,0,0,0,0,
	0,0,1,2,3,3,3,3,3,3,3,3,2,0,0,0,
	0,1,2,3,3,3,3,3,3,3,3,3,2,1,0,0,
	0,1,3,3,3,3,2,3,2,3,3,3,3,2,0,0,
	0,2,3,3,3,3,3,3,3,3,3,3,3,2,0,0,
	0,2,3,3,3,3,3,2,3,3,3,3,3,2,0,0,
	0,2,3,3,3,3,3,3,3,3,3,3,3,2,0,0,
	0,1,3,3,3,3,3,3,3,3,3,3,3,2,0,0,
	0,1,2,3,3,3,3,3,3,3,3,3,2,1,0,0,
	0,0,1,2,3,3,3,3,3,3,3,3,2,0,0,0,
	0,0,0,1,2,3,3,3,3,3,2,2,0,0,0,0,
	0,0,0,0,1,1,2,2,2,2,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
