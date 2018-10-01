#include "badge15.h"

enum {
   FW_INIT,
   FW_RED_ON,
   FW_RED_WAIT,
   FW_RED_OFF,
   FW_GREEN_ON,
   FW_GREEN_WAIT,
   FW_GREEN_OFF,
   FW_BLUE_ON,
   FW_BLUE_WAIT,
   FW_BLUE_OFF,
   FW_EXIT,
};

void firewall_app()
{
	static unsigned short state = FW_INIT;
	static unsigned int cnt;

	cnt++;
	switch (state) {
		case FW_INIT:
			cnt = 0;
			state++;
			break;

		/* red */
		case FW_RED_ON:
			red(50);
			state++;
			break;

		case FW_RED_WAIT:
			if (cnt == 100000) state++;
			break;

		case FW_RED_OFF:
			red(0);
			state++;
			break;

		/* green */
		case FW_GREEN_ON:
			green(50);
			state++;
			break;

		case FW_GREEN_WAIT:
			if (cnt == 200000) state++;
			break;

		case FW_GREEN_OFF:
			green(0);
			state++;
			break;

		/* blue */
		case FW_BLUE_ON:
			blue(50);
			state++;
			break;

		case FW_BLUE_WAIT:
			if (cnt == 300000) state++;
			break;

		case FW_BLUE_OFF:
			blue(0);
			state++;
			break;

		case FW_EXIT:
			state = FW_INIT;
			returnToMenus();
			break;

		default:
			break;
	}
};

