/* 
 * File:   screensavers.h
 * Author: morgan
 *
 * Created on May 28, 2016, 11:42 AM
 */

enum {
    STUPID_RECTS_SAVER,
    CARZY_TUNNEL_ANIMATOR_SAVER,
    HACK_THE_DRAGON_SAVER,
    FOR_PRESIDENT,
    SMILEY,
    MATRIX,
    BLUESCREEN,
    DISP_ASSET,
    SCOREBOARD,
};

enum scoreApps {
    SCORE_BADGE_INVADER,
    SCORE_PONG,
    SCORE_TWENTYFORTYEIGHT,
    SCORE_BADGYBIRD,
    SCORE_LANDER,
};

//Set badgeID to 0 for this badge
void setHighScore(enum scoreApps appID, unsigned short score, unsigned short badgeId);


void disp_ir_draw();
void just_the_badge_tips();
void dotty();
void disp_asset_saver();
void hack_the_dragon();
void stupid_rects();
void carzy_tunnel_animator();
void for_president();
void smiley();
void matrix();
void bluescreen();
void scoreBoard();