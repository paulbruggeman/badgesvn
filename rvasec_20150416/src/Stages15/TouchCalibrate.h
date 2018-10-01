/* 
 * File:   TouchCalibrate.h
 * Author: morgan
 *
 * Created on January 21, 2015, 8:45 PM
 */

#ifndef TOUCHCALIBRATE_H
#define	TOUCHCALIBRATE_H
#include "../badge15.h"
#include "nokia.h"

DECLARE_BASE_STATE(TouchCalibrate)

// some extra helper functions, not handlers
unsigned char TouchCalibrate15_calibrateSide(struct BadgeState *b_state);
unsigned char TouchCalibrate15_calibrateBottom(struct BadgeState *b_state);

#endif	/* TOUCHCALIBRATE_H */

