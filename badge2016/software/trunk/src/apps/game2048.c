#include "game2048.h"
#include "badge16.h"
#include "menu.h"
#include "fb.h"
#include "touchCTMU.h"
#include "colors.h"
#include "../prngs.h"


enum GAME_2048_STATES
{
    GAME_2048_INIT,
    GAME_2048_INIT_GRID,
    GAME_2048_WELCOME,
    GAME_2048_DRAW_GRID,
    GAME_2048_DRAW_SCORE,
    GAME_2048_PLAY,
    GAME_2048_COLLAPSE,
    GAME_2048_SPAWN,
    GAME_2048_DELAY,
    GAME_2048_MENU,
    GAME_2048_GAME_WON,
    GAME_2048_GAME_OVER,
    GAME_2048_EXIT
};
enum GAME_2048_STATES GAME_2048_STATE;

#define TILE_SIZE 29
//#define GAME_2048_STATE b_state.counter1

int GAME_2048_SELECTED;
int drawn;
long GAME_2048_WAIT_COUNTER;

#define CHAR_HEIGHT 10

bool moveUp();
bool moveDown();
bool moveLeft();
bool moveRight();
bool hasWon();
bool canMove();

static unsigned char grid[4][4];
static unsigned int score;

const char vals[12][4] = {{' ', ' ', ' ', ' '}, {' ', ' ', '2', ' '},
                    {' ' ,' ', '4', ' '}, {' ' ,' ', '8', ' '},
                    {' ' ,'1', '6', ' '}, {' ' ,'3', '2', ' '},
                    {' ' ,'6', '4', ' '}, {' ' ,'1', '2', '8'},
                    {' ' ,'2', '5', '6'}, {' ' ,'5', '1', '2'},
                    {'1' ,'0', '2', '4'}, {'2' ,'0', '4', '8'}};

const unsigned short color_2048_tile[12] = {
    0b0111101111101111,
    0b1110011011111000,
    0b1110011000110011,
    0b1110110101101110,
    0b1110110010001100,
    0b1110101111001011,
    0b1110101011100111,
    0b1110011001101101,
    0b1110011001001011,
    0b1110011000101001,
    0b1110011000000111,
    0b1110010111100101
};

void resume_cb(){
    GAME_2048_STATE = GAME_2048_DRAW_GRID;
}

void restart_cb(){
    game_2048_InitGrid();
    GAME_2048_STATE = GAME_2048_DRAW_GRID;
}

void quit_cb(){
    game_2048_onExit();
}

const struct menu_t game_2048_menu[] = {
   {"Resume", VERT_ITEM|DEFAULT_ITEM, FUNCTION, (struct menu_t *)(resume_cb)},
   {"Restart", VERT_ITEM, FUNCTION, (struct menu_t *)(restart_cb)},
   {"Quit", VERT_ITEM|LAST_ITEM, FUNCTION, (struct menu_t *)(quit_cb)},
};

void game_2048_Run(){

    switch(GAME_2048_STATE)
    {
        case GAME_2048_INIT:
            drawn = 0;
            GAME_2048_WAIT_COUNTER = 0;
            GAME_2048_STATE++;
            break;

        case GAME_2048_INIT_GRID:
            game_2048_InitGrid();
            break;

        case GAME_2048_WELCOME:
            game_2048_welcome();
            break;        

        case GAME_2048_DRAW_GRID:
            printGrid();
            break;

        case GAME_2048_DRAW_SCORE:
            printScore();
            break;

        case GAME_2048_PLAY:
            game_2048_play();
            break;

        case GAME_2048_COLLAPSE:
            printGrid();
            GAME_2048_STATE = GAME_2048_SPAWN;
            break;

        case GAME_2048_SPAWN:
            placeTile(0);
            break;

        case GAME_2048_DELAY:
            GAME_2048_WAIT_COUNTER++;
            /* pause before rendering new tile */
            if (GAME_2048_WAIT_COUNTER == 250)
            { 
                GAME_2048_WAIT_COUNTER = 0;
                GAME_2048_STATE = GAME_2048_DRAW_GRID;
            }
            break;

        case GAME_2048_GAME_OVER:
            game_2048_game_over();
            break;

        case GAME_2048_GAME_WON:
            game_2048_game_won();
            break;

        case GAME_2048_MENU:
            returnToMenus();
            GAME_2048_STATE = GAME_2048_INIT;
            //genericMenu(game_2048_menu, MAIN_MENU_STYLE);
            break;

        case GAME_2048_EXIT:
            GAME_2048_STATE = GAME_2048_INIT;
            returnToMenus();
            break;

        default:
            break;
    }
}

void game_2048_InitGrid(){

    unsigned short i;
    unsigned short j;

    score = 0;

    //Init Grid to be empty
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            grid[i][j] = 0;

    placeTile(1);
    placeTile(1);
    
    GAME_2048_STATE = GAME_2048_WELCOME;
}

void game_2048_welcome(){

    if(drawn == 0){
        unsigned char i = 0;
        FbTransparentIndex(0);
        FbBackgroundColor(BLACK);
        FbClear();
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbColor(GREEN);
        FbWriteLine("Merge tiles");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("to reach 2048");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("Use the sliders");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("to move the");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("tiles. When 2");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("tiles with the");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("same number");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("touch they merge");
        
        FbMove(33,5 + (CHAR_HEIGHT * i++));
        FbWriteLine("into one.");
        
        FbColor(BLUE);
        FbMove(33,8 + (CHAR_HEIGHT * i++) + 5);
        FbWriteLine("Play Game");
        
        drawn = 1;
        FbSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME)
    {
        GAME_2048_STATE++; 
        drawn = 0;
    }
}

void game_2048_play(){

    if (BUTTON_PRESSED_AND_CONSUME){
        GAME_2048_STATE = GAME_2048_MENU;
    }
    else if(TOP_SLIDE_AND_CONSUME) {
        setNote(97, 2048);
        down();
    }
    else if (BOTTOM_SLIDE_AND_CONSUME) {
        setNote(109, 2048);
        up();
    }
    else if (LEFT_SLIDE_AND_CONSUME){
        setNote(97, 2048);
        right();
    }
    else if (RIGHT_SLIDE_AND_CONSUME){
        setNote(109, 2048);
        left();
    }
}

void game_2048_game_won(){

    if(drawn == 0){

        printGrid();
        FbBackgroundColor(BLACK);
        FbMove(20,20);
        FbFilledRectangle(80, 50);
        FbMove(33,33);
        FbColor(GREEN);
        FbWriteLine("YOU WIN!");
        FbMove(30,53);
        FbWriteLine("Press button");
        FbSwapBuffers();

        drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME){
        game_2048_InitGrid();
        drawn = 0;
        GAME_2048_STATE = GAME_2048_DRAW_GRID;
    }
}

void game_2048_game_over(){

    if(drawn == 0){

        printGrid();
        FbBackgroundColor(BLACK);
        FbMove(20, 20);
        FbFilledRectangle(80, 50);
        FbColor(GREEN);
        FbMove(33,33);
        FbWriteLine("GAME OVER");
        FbMove(30,53);
        FbWriteLine("Press button");
        
        FbSwapBuffers();

        drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME){
        game_2048_InitGrid();
        drawn = 0;
    }
}

void game_2048_onExit(){
    GAME_2048_STATE = GAME_2048_EXIT;
}

void up() {
    if (moveUp())
        GAME_2048_STATE = GAME_2048_COLLAPSE;
}

void down() {
    if (moveDown())
        GAME_2048_STATE = GAME_2048_COLLAPSE;
}

void left() {
    if (moveLeft())
        GAME_2048_STATE = GAME_2048_COLLAPSE;
}

void right() {
    if (moveRight())
        GAME_2048_STATE = GAME_2048_COLLAPSE;
}

unsigned char random(unsigned char min, unsigned char max){
    return get_rand_char(min,max);
}

void placeTile(unsigned char val) {
    if (val==0)
    {
        val = random(0, 9);
        if (val >= 2)
            val = 1;
        else
            val = 2;
    }

    unsigned char x;
    unsigned char k = 0;
    unsigned char i;
    unsigned char j;
    
    x = random(0, 15);

    while(k <= x)
    {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (grid[i][j] == 0)
                {
                    if (x == k)
                    {
                        grid[i][j] = val;
                        k = x + 1;
                    }
                    else
                    {
                        k++;
                    }
                }
            }
        }
    }

    //Check for winning
    if (hasWon())
        GAME_2048_STATE = GAME_2048_GAME_WON;
    else if (!canMove())
        GAME_2048_STATE = GAME_2048_GAME_OVER;
    else
        GAME_2048_STATE = GAME_2048_DELAY;
}

static char buf[20];

void printScore(){
    unsigned char i = 0;;
    char tmp[10] = {0};

    strcpy(buf, "Score:");
    itoa(tmp, score, 10);
    strcat(buf, tmp);

    while(buf[i] != '\0')
        i++;
    
    FbColor(BLACK);
    FbMove(10,3);
    FbWriteLine(buf);
}

void printGrid() {
    unsigned char i, j, x, y, val;

    FbTransparentIndex(0);
    FbBackgroundColor(WHITE);
    FbClear();
    printScore();

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            x = (i*TILE_SIZE)+7;
            y = (j*TILE_SIZE)+12;
            val = grid[i][j];
            FbColor(color_2048_tile[val]);
            FbMove(x+1, y+1);
            FbFilledRectangle(TILE_SIZE-3, TILE_SIZE-2);
            FbColor(BLACK);
            FbMove(x, y);
            FbRectangle(TILE_SIZE-1, TILE_SIZE);
            buf[0] = vals[val][0];
            buf[1] = vals[val][1];
            buf[2] = vals[val][2];
            buf[3] = vals[val][3];
            //writeline(&buf, 4, x+3, y+13, BLACK);//(val <=2) ? BLACK : WHITE);
            FbMove(x+3, y+13);
            FbColor(BLACK);
            FbWriteLine(buf);
        }
    }
    FbSwapBuffers();
    GAME_2048_STATE = GAME_2048_PLAY;
}

bool moveUp() {
    bool moved = false;

    unsigned short i;
    unsigned short j;
    unsigned short k;

    //move
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(grid[i][j] == 0){
                for(k=j+1;k<4;k++){
                    if(grid[i][k] != 0){
                        grid[i][j]=grid[i][k];
                        grid[i][k]=0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }

    //combine
    for(i=0;i<4;i++){
        for (j = 0; j < 3; j++) {
            if (grid[i][j] != 0 && grid[i][j] == grid[i][j + 1]) {
                grid[i][j]++;
                grid[i][j + 1] = 0;
                score += ((grid[i][j])-1) * (1 << grid[i][j]);
                moved = true;
            }
        }
    }

    //move again
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (grid[i][j] == 0) {
                for (k = j + 1; k < 4; k++) {
                    if (grid[i][k] != 0) {
                        grid[i][j] = grid[i][k];
                        grid[i][k] = 0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

bool moveDown() {
    bool moved = false;

    char i;
    char j;
    char k;

    //move
    for (i = 0; i < 4; i++){
        for (j = 3; j >= 0; j--) {
            if (!grid[i][j] != 0) {
                for (k = j - 1; k >= 0; k--)
                    if (grid[i][k] != 0) {
                        grid[i][j] = grid[i][k];
                        grid[i][k] = 0;
                        moved = true;
                        break;
                    }
            }
        }
    }

    //combine
    for(i=0;i<4;i++){
        for (j = 3; j > 0; j--) {
            if (grid[i][j] != 0 && grid[i][j] == grid[i][j - 1]) {
                grid[i][j]++;
                grid[i][j - 1] = 0;
                score += ((grid[i][j])-1) * (1 << grid[i][j]);
                moved = true;
            }
        }
    }

    //move again
    for (i = 0; i < 4; i++) {
        for (j = 3; j >= 0; j--) {
            if (!grid[i][j] != 0) {
                for (k = j - 1; k >= 0; k--)
                {
                    if (grid[i][k] != 0) {
                        grid[i][j] = grid[i][k];
                        grid[i][k] = 0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

bool moveLeft() {
    bool moved = false;

    unsigned short i;
    unsigned short j;
    unsigned short k;

    //move
    for(i=0;i<4;i++){
        for (j = 0; j < 4; j++) {
            if (!grid[j][i] != 0) {
                for (k = j + 1; k < 4; k++) {
                    if (grid[k][i] != 0) {
                        grid[j][i] = grid[k][i];
                        grid[k][i] = 0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }
    //combine
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            if (grid[j][i] != 0 && grid[j][i] == grid[j + 1][i]) {
                grid[j][i]++;
                grid[j + 1][i] = 0;
                score += ((grid[j][i])-1) * (1 << grid[j][i]);
                moved = true;

            }
        }
    }

    //move again
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (!grid[j][i] != 0) {
                for (k = j + 1; k < 4; k++) {
                    if (grid[k][i] != 0) {
                        grid[j][i] = grid[k][i];
                        grid[k][i] = 0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }

    return moved;
}

bool moveRight() {
    bool moved = false;

    char i;
    char j;
    char k;

    //move
    for(i=0;i<4;i++){
        for (j = 3; j >= 0; j--) { 
            if (!grid[j][i] != 0) {
                for (k = j - 1; k >= 0; k--) { 
                    if (grid[k][i] != 0) {
                        grid[j][i] = grid[k][i];
                        grid[k][i] = 0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }
    
    //combine
    for(i=0;i<4;i++){
        for (j = 3; j > 0; j--) {
            if (grid[j][i] && grid[j][i] == grid[j - 1][i]) {
                grid[j][i]++;
                grid[j - 1][i] = 0;
                score += ((grid[j][i])-1) * (1 << grid[j][i]);
                moved = true;
            }
        }
    }

    //move again
    for (i = 0; i < 4; i++) {
        for (j = 3; j >= 0; j--) {
            if (!grid[j][i] != 0) {
                for (k = j - 1; k >= 0; k--) {
                    if (grid[k][i] != 0) {
                        grid[j][i] = grid[k][i];
                        grid[k][i] = 0;
                        moved = true;
                        break;
                    }
                }
            }
        }
    }
    
    return moved;
}

bool hasWon(){
    unsigned char i, j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            if(grid[i][j] == 11){
                return true;
            }
        }
    }
    return false;
}

bool canMove(){
    
    unsigned short x;
    unsigned short y;

    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++){
            if (grid[x][y] == 0)
                return true;
            else if (y < 3 && grid[x][y] == grid[x][y + 1])
                return true;
            else if (x < 3 && grid[x][y] == grid[x + 1][y])
                return true;
        }
    }

    return false;
}
