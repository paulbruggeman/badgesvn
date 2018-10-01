unsigned char max_configurable_relay_amount;

void badge_messenger_cb();

void msg_set_edit_msg();
void msg_set_relay();
void msg_begin_send();
void map_to_c_chars(unsigned char lcd_str[],
                    unsigned char to_c_str[],
                    unsigned char len,
                    unsigned char fill);

unsigned char msg_send_msg(unsigned char send_msg[], unsigned char relay_cnt,
                           unsigned reset);
void msg_clear_menu();
void msg_clear_msg();
void msg_set_exit();
void msg_populate_menu();
void msg_disp_message();
