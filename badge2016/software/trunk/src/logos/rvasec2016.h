#define rvasec2016_width 132
//#define rvasec2016_height 53
#define rvasec2016_height 56
const static char rvasec2016_data_cmap[16][3] = {
{ 2, 5, 1 },
{ 51, 70, 50 },
{ 101, 100, 102 },
{ 15, 156, 44 },
};
const static char rvasec2016_data[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0xaa,0xaa,0xa4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x6a,0xaa,0xa9,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x25,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x2a,0xaa,0x88,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x10,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x01,0x10,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x05,0x60,0x00,0x00,0x00,0x00,0x40,0x00,0x01,0x10,0x08,0x00,0x55,0x00,0x05,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x10,0x04,0x01,0x00,0x40,0x20,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x00,0x15,0x54,0x40,0x00,0x01,0x10,0x08,0x08,0x00,0x80,0x20,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x40,0x09,0x80,0x00,0x01,0x10,0x08,0x04,0x00,0x10,0x80,0x02,0x00,0x00,0x00,0x00,0xaa,0xa8,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x00,0x01,0x80,0x00,0x01,0x10,0x04,0x20,0x00,0x22,0x00,0x00,0x80,0x00,0x00,0x01,0x40,0x08,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x00,0x01,0x40,0x00,0x01,0x10,0x04,0x20,0x00,0x0a,0x00,0x00,0x80,0x00,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x00,0x00,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x00,0x01,0x40,0x00,0x01,0x10,0x04,0x20,0x00,0x09,0x00,0x00,0x80,0x00,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x00,0x0a,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x00,0x01,0x80,0x00,0x01,0x10,0x04,0x20,0x00,0x09,0x00,0x00,0x80,0x00,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x00,0xa2,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x00,0x01,0x80,0x00,0x01,0x10,0x04,0x20,0x00,0x09,0x00,0x00,0x80,0x00,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x02,0x00,0x01,0x40,0x00,0x01,0x00,0x04,0x20,0x00,0x09,0x00,0x00,0x80,0x00,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x9a,0xa2,0x00,0x01,0x80,0x00,0x01,0x10,0x08,0x20,0x00,0x09,0x00,0x00,0x80,0x00,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x21,0x00,0x22,0x00,0x01,0x80,0x00,0x01,0x10,0x08,0x20,0x00,0x09,0x00,0x00,0x86,0xa0,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x22,0x00,0x01,0x80,0x00,0x01,0x10,0x04,0x20,0x00,0x09,0x00,0x00,0x84,0x04,0x00,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x12,0x00,0x01,0x80,0x00,0x01,0x00,0x08,0x20,0x00,0x09,0x00,0x00,0x84,0x00,0xaa,0x01,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x12,0x00,0x01,0x40,0x00,0x01,0x10,0x08,0x20,0x00,0x09,0x00,0x00,0x84,0x01,0x40,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x21,0x00,0x12,0x00,0x01,0x40,0x00,0x01,0x10,0x08,0x20,0x00,0x09,0x00,0x00,0x84,0x01,0x41,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x21,0x00,0x12,0x00,0x01,0x40,0x00,0x01,0x00,0x04,0x20,0x00,0x09,0x00,0x00,0x84,0x01,0x01,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x21,0x00,0x12,0x00,0x01,0x40,0x00,0x01,0x00,0x08,0x20,0x00,0x09,0x00,0x00,0x84,0x01,0x41,0x02,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x21,0x00,0x22,0x00,0x00,0x80,0x00,0x01,0x10,0x04,0x20,0x00,0x09,0x00,0x00,0x84,0x06,0x01,0x01,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x88,0x00,0x00,0x00,0x20,0x00,0x12,0x00,0x00,0x80,0x00,0x01,0x00,0x08,0x20,0x00,0x0a,0x00,0x00,0x84,0x01,0x01,0x01,0x00,0x08,0x00,0x00,
0x00,0x00,0x02,0x02,0x20,0x00,0x98,0x00,0x00,0x00,0x21,0x00,0x22,0x00,0x01,0x80,0x00,0x02,0x50,0x04,0x60,0x00,0x0a,0x00,0x00,0x48,0x0a,0x01,0x82,0x00,0x08,0x00,0x00,
0xaa,0xaa,0xa8,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x0a,0x00,0x08,0x00,0x02,0x40,0x00,0x01,0x80,0x02,0x80,0x00,0x09,0x00,0x00,0x24,0x05,0x00,0x44,0x00,0x01,0x55,0x54,
0x55,0x55,0x55,0x55,0x54,0x05,0x40,0x00,0x00,0x00,0x17,0x41,0x55,0x55,0x55,0x55,0x40,0x1f,0xff,0xff,0xff,0xff,0x4f,0xff,0xff,0xff,0xff,0xf0,0x3f,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xfd,0x07,0xd0,0x00,0x00,0x00,0x7d,0x03,0xff,0xff,0xff,0xff,0xf0,0x7f,0xff,0xff,0xff,0xff,0x4f,0xff,0xff,0xff,0xff,0xf0,0xff,0xff,0xff,0xff,0xff,
0xf5,0x55,0x55,0x55,0x7d,0x01,0xf4,0x00,0x00,0x01,0xf4,0x07,0xd5,0x55,0x55,0x55,0xf4,0xff,0xff,0xff,0xff,0xfd,0x0f,0xff,0xff,0xff,0xff,0xd0,0xff,0xff,0xff,0xff,0xfd,
0xf0,0x00,0x00,0x00,0x1d,0x00,0x7f,0x00,0x00,0x07,0xd0,0x07,0xc0,0x00,0x00,0x00,0xf4,0xf4,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x00,
0xf0,0x00,0x00,0x00,0x1d,0x00,0x1f,0x40,0x00,0x1f,0x40,0x07,0xc0,0x00,0x00,0x00,0x74,0xff,0xff,0xff,0xff,0xf4,0x0f,0xff,0xff,0xff,0xff,0xd0,0xf0,0x00,0x00,0x00,0x00,
0xf5,0x55,0x55,0x55,0x7d,0x00,0x07,0xf0,0x00,0x7d,0x00,0x07,0xd5,0x55,0x55,0x55,0xf4,0x7f,0xff,0xff,0xff,0xff,0x0f,0xff,0xff,0xff,0xff,0xf0,0xf0,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xfd,0x00,0x01,0xfc,0x01,0xf4,0x00,0x07,0xff,0xff,0xff,0xff,0xf4,0x17,0xff,0xff,0xff,0xff,0x4f,0xff,0xff,0xff,0xff,0xd0,0xf0,0x00,0x00,0x00,0x00,
0xf5,0x55,0x55,0x57,0xd0,0x00,0x00,0x7f,0x07,0xd0,0x00,0x07,0xd5,0x55,0x55,0x55,0xf4,0x00,0x00,0x00,0x00,0x0f,0x4f,0x00,0x00,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0x00,
0xf0,0x00,0x00,0x01,0xf0,0x00,0x00,0x1f,0xdf,0xc0,0x00,0x07,0xc0,0x00,0x00,0x00,0x74,0x55,0x55,0x55,0x55,0x5f,0x4f,0x55,0x55,0x55,0x55,0x50,0xf5,0x55,0x55,0x55,0x55,
0xf0,0x00,0x00,0x00,0x7c,0x00,0x00,0x07,0xfd,0x00,0x00,0x07,0xc0,0x00,0x00,0x00,0xf4,0xff,0xff,0xff,0xff,0xff,0x0f,0xff,0xff,0xff,0xff,0xf0,0xff,0xff,0xff,0xff,0xff,
0xf0,0x00,0x00,0x00,0x3d,0x00,0x00,0x01,0xf4,0x00,0x00,0x07,0x40,0x00,0x00,0x00,0x74,0xff,0xff,0xff,0xff,0xfc,0x0f,0xff,0xff,0xff,0xff,0xf0,0x3f,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xaa,0xaa,0xaa,0x08,0x6a,0xaa,0xaa,0x48,0x00,0x00,0x22,0xa0,0x00,0x00,0x1a,0x4a,0xaa,0xaa,0x92,0xa0,0x00,0x08,0x6a,0xaa,0xaa,0x40,0x28,0x00,0x00,0xa0,0xaa,0xaa,0xaa,
0x90,0x00,0x02,0x48,0xa0,0x00,0x00,0x08,0x00,0x00,0x22,0xaa,0x80,0x02,0xaa,0x88,0x00,0x00,0x62,0xa9,0x00,0x08,0xa0,0x00,0x02,0x80,0x0a,0x00,0x02,0x80,0x80,0x00,0x02,
0xa0,0x00,0x02,0x48,0xa0,0x00,0x00,0x0a,0x95,0x55,0xa2,0x02,0xa9,0x6a,0x42,0x88,0x00,0x00,0x22,0x06,0x90,0x08,0xa0,0x00,0x01,0x80,0x02,0x80,0x0a,0x00,0x90,0x00,0x02,
0xaa,0xaa,0xaa,0x08,0xa0,0x00,0x00,0x0a,0xaa,0xaa,0xa2,0x00,0x0a,0xa0,0x02,0x88,0x00,0x00,0x22,0x40,0x6a,0x08,0xa0,0x00,0x01,0x80,0x00,0xa0,0x28,0x00,0xaa,0xaa,0xaa,
0xa0,0x00,0x08,0x08,0xa0,0x00,0x00,0x08,0x00,0x00,0x22,0x00,0x00,0x00,0x02,0x88,0x00,0x00,0x22,0x40,0x06,0xa8,0xa0,0x00,0x02,0x80,0x00,0x28,0xa0,0x00,0x90,0x00,0x02,
0x90,0x00,0x0a,0x08,0x6a,0xaa,0xaa,0x88,0x00,0x00,0x22,0x00,0x00,0x00,0x02,0x8a,0xaa,0xaa,0xa2,0x40,0x00,0x68,0xaa,0xaa,0xaa,0x88,0x00,0x0a,0x80,0x00,0x80,0x00,0x02,
};