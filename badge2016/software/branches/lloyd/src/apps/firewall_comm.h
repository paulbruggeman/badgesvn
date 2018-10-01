/*
 * File:   firewall_comm.h
 * Author: aflanagan
 *
 * Created on June 2, 2015, 5:09 PM
 */

#ifndef FIREWALL_COMM_H
#define	FIREWALL_COMM_H

#include "firewall_common.h"
#include "ir.h"

typedef enum {
    // Pairing request
    MSG_SHALL_WE_PLAY_A_GAME,
    // response to pairing request
    MSG_LOVE_TO,
    // send random value
    MSG_WHO_STARTS,
    // tell other badge we got random value
    MSG_LETS_START,
    MSG_SHOT_TAKEN,
    MSG_IM_DEAD, // all my network parts are dead
    MSG_IM_HIT,
    MSG_YOU_MISSED
    //max 2 ^ 5 = 32
} fc_command;

/**
 * master callback for incoming firewall communication. Handles
 * packet decoding and dispatch to appropriate function.
 */
void firewall_ir_cb(struct IRpacket_t p);

/**
 * Send a general firewall command to our paired badge, with
 * associated datum.
 *
 * @param cmd The command to send to the other badge.
 * @param datum Additional info associated with the command.
 */
void fw_send_cmd(fc_command cmd, unsigned short datum);

/*
 * Send a peering firewall command to all badges.
 *
 * @param cmd The command to send to the other badge(s).
 * @param datum Additional info associated with the command.
 */
void fw_send_cmd_all(fc_command cmd, unsigned short datum);

void fw_send_shot(char row, char col);
void fw_send_im_hit(char row, char column);
void fw_send_you_missed(char row, char column);

#endif	/* FIREWALL_COMM_H */

/* Local Variables: */
/* compile-command: "make -k -f ~/Devel/badge2016/software/branches/lloyd/Makefile exec" */
/* compilation-scroll-output: first-error */
/* c-file-style: "k&r" */
/* End: */
