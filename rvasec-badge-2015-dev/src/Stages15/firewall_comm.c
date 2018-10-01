/*
 * Functions for firewall program to communicate with other board.
 *
 * Start:
 * 1. broadcast request to play.
 * 2. request received?
 * 3.   send ack
 * 4. ack received?
 * 5.   save other ID
 * 6.   send paired conf.
 * 7. receive paired conf.
 * 8. board with higher ID randomly picks first player
 * 9. if first player:
 * 10.  go to offense mode
 * 11.  wait for user input
 * 12. else:
 * 13.  go to defense mode
 * 14.  wait for offense shot
 * 15. offense board sends shot info.
 * 16.   wait for ack
 * 17. defense board sends hit/miss/win
 * 18. offense board shows result
 * 19. if hit/miss:
 * 20.   go to 9, switch roles
 * 21. else:
 * 22.    receiver ack win/lose?
 * 23.    display win or lose
 */
#include "badge15.h"
#include "firewall_comm.h"


void fw_ir_cb(struct IRpacket_t p) {
    
    if (p.badgeId != G_peerBadgeId) {
        //not meant for us
        return;
    }

    union fw_comm_cmd_u comm;
    comm.i = p.data;

    // if we're not paired, don't handle anything besides pair request
    switch (comm.cmd.command) {
        case PLACE_SHOT:
            shot_received(comm.cmd.row, comm.cmd.column);
            break;
        case IM_HIT:
            record_hit(comm.cmd.row, comm.cmd.column);
            break;
        case YOU_MISSED:
            record_miss(comm.cmd.row, comm.cmd.column);
            break;
    }

}

void fw_send_cmd(fc_command cmd, char row, char col) {
    union IRpacket_u pkt;
    union fw_comm_cmd_u comm;
    comm.cmd.command = cmd;
    comm.cmd.row = row;
    comm.cmd.column = col;
    pkt.p.data = (short unsigned int)comm.i;
    pkt.p.command = IR_WRITE;
    pkt.p.address = IR_LED;
    pkt.p.badgeId = G_sysData.badgeId;
    IRqueueSend(pkt);
}

void fw_send_shot(char row, char col) {
    fw_send_cmd(PLACE_SHOT, row, col);
}

void fw_send_im_hit(char row, char column) {
    fw_send_cmd(IM_HIT, row, column);
}

void fw_send_you_missed(char row, char column) {
    fw_send_cmd(YOU_MISSED, row, column);
}