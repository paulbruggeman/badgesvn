#define gp_footswitch_width 16
#define gp_footswitch_height 14
const static char gp_footswitch_data_cmap[4][3] = {
{ 32, 32, 142 },
{ 55, 55, 232 },
{ 107, 107, 107 },
{ 255, 255, 255 },
};

const static char gp_footswitch_data[] = {
0xff,0xaa,0xaa,0xbf,
0xfe,0x55,0x55,0xbf,
0xfe,0x55,0x55,0xbf,
0xfe,0x55,0x55,0xbf,
0xfa,0xaa,0xaa,0xaf,
0xea,0xaa,0xaa,0xab,
0xaa,0xaa,0xaa,0xaa,
0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,
0xfe,0xaa,0xaa,0xbf,
0xfe,0x00,0x00,0xbf,
0xfa,0xaa,0xaa,0xaf,
0xea,0xaa,0xaa,0xab,
0xaa,0xaa,0xaa,0xaa,
};
