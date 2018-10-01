#include "ir.h"
#include "fb.h"
#include "TimeDelay.h"
extern unsigned char outgoing_message[];


void messenger_set_outgoing_msg_char(unsigned char idx,
                                     unsigned char c){
    if(idx > MAX_MSG_LENGTH)
       return;

    outgoing_message[idx] = MapCCharToLCDCharIdx(c);
}

void messenger_send_outgoing_msg(unsigned char relay_amount){
    if(relay_amount > 15)
        relay_amount = 15;

    //keep calling while still more to send
    //msg_send_msg(outgoing_message, relay_amount, 0);
    while(msg_send_msg(outgoing_message, relay_amount, 0))
    {
        PowerSaveIdle();
    }
}
