/*
 * File:   firewall_comm.h
 * Author: aflanagan
 *
 * Created on June 2, 2015, 5:09 PM
 */

#ifndef FIREWALL_COMM_H
#define	FIREWALL_COMM_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        PLACE_SHOT,
        IM_DEAD, // all my network parts are dead
        IM_HIT,
        YOU_MISSED
    } fc_command;

    struct fw_comm_cmd {
        fc_command command : 4;
        unsigned row : 3;
        unsigned column : 3;
    };

    union fw_comm_cmd_u {
        struct fw_comm_cmd cmd;
        unsigned short int i;
    };
    
    /* master callback for incoming firewall communication. Handles packet
     * decoding and dispatch to appropriate function.
     */
    void fw_ir_cb(struct IRpacket_t p);
    void fw_send_cmd(fc_command cmd, char row, char col);
    void fw_send_shot(char row, char col);
    void fw_send_im_hit(char row, char column);
    void fw_send_you_missed(char row, char column);

#ifdef	__cplusplus
}
#endif

#endif	/* FIREWALL_COMM_H */

