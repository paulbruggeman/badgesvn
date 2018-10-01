/*
 * Functions for firewall program to communicate with other board.
 *
 * Start:
 */
#include <stdlib.h>   //srand(), rand(), etc.
#include "peripheral/timer.h"
#include "firewall_common.h"
#include "firewall_comm.h"
#include "flash.h"
#include "assets.h"

#define FW_COM_ADDR IR_APP3

extern struct firewall_global_state fw_state;


void firewall_ir_cb(struct IRpacket_t p) {
     // Reject our own packets
     if (p.badgeId == G_sysData.badgeId) {
          return;
     };

     unsigned short data = p.data >> 5;
     fc_command command = p.data & 0x1F;  // lowest 5 bits

     switch (command) {
     case MSG_SHALL_WE_PLAY_A_GAME:
          if (fw_state.state == ST_WAITING_FOR_CONNECT) {
               received_peering(data, ST_WAITING_FOR_OTHER_PLAYER);
          }
          break;
     case MSG_LOVE_TO:
          if (fw_state.state == ST_WAITING_FOR_CONNECT || fw_state.state == ST_WAITING_FOR_OTHER_PLAYER) {
               received_peering(data, ST_WHO_GOES_FIRST);
          }
          break;
     case MSG_WHO_STARTS:
          if (fw_state.state == ST_WAITING_FOR_OTHER_PLAYER || fw_state.state == ST_WHO_GOES_FIRST) {
               received_who_first(data);
          }
          break;
     case MSG_LETS_START:
          if (fw_state.state == ST_WHO_GOES_FIRST) {
               received_lets_start(data); // data not realy used here
          }
          break;
     case MSG_SHOT_TAKEN:
          if (fw_state.state == ST_DISPLAY_DEFENSE || fw_state.state == ST_WAITING || fw_state.state == ST_SHOW_OFF_GRID) {
               received_shot(data);
          }
          break;
     case MSG_IM_HIT:
          if (fw_state.state == ST_SENDING_SHOT) {
               received_confirmation(TRUE, data);
          }
          break;
     case MSG_YOU_MISSED:
          if (fw_state.state == ST_SENDING_SHOT) {
               received_confirmation(FALSE, data);
          }
          break;
     case MSG_IM_DEAD:
          if (fw_state.state == ST_SENDING_SHOT) {
               received_victory();
          }
          break;
     }

} // firewall_ir_cb()


void fw_send_cmd(fc_command cmd, unsigned short payload) {
    union IRpacket_u upkt;
    
    upkt.p.data = payload << 5 | cmd;
    upkt.p.address = FW_COM_ADDR;
    
    //upkt.p.badgeId = fw_state.other_badge_id;
    upkt.p.badgeId = 0;
    upkt.p.command = IR_WRITE;
    IRqueueSend(upkt);
} // fw_send_cmd()


void fw_send_cmd_all(fc_command cmd, unsigned short datum) {
    union IRpacket_u upkt;

    upkt.p.data = datum << 5 | cmd;
    upkt.p.address = FW_COM_ADDR;
    upkt.p.badgeId = 0; //send to everybody
    upkt.p.command = IR_WRITE;
    IRqueueSend(upkt);
} // fw_send_cmd_all()


/* Local Variables: */
/* compile-command: "make -k -f ~/Devel/badge2016/software/branches/lloyd/Makefile exec" */
/* compilation-scroll-output: first-error */
/* c-file-style: "k&r" */
/* End: */
