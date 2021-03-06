#include "game2048.h"
#include "badge15.h"

extern badge_state b_state;//badge state structure to reduce variables

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

#define TILE_SIZE 29
#define GAME_2048_STATE b_state.counter1
#define GAME_2048_SELECTED b_state.counter2
#define GAME_2048_WAIT_COUNTER b_state.large_counter

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
   {"Resume", GREEN_BG, FUNCTION, (struct menu_t *)(resume_cb)},
   {"Restart", GREEN_BG, FUNCTION, (struct menu_t *)(restart_cb)},
   {"Quit", GREEN_BG, FUNCTION, (struct menu_t *)(quit_cb)},
   {"", GREEN_BG, BACK, NULL},
};

void game_2048_Run(){

    switch(GAME_2048_STATE)
    {
        case GAME_2048_INIT:
            b_state.state_drawn = 0;
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
            genericMenu(game_2048_menu);
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

void writeline(unsigned char * charin, unsigned char no_of_chars, unsigned char x, unsigned char y, unsigned short color)
{
    if(y<1)
        y=1;

    unsigned char j;
    if(no_of_chars > 22)
        no_of_chars = 22;

    DlColor(color);
    for(j=0;j<no_of_chars;j++)
    {   
        DlMove(x+(j*6), y);
        DlCharacter(charin[j]);
    }
}

void game_2048_welcome(){

    if(b_state.state_drawn == 0){
        unsigned char i = 0;
        DlInit();
	DlTransparentIndex(0);
        DlBackgroundColor(BLACK);
        DlClear();
        writeline("Merge tiles", 11, 33, 5 + (CHAR_HEIGHT * i++), GREEN);
        writeline("to reach 2048", 13, 21, 5 + (CHAR_HEIGHT * i++), GREEN);
               
        writeline("Use the sliders", 15, 21, 8 + (CHAR_HEIGHT * i++), GREEN);
        writeline("to move the", 11, 33, 8 + (CHAR_HEIGHT * i++), GREEN);
        writeline("tiles. When 2", 13, 27, 8 + (CHAR_HEIGHT * i++), GREEN);
        writeline("tiles with the", 14, 24, 8 + (CHAR_HEIGHT * i++), GREEN);
        writeline("same number", 11, 30, 8 + (CHAR_HEIGHT * i++), GREEN);
        writeline("touch they merge", 16, 18, 8 + (CHAR_HEIGHT * i++), GREEN);
        writeline("into one.", 9, 39, 8 + (CHAR_HEIGHT * i++), GREEN);
        
        writeline("Play Game", 9, 39, 8 + (CHAR_HEIGHT * i++) + 5, RED);
        b_state.state_drawn = 1;
	DlSwapBuffers();
    }

    if (BUTTON_PRESSED_AND_CONSUME)
    {
        GAME_2048_STATE++; 
        b_state.state_drawn = 0;
    }
}

void game_2048_play(){

    if (BUTTON_PRESSED_AND_CONSUME){
        GAME_2048_STATE = GAME_2048_MENU;
    }
    else if(TOP_SLIDE_AND_CONSUME){
        setNote(109, 2048);
        up();
    }
    else if (BOTTOM_SLIDE_AND_CONSUME){
        setNote(97, 2048);
        down();
    }
    else if (LEFT_SLIDE_AND_CONSUME){
        setNote(109, 2048);
        left();
    }
    else if (RIGHT_SLIDE_AND_CONSUME){
        setNote(97, 2048);
        right();
    }
}

void game_2048_game_won(){

    if(b_state.state_drawn == 0){

        printGrid();
        DlBackgroundColor(BLACK);
        DlMove(20,20);
        DlFilledRectangle(80, 50);
        writeline("YOU WIN!",      8,      33, 33, GREEN);
        writeline("Press button",   12,     30, 53, GREEN);
	DlSwapBuffers();

        b_state.state_drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME){
        game_2048_InitGrid();
        b_state.state_drawn = 0;
        GAME_2048_STATE = GAME_2048_DRAW_GRID;
    }
}

void game_2048_game_over(){

    if(b_state.state_drawn == 0){

        printGrid();
        DlBackgroundColor(BLACK);
        DlMove(20, 20);
        DlFilledRectangle(80, 50);
        writeline("GAME OVER",      9,      33, 33, GREEN);
        writeline("Press button",   12,     30, 53, GREEN);
	DlSwapBuffers();

        b_state.state_drawn = 1;
    }

    if (BUTTON_PRESSED_AND_CONSUME){
        game_2048_InitGrid();
        b_state.state_drawn = 0;
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
    return ((ReadCoreTimer() & 0xff) % (max-min)) + min;
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

    writeline(buf, i, 10, 3, BLACK);
}

void printGrid() {
    unsigned char i, j, x, y, val;

    DlInit();
    DlTransparentIndex(0);
    DlBackgroundColor(WHITE);
    DlClear();
    printScore();

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            x = (i*TILE_SIZE)+7;
            y = (j*TILE_SIZE)+12;
            val = grid[i][j];
            DlColor(color_2048_tile[val]);
            DlMove(x+1, y+1);
            DlFilledRectangle(TILE_SIZE-3, TILE_SIZE-2);
            DlColor(BLACK);
            DlMove(x, y);
            DlRectangle(TILE_SIZE-1, TILE_SIZE);
            buf[0] = vals[val][0];
            buf[1] = vals[val][1];
            buf[2] = vals[val][2];
            buf[3] = vals[val][3];
            //writeline(&buf, 4, x+3, y+13, BLACK);//(val <=2) ? BLACK : WHITE);
            writeline(buf, 4, x+3, y+13, BLACK);//(val <=2) ? BLACK : WHITE);
        }
    }
    DlSwapBuffers();
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
