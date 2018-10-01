#define gp_server_1_width 16
#define gp_server_1_height 80

const static char server_1_data_cmap[10][3] = {
	{  0,  0,  0},
	{127,  0,  0},
	{255,  0,  0},
	{  0,127,  0},
	{102,102,102},
	{130,127,  0},
	{178,178,178},
	{  0,255,  0},
	{255,255,  0},
	{255,255,255},
	};

const static char server_1_data[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,7,7,6,3,6,3,6,4,4,4,4,4,4,4,0,
	0,7,7,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,5,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,5,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,1,6,1,6,1,6,4,4,4,4,4,4,4,0,
	0,6,1,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,6,6,6,6,6,6,6,4,4,4,4,4,4,4,0,
	0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,7,7,6,3,6,3,6,4,4,4,4,4,4,4,0,
	0,7,7,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,5,6,3,6,7,7,4,0,4,0,4,0,4,0,
	0,6,5,6,3,6,7,7,4,0,4,0,4,0,4,0,
	0,6,1,6,1,6,1,6,4,4,4,4,4,4,4,0,
	0,6,1,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,6,6,6,6,6,6,6,4,4,4,4,4,4,4,0,
	0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,7,7,6,3,6,7,7,4,4,4,4,4,4,4,0,
	0,7,7,6,3,6,7,7,4,0,4,0,4,0,4,0,
	0,6,5,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,5,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,1,6,1,6,1,6,4,4,4,4,4,4,4,0,
	0,6,1,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,1,6,4,0,4,0,4,0,4,0,
	0,6,6,6,6,6,6,6,4,4,4,4,4,4,4,0,
	0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,6,3,6,3,6,3,6,4,4,4,4,4,4,4,0,
	0,6,3,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,8,8,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,8,8,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,2,2,4,4,4,4,4,4,4,0,
	0,2,2,6,1,6,2,2,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,2,2,4,0,4,0,4,0,4,0,
	0,2,2,6,1,6,2,2,4,0,4,0,4,0,4,0,
	0,6,6,6,6,6,6,6,4,4,4,4,4,4,4,0,
	0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,6,3,6,3,6,3,6,4,4,4,4,4,4,4,0,
	0,6,3,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,8,8,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,8,8,6,3,6,3,6,4,0,4,0,4,0,4,0,
	0,6,1,2,2,2,1,6,4,4,4,4,4,4,4,0,
	0,6,1,2,2,2,1,6,4,0,4,0,4,0,4,0,
	0,6,1,2,2,2,1,6,4,0,4,0,4,0,4,0,
	0,6,1,2,2,2,1,6,4,0,4,0,4,0,4,0,
	0,6,6,6,6,6,6,6,4,4,4,4,4,4,4,0,
	0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,6,0,0,0,0,0,0,0,0,0,0,0,0,6,0,
	0,6,0,6,6,6,6,6,6,6,6,6,6,0,6,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};