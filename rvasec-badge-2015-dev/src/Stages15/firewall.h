#ifndef FIREWALL_H
#define FIREWALL_H

void firewall_app(void);

typedef enum {
    FIREWALL_WELCOME,
    FIREWALL_INSTR,
    FIREWALL_INSTR2,
    FIREWALL_CONNECT, // waiting to peer
    FIREWALL_PICK1ST, // peered: who gets first move?
    FIREWALL_LAYOUT,
    FIREWALL_OFFENSE,
    FIREWALL_OFFENSE_RESULT,
    FIREWALL_DEFENSE,
    FIREWALL_DEFENSE_RESULT,
    FIREWALL_LOST,
    FIREWALL_WON,
    FIREWALL_EXIT
} firewall_state;


/**
 * Write out zero-terminated string beginning at pixels (x, y) witn color color. Terminates
 * when '\0' reached or there is no space on display for another character.
 */
void writelinez(char * charin, unsigned char x, const unsigned char y,
        const unsigned short color);

void display_welcome(void);
void display_help(void);
void display_help2(void);
void display_layout(void);
void defend_firewall(void);
void display_grid(void);
void attack_firewall(void);
void show_victory(void);
void show_loss(void);
void show_shots(void);
void record_hit(char row, char column);
void record_miss(char row, char column);
void pick_first(void);

#endif
