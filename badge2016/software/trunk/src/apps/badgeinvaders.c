#include "badge16.h"
#include "colors.h"
#include "menu.h"
#include "ir.h"
#include "touchCTMU.h"
#include "badgeinvaders.h"
#include "fb.h"
#include "assetList.h"
#include "screensavers.h"


struct menu_t badge_invaders_config_m[] = {
    {"Play", VERT_ITEM|DEFAULT_ITEM, FUNCTION, 
        {(struct menu_t *)bi_continue}},
    {"Restart", VERT_ITEM, FUNCTION, {(struct menu_t *)bi_init}},
    {"Exit", VERT_ITEM|LAST_ITEM, FUNCTION, {(struct menu_t *)set_bi_exit}}
};

enum
{
    BI_INIT,
    BI_SHOW_MENU,
    BI_LEVEL_INIT,
    BI_GET_INPUT,
    BI_PROCESS_MOVE,
    BI_CHECK_WIN,
    BI_RENDER,
    BI_DELAY,
    BI_WIN,
    BI_GAME_OVER,
    BI_EXIT 
};

enum
{
    BI_NONE,
    BI_MOVE_RIGHT,
    BI_MOVE_LEFT,
    BI_FIRE,
};

char bi_flip = 0;//Determines alien move direction
char bi_player_life = 3;//number of times player can be hit by missile
char bi_kmotion = 5;//the # of pixels the sprites move each frame
char bi_state = BI_SHOW_MENU;//game state
char bi_game_speed;//speed at which aliens move
char bi_player_action = BI_NONE;//player input var
char bi_game_counter = 0;//game counter controls missile differential speed
char bi_game_level = 0;//current level
char bi_render_divider=0;//controls renders cycles for missile
int bi_delay_counter = 0;//walks through missile timing
int bi_delay = 80;//divider for missile time rendering
int missile_counter = 0;//counter for missile timing
int bi_score = 0;//score

//player and alien width = 10 pixels, height = 5 pixels
//game map 26x26 in 5 pixel units
struct bi_alien {
    char alien[5][5];
    char pos_x;
    char pos_y;
    char missile_x;
    char missile_y;
    char old_x;
    char old_y;
};

struct bi_player {
    char pos_x;
    char pos_y;
    char missile_x;
    char missile_y;
    char old_x;
};

struct bi_alien biAlien;
struct bi_player biPlayer;

void bi_level_init() {
    char i;
    if(bi_game_level != 0){
        bi_delay /=bi_game_level;
    }
    else{
        bi_delay = 80;
    }
    missile_counter = bi_delay/(8-bi_game_level);
    biAlien.missile_x = 0;
    biAlien.missile_y = 0;
    biAlien.pos_x = 1;
    biAlien.pos_y = 1;
    for(i=0;i<5;i++){
        biAlien.alien[0][i] = 3;
        biAlien.alien[1][i] = 3;
        biAlien.alien[2][i] = 2;
        biAlien.alien[3][i] = 2;
        biAlien.alien[4][i] = 1+bi_game_level;
    }
    biPlayer.missile_x = 0;
    biPlayer.missile_y = 0;
    biPlayer.pos_x = 13;
    biPlayer.pos_y = 25;
    bi_player_action = BI_NONE;
    bi_game_counter = 0;
    bi_flip = 0;
    bi_player_life = 3-bi_game_level;
    FbBackgroundColor(BLACK);
}

void bi_init(){
    bi_score = 0;
    bi_game_level = 0;
    bi_level_init();
    bi_state = BI_GET_INPUT;
}


void set_bi_exit()
{
    bi_state = BI_SHOW_MENU;
    returnToMenus();
}

void bi_get_input(){
    if (BUTTON_PRESSED_AND_CONSUME)
    {
        bi_state = BI_SHOW_MENU;
    }

    if(TOP_TAP_AND_CONSUME)
    {
        setNote(154, 2048);
        bi_player_action = BI_FIRE;
    }

    if (BOTTOM_TAP_AND_CONSUME)
    {
        setNote(154, 2048);
        bi_player_action = BI_FIRE;
    }

    if (LEFT_TAP_AND_CONSUME)
    {
        setNote(173, 2048);
        bi_player_action = BI_MOVE_LEFT;
    }

    if (RIGHT_TAP_AND_CONSUME)
    {
        setNote(173, 2048);
        bi_player_action = BI_MOVE_RIGHT;
    }
    
    if (bi_state == BI_GET_INPUT)
    {
        bi_state++;
    }
}

void bi_alien_hit_check(){
    if(biPlayer.missile_x < (biAlien.pos_x+10) &&
       biPlayer.missile_x > (biAlien.pos_x-1) && 
       biPlayer.missile_y < (biAlien.pos_y+5) &&
       biPlayer.missile_y > (biAlien.pos_y-1)){
        if(biAlien.alien[biPlayer.missile_y-biAlien.pos_y][(biPlayer.missile_x-biAlien.pos_x)/2]>0){
        biAlien.alien[biPlayer.missile_y-biAlien.pos_y][(biPlayer.missile_x-biAlien.pos_x)/2]--;
            biPlayer.missile_y = 0;
            biPlayer.missile_x = 0;
            setNote(103, 2048);
            
            bi_score += 20;
            setHighScore(SCORE_BADGE_INVADER, bi_score, 0);
        }
    }
    if(biPlayer.missile_y <= biAlien.pos_y){
        biPlayer.missile_x = 0;
        biPlayer.missile_y = 0;
    }
    
    if(biPlayer.missile_y == 0){
        biPlayer.missile_x = 0;
    }
}

void bi_player_hit_check(){
    if(biAlien.missile_x == biPlayer.pos_x && biAlien.missile_y == biPlayer.pos_y){
        bi_player_life--;
        setNote(121, 2048);
    }
}

void bi_process_move(){
    biPlayer.old_x = biPlayer.pos_x;
    biAlien.old_x = biAlien.pos_x;
    biAlien.old_y = biAlien.pos_y;
    bi_alien_hit_check();
    bi_player_hit_check();
    switch (bi_player_action){
        case BI_FIRE:
            if(biPlayer.missile_x == 0 && biPlayer.missile_y == 0){
                biPlayer.missile_x = biPlayer.pos_x;
                biPlayer.missile_y = biPlayer.pos_y-1;
            }
            break;
        case BI_MOVE_LEFT:
            if(biPlayer.pos_x > 0){
                biPlayer.pos_x--;
            }
            break;
        case BI_MOVE_RIGHT:
            if(biPlayer.pos_x < 25){
                biPlayer.pos_x++;
            }
            break;
    }
    if(biPlayer.missile_x != 0 && biPlayer.missile_y != 0){//advances player missile if there is one
        biPlayer.missile_y--;
    }
        
    if(biAlien.pos_y < 21){
        if(biAlien.pos_x <12 && biAlien.pos_x > 0){//moves aliens over in direction
            if(bi_flip==0){
                biAlien.pos_x++;
            }
            else{
                biAlien.pos_x--;
            }
        }
        else{//moves aliens down and changes direction
            biAlien.pos_y++;
            if(biAlien.pos_x<2){
                bi_flip=0;
                biAlien.pos_x++;
            }
            else{
                bi_flip=1;
                biAlien.pos_x--;
            }
        }
    }
    
    if(biAlien.missile_x != 0 && biAlien.missile_y != 0){//advances alien missile if there is one
        biAlien.missile_y++;
    }
    if(bi_game_counter==21){//creates alien missile
        biAlien.missile_x = biAlien.pos_x+3;
        biAlien.missile_y = biAlien.pos_y+5;
        bi_game_counter=0;
    }
    
    bi_player_action = BI_NONE;
    bi_game_counter++;
    bi_state++;  
}

void bi_check_win(){
    char i,j,k;
    k=0;
    for(i=0;i<5;i++){
        for(j=0;j<5;j++){
            if(biAlien.alien[i][j]>0){
                k++;
            }
        }
    }
    if(biAlien.pos_y > 20 || bi_player_life == 0){//checks for game over
        bi_delay_counter=0;
        bi_state = BI_GAME_OVER;
    }
    else if(k==0){
        if(bi_game_level == 2){
            bi_delay_counter=0;
            bi_state = BI_WIN;
        }
        else{
            bi_score += biAlien.pos_y;
            setHighScore(SCORE_BADGE_INVADER, bi_score, 0);
            bi_game_level++;
            bi_state = BI_LEVEL_INIT;
        }
    }
    else{
        bi_state++;//advances to rendering
    }
}

void bi_continue(){
    if(bi_game_level == 0 && bi_game_counter == 0)
    {
        bi_state = BI_INIT;
    }
    else
    {
        bi_state = BI_GET_INPUT;
    }
}

void bi_render(){

    char i = 0;
    char j = 0;
    for (i=0;i<5;i++) {
        for(j=0;j<5;j++){
            FbMove(biAlien.pos_x*bi_kmotion+(j*(bi_kmotion+1)*2), biAlien.pos_y*bi_kmotion+(i*(bi_kmotion+1)));
            switch(biAlien.alien[i][j]) {
                case 3:
                    FbColor(GREEN);
                    FbImage(BI_SPRITE,0);
                    break;
                case 2:
                    FbColor(RED);
                    FbImage(BI_SPRITE,1);
                    break; 
                case 1:
                    FbColor(YELLOW);
                    FbImage(BI_SPRITE,2);
                    break;
                case 0:
                    break;
            }
        } 
    }       
    
    FbColor(GREEN);
    
    FbMove(biAlien.missile_x*bi_kmotion, biAlien.missile_y*bi_kmotion);
    if(biAlien.missile_y!=0){
        FbCharacter(':');
    }
    
            
    FbMove(biPlayer.missile_x*bi_kmotion, biPlayer.missile_y*bi_kmotion);
    if(biPlayer.missile_y!=0){
        FbCharacter(':');
    }        
            
    FbMove(biPlayer.pos_x*bi_kmotion, biPlayer.pos_y*bi_kmotion);
    FbImage(BI_SPRITE, 3);

    FbColor(WHITE);
    if(bi_state == BI_RENDER){
        bi_state++;
    }

    FbSwapBuffers();
}

void bi_game_delay(){
    bi_get_input();
    if(bi_delay_counter == bi_delay){
        bi_delay_counter = 0;
        bi_state = BI_GET_INPUT;
    }
                     
    if (bi_delay_counter == missile_counter ||//modulous does not work because these don't ever evenly divide. Do not break by trying to use it.
        bi_delay_counter == (missile_counter*2) ||
        bi_delay_counter == (missile_counter*3) ||
        bi_delay_counter == (missile_counter*4) ||
        bi_delay_counter == (missile_counter*5) ||
        bi_delay_counter == (missile_counter*6) ||
        bi_delay_counter == (missile_counter*7)){
        if(biPlayer.missile_x != 0 && biPlayer.missile_y != 0){//advances player missile if there is one
            biPlayer.missile_y--;
        }
        bi_alien_hit_check();
        if(bi_render_divider == 1){
            bi_render_divider = 0;
            bi_render();
        }
        else{
            bi_render_divider++;
        }
    }
    
    bi_delay_counter++;
}

static char bi_score_buf[20];

void bi_print_score(){
    unsigned char i = 0;;
    char tmp[10] = {0};

    strcpy(bi_score_buf, "Score:");
    itoa(tmp, bi_score, 10);
    strcat(bi_score_buf, tmp);

    FbMove(10,40);
    FbWriteLine(bi_score_buf);
}

void bi_win(){
    if(bi_delay_counter<30){
        FbBackgroundColor(BLUE);
        FbMove(10,10);
        FbColor(WHITE);
        FbWriteLine("Unexpected Error:");
        FbMove(10,25);
        FbWriteLine("Player Has Won!");  
        bi_print_score();
    }
    else{
        bi_init();
        bi_state = BI_SHOW_MENU;
    } 
    bi_delay_counter++;
}

void bi_game_over(){
    if(bi_delay_counter<20){
        FbBackgroundColor(BLACK);
        FbMove(10,10);
        FbColor(RED);
        FbWriteLine("GAME OVER");
        FbMove(10,25);
        FbWriteLine("YOU LOSE!");
        bi_print_score();
    }
    else{
        bi_init();
        bi_state = BI_SHOW_MENU;
    }
    bi_delay_counter++;
}

void badgeinvaders_cb(){
    
    switch (bi_state) {
        
        case BI_INIT:
            bi_init();
            break;
        case BI_SHOW_MENU:
            genericMenu((struct menu_t *)badge_invaders_config_m, MAIN_MENU_STYLE);
            FbSwapBuffers();
            break;
        case BI_LEVEL_INIT:
            bi_level_init();
            bi_state = BI_GET_INPUT;
            break;
        case BI_GET_INPUT:
            bi_get_input();
            break;
        case BI_PROCESS_MOVE:
            bi_process_move();
            break;
        case BI_CHECK_WIN:
            bi_check_win();
            break;
        case BI_RENDER:
            bi_render();
            break;
        case BI_DELAY:
            bi_game_delay();
            break;
        case BI_WIN:
            bi_win();
            break;
        case BI_GAME_OVER:
            bi_game_over();
            break;
        case BI_EXIT:
            set_bi_exit();
            break;
        default:
            bi_state = BI_EXIT;
            break;
    }
}
