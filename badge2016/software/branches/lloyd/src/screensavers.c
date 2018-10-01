#include "colors.h"
#include "ir.h"
#include "assetList.h"
#include "touchCTMU.h"
#include "fb.h"
#include "screensavers.h"
#include "prngs.h"
#include "flash.h"

extern unsigned int G_currentstamp;
extern unsigned int G_touchstamp;
extern unsigned short popup_time;

unsigned short anim_cnt = 0;
extern unsigned char draw_moved;

#define DRAW_IR_ENUM 1
void draw_render();
void disp_ir_draw(){
    draw_moved = 1;
    draw_render(DRAW_IR_ENUM);
    //FbMove(10, 10);
    //FbRectangle(10,10);
    FbSwapBuffers();
}

typedef struct __attribute__((packed)){
    unsigned short badgeID;
    unsigned score:12;
} highScore;


#define NUMBER_OF_HIGHSCORES 5
int score_transmission = 0;
highScore highScores[NUMBER_OF_HIGHSCORES] = {};


void ir_highscore(struct IRpacket_t p) {
    highScore hs;
    hs.badgeID = p.badgeId;
    
    unsigned short data = p.data;
    hs.score = data & 0b0000111111111111;
    unsigned char appid;
    appid = (data & 0b1111000000000000) >> 12;
    
    setNote(75, 256);
    
    setHighScore(appid,hs.score,hs.badgeID);
};

void sendScorePacket(highScore hs, enum scoreApps apid) {
    //if(hs.badgeID) { return; }
    
    union IRpacket_u pkt;
    
    pkt.p.badgeId = 0;
    pkt.p.command = IR_WRITE;
    
    unsigned short tmpdata = apid << 12;
    tmpdata += hs.score & 0b0000111111111111;
    pkt.p.address = IR_HIGHSCORE;
    
    pkt.p.data = tmpdata;
    
    IRqueueSend(pkt);
    setNote(75, 256);
    
};

void setHighScore(enum scoreApps appID, unsigned short score, unsigned short bid) {
    bid = (bid == 0) ? G_sysData.badgeId : bid;
    
    if(score > highScores[appID].score) {
        highScores[appID].score = score;
        highScores[appID].badgeID = bid;
    }
};
void scoreBoard(){
    switch(anim_cnt) {
    case 0: {
        sendScorePacket(highScores[score_transmission%NUMBER_OF_HIGHSCORES],
                score_transmission%NUMBER_OF_HIGHSCORES);
        score_transmission++;
        if(score_transmission >= NUMBER_OF_HIGHSCORES) {
            score_transmission = 0;
        }
        anim_cnt++;
    }
        break;
    case 1:
#define CHARSIZE 10
#define SCREEN_WIDTH 131
#define SCORE_WRITE(T,I,S) {FbMove(SCREEN_WIDTH - (CHARSIZE*(sizeof(T)-1)) - 5, I*12+2); FbWriteLine(T);  char tmp[10] = {0}; itoa(tmp, S, 10); FbMove(3,I*12+2); FbWriteLine(tmp); I++; }

        FbColor(WHITE);
        FbBackgroundColor(BLACK);
        int i = 1;
        char btmp[3] = {};
        
        itoa(btmp,highScores[SCORE_BADGE_INVADER],10);
        FbMove(1 ,i*12+2);
        FbWriteLine("BADGE INVADER");
        i++;
        SCORE_WRITE(btmp, i, highScores[SCORE_BADGE_INVADER].score);


        itoa(btmp,highScores[SCORE_TWENTYFORTYEIGHT].badgeID,10);
        FbMove(1 ,i*12+2);
        FbWriteLine("GAME 2048");
        i++;
        SCORE_WRITE(btmp, i, highScores[SCORE_TWENTYFORTYEIGHT].score);
        
        itoa(btmp,highScores[SCORE_BADGYBIRD].badgeID,10);
        FbMove(1 ,i*12+2);
        FbWriteLine("BADGY BIRD");
        i++;
        SCORE_WRITE(btmp, i, highScores[SCORE_BADGYBIRD].score);
        
        itoa(btmp,highScores[SCORE_LANDER].badgeID,10);
        FbMove(1 ,i*12+2);
        FbWriteLine("LANDER");
        i++;
        SCORE_WRITE(btmp, i, highScores[SCORE_LANDER].score);
        
                
        itoa(btmp,highScores[SCORE_PONG].badgeID,10);
        FbMove(1 ,i*12+2);
        FbWriteLine("PONG");
        i++;
        SCORE_WRITE(btmp, i, highScores[SCORE_PONG].score);
        
        //DRAW SHIT
        FbSwapBuffers();
        break;
    }
}

void disp_asset_saver(){
    unsigned char x, y;
    static unsigned char imgnum = 0;
    if(!anim_cnt){
        imgnum = quick_rand(G_currentstamp + popup_time) % 2;
        anim_cnt++;
    }
    //imgnum = 2;

    switch(imgnum){
        case 0:
            FbMove(0,0);
            FbImage(HACKRVA4, 0);
            break;

//        case 1:
//            FbMove(0,20);
//            FbImage(RVASEC2016, 0);
//            break;

        case 1:
            for(x=0; x<126; x+=20)
                for(y=0; y<126; y+=8){
                    FbColor(BLUE << ((x>>4) + (y>>4)));
                    FbMove(x, y);
                    FbImage(BI_SPRITE, 0);
                }
            break;
//        case 2:
//            for(x=0; x<126; x+=20)
//                for(y=0; y<126; y+=14){
//                    //FbColor(BLUE << ((x>>4) + (y>>4)));
//                    FbMove(x, y);
//                    FbImage(BADGEY_BIRD, 0);
//                }
//            break;
    }

    FbSwapBuffers();
}

const unsigned char drag_hack[] = "Hack the Dragon";
const unsigned char drag_hack_num[] = "804-415-4260";
void hack_the_dragon(){

    red(100);
    FbMove(5, 20);
    FbColor(RED);
    FbWriteLine(drag_hack);

    unsigned int i = 0;
    for(i=0; i<(8 - popup_time%8); i++){
        FbMove(17, 35+ (i*10));
        FbWriteLine(drag_hack_num);
    }

    FbSwapBuffers();

}

void stupid_rects(){
    unsigned int rnd = irbit2(G_currentstamp);
    if(rnd < 10){
        rnd = irbit2(~G_currentstamp);
    }
    //rnd = irbit2(rnd);
    anim_cnt++;

    if(anim_cnt==1){
        FbColor(YELLOW);
        FbMove(rnd%20+rnd%20,rnd%70+rnd%30);

        FbFilledRectangle(rnd%80, rnd%20);
        red(100);
        green(100);
        blue(0);
    }
    else if(anim_cnt == 200){
        FbColor(GREEN);
        FbMove(rnd%60+rnd%60,rnd%55+rnd%10);

        FbFilledRectangle(rnd%10, rnd%50);
        red(0);
        green(100);
        blue(0);
    }
    else if(anim_cnt == 400){
        FbColor(CYAN);
        FbMove(rnd%70+rnd%45,rnd%45+rnd%33);

        FbFilledRectangle(rnd%25, rnd%30);
        red(0);
        green(100);
        blue(100);
    }
    else if(anim_cnt == 600){

        FbColor(WHITE);
        FbMove(rnd%30+rnd%10,rnd%30+rnd%30);

        FbFilledRectangle(rnd%70, rnd%20);
        red(100);
        green(100);
        blue(100);
    }
    else if(anim_cnt == 800){

        FbColor(B_BLUE2);
        FbMove(rnd%50+rnd%30,rnd%10+rnd%15);

        FbFilledRectangle(rnd%30, rnd%50);
        red(0);
        green(0);
        blue(100);
    }
    else if(anim_cnt == 1000){

        FbColor(MAGENTA);
        FbMove(rnd%33+rnd%47,rnd%65+rnd%33);

        FbFilledRectangle(rnd%40, rnd%30);
        red(50);
        green(0);
        blue(100);
    }
    else if(anim_cnt > 1199)
        anim_cnt = 0;

    FbPushBuffer();
}

#define TUNNEL_COLOR CYAN
void carzy_tunnel_animator(){
    //static unsigned short anim_cnt = 0;
    anim_cnt++;

    if(!anim_cnt){
        FbColor(TUNNEL_COLOR);
        FbMove(65,65);

        FbRectangle(2, 2);
        red(0);
        green(1);
        blue(1);
    }
    else if(anim_cnt == 200){
        FbColor(TUNNEL_COLOR);
        FbMove(64,64);

        FbRectangle(4, 4);
        red(0);
        green(5);
        blue(5);
    }
    else if(anim_cnt == 400){
        FbColor(TUNNEL_COLOR);
        FbMove(62,62);

        FbRectangle(8, 8);
        red(0);
        green(20);
        blue(20);
    }
    else if(anim_cnt == 600){
        FbColor(TUNNEL_COLOR);
        FbMove(58,58);

        FbRectangle(16, 16);
        red(0);
        green(45);
        blue(45);
    }
    else if(anim_cnt == 800){
        FbColor(TUNNEL_COLOR);
        FbMove(50,50);

        FbRectangle(32, 32);
        red(0);
        green(75);
        blue(75);
    }
    else if(anim_cnt == 1000){
        FbColor(TUNNEL_COLOR);
        FbMove(34,34);

        FbRectangle(64, 64);
        red(0);
        green(100);
        blue(100);
    }
    else if(anim_cnt > 1199)
        anim_cnt = 0 - 1;

    FbSwapBuffers();
}

void dotty(){
    unsigned int rnd = irbit2(G_currentstamp);
    if(anim_cnt == 0){
        FbClear();
    }

    //if(anim_cnt%5){
    unsigned char i = 0;
    for(i = 0; i < 200; i++)
    {
        FbColor(BLUE << (anim_cnt>>4));
        //FbPoint(rnd%130, irbit2(~G_currentstamp)%130);
        //FbPoint(irbit2(get_rand_char(0, 132) + G_currentstamp + i),
        //        irbit2(get_rand_char(0, 132) + ~G_currentstamp + i));

        FbMove(irbit2(get_rand_char(0, 132) + G_currentstamp + i),
                irbit2(get_rand_char(0, 132) + ~G_currentstamp + i));
        FbFilledRectangle(3, 3);
    }

    FbPushBuffer();
    anim_cnt += 2;

}

const unsigned char president1[] = "HAL FOR";
const unsigned char president2[] = "president";
const unsigned char president3[] = "Badge for";
const unsigned char president4[] = "Vice President";
void for_president(){

    FbMove(20, 17);
    FbColor(WHITE);
    FbWriteLine(president1);

    FbMove(30, 35);
    FbWriteLine(president2);

    if(popup_time > 30){
        unsigned char i = 0;
        for(i=0; i<8; i++){
            FbColor((i+anim_cnt)%2 ? WHITE: RED);
            FbMove(0, 50 + (i*10));
            FbFilledRectangle(132, 10);
        }
    }
    else{
        FbColor(WHITE);
        FbMove(30, 70);
        FbWriteLine(president3);

        FbMove(10, 80);
        FbWriteLine(president4);
    }
    anim_cnt++;
    FbSwapBuffers();
    blue(100);

}

void smiley(){

    FbColor(WHITE);

    FbMove(10, 10);
    FbFilledRectangle(20, 35);
    FbMove(100, 10);
    FbFilledRectangle(20, 35);
    FbMove(20, 80);
    FbFilledRectangle(80, 5);

    FbColor(BLACK);

    if(popup_time > 40)
        FbMove(15, 20);
    else if(popup_time > 30)
        FbMove(12, 15);
    else if(popup_time > 20)
        FbMove(19, 15);
    else
        FbMove(16, 25);

    FbFilledRectangle(7, 7);

    if(popup_time > 40)
        FbMove(105, 20);
    else if(popup_time > 30)
        FbMove(102, 15);
    else if(popup_time > 20)
        FbMove(109, 15);
    else
        FbMove(106, 25);

    FbFilledRectangle(7, 7);


    FbColor(RED);

    if(popup_time > 20){
        FbMove(60, 85);
        FbFilledRectangle(30, 20);
        FbMove(65, 105);
        FbFilledRectangle(20, 7);

    }
    else{
        FbMove(40+popup_time, 85);
        FbFilledRectangle(30, popup_time);
        FbMove(45+popup_time, 85+popup_time);
        FbFilledRectangle(20, 7);
    }


    FbSwapBuffers();
}

void matrix(){
    unsigned char w = 0, h = 0;
    FbColor(GREEN);
    for(w = 0; w <132; w+=10){
        for(h = 0; h < 132; h+=10){
            FbMove(w, h);
            FbCharacter(lcd_font_map[irbit2(popup_time+w+h+G_currentstamp)%42]);
        }
    }
    FbSwapBuffers();
}

const unsigned char bs1[] = "Badgedows";
const unsigned char bs2[] = "An error occured";
const unsigned char bs3[] = "Give up to";
const unsigned char bs4[] = "continue";
void bluescreen(){
    FbColor(BLUE);
    FbMove(0,0);
    FbFilledRectangle(132, 132);

    FbColor(WHITE);
    FbMove(23, 10);
    FbFilledRectangle(75, 10);

    FbMove(2, 40);
    FbWriteLine(bs2);

    if(popup_time < 40){
        FbMove(2, 60);
        FbWriteLine(bs3);

        FbMove(15, 70);
        FbWriteLine(bs4);
    }

    FbColor(BLUE);
    FbMove(25, 11);
    FbWriteLine(bs1);
    FbSwapBuffers();
}


const unsigned char badgetips_header[] = "--Badge Tip--";
//const unsigned char badgetip_more_you_know = 
void just_the_badge_tips(){
    static unsigned char tipnum = 0;
    if(!anim_cnt){
        tipnum = quick_rand(G_currentstamp + popup_time) % 19;
        anim_cnt++;
    }

    FbBackgroundColor(BLACK);
    FbColor(GREEN);
    FbMove(12, 4);
    FbWriteLine(badgetips_header);


    FbColor(YELLOW);
    switch(tipnum){
        case 0:
            FbMove(2, 20);
            FbWriteLine("Dont lick badge");
            break;
        case 1:
            FbMove(2, 20);
            FbWriteLine("These are land");
            FbMove(2, 30);
            FbWriteLine("dwelling badges.");
            FbMove(2, 40);
            FbWriteLine("Avoid water");
            break;
        case 2:
            FbMove(2, 20);
            FbWriteLine("Do not leave");
            FbMove(2, 30);
            FbWriteLine("battery in");
            FbMove(2, 40);
            FbWriteLine("when using USB");
            break;
        case 3:
            FbMove(2, 20);
            FbWriteLine("Wash your hands");
            FbMove(2, 30);
            FbWriteLine("next time.");
            break;
        case 4:
            FbMove(2, 20);
            FbWriteLine("Say hi to");
            FbMove(2, 30);
            FbWriteLine("the creators");
            FbMove(2, 40);
            FbWriteLine("over at hackrva");
            break;
        case 5:
            FbMove(2, 20);
            FbWriteLine("Have another");
            FbMove(2, 30);
            FbWriteLine("beer.");
            break;
        case 6:
            FbMove(2, 20);
            FbWriteLine("Jet fuel cant");
            FbMove(2, 30);
            FbWriteLine("melt steel beams.");
            break;
        case 7:
            FbMove(2, 20);
            FbWriteLine("Sit down and");
            FbMove(2, 30);
            FbWriteLine("   STFU");
            break;
        case 8:
            FbMove(2, 20);
            FbWriteLine("checkout the");
            FbMove(2, 30);
            FbWriteLine("   CTF!");
            break;
        case 9:
            FbMove(2, 20);
            FbWriteLine("Its impolite");
            FbMove(2, 30);
            FbWriteLine("to stare at");
            FbMove(2, 40);
            FbWriteLine("other peoples");
            FbMove(2, 50);
            FbWriteLine("badges.");
            break;
        case 10:
            FbMove(2, 20);
            FbWriteLine("Every badge is");
            FbMove(2, 30);
            FbWriteLine("different. Get");
            FbMove(2, 40);
            FbWriteLine("to know yours!");
            break;
        case 11:
            FbMove(2, 20);
            FbWriteLine("Badges can be");
            FbMove(2, 30);
            FbWriteLine("very social.");
            FbMove(2, 40);
            FbWriteLine("Unlike all of");
            FbMove(2, 50);
            FbWriteLine("you.");
            break;
        case 12:
            FbMove(2, 20);
            FbWriteLine("Unix or bust.");
            break;
        case 13:
            FbMove(2, 20);
            FbWriteLine("If you cant");
            FbMove(2, 30);
            FbWriteLine("read this your");
            FbMove(2, 40);
            FbWriteLine("badge is broken");
            break;
        case 14:
            FbMove(2, 20);
            FbWriteLine("Youll find the");
            FbMove(2, 30);
            FbWriteLine("source code ");
            FbMove(2, 40);
            FbWriteLine("online after");
            FbMove(2, 50);
            FbWriteLine("the conference.");
            break;
        case 15:
            FbMove(2, 20);
            FbWriteLine("Dont touch");
            FbMove(2, 30);
            FbWriteLine("the screen.");
            break;
        case 16:
            FbMove(2, 20);
            FbWriteLine("If you are");
            FbMove(2, 30);
            FbWriteLine("having problems");
            FbMove(2, 40);
            FbWriteLine("with your badge");
            FbMove(2, 50);
            FbWriteLine("go to hackrva");
            FbMove(2, 60);
            FbWriteLine("near the CTF");
            break;
        case 17:
            FbMove(2, 20);
            FbWriteLine("These arent the");
            FbMove(2, 30);
            FbWriteLine("badges your ");
            FbMove(2, 40);
            FbWriteLine("looking for.");
            break;
        case 18:
            FbMove(2, 20);
            FbWriteLine("The badge is a");
            FbMove(2, 30);
            FbWriteLine("dish best served ");
            FbMove(2, 40);
            FbWriteLine("cold.");
            break;
    }

    FbSwapBuffers();
}
