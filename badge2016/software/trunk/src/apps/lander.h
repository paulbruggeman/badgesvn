#define LANDER_BODY_HEIGHT 15
#define LANDER_BODY_WIDTH 12
//#define LANDER_HEIGHT 20
#define LANDER_HEIGHT 22
//#define LANDER_WIDTH 15
#define LANDER_WIDTH 16

#define DRAG_UPDATE_RATE 4
#define GRAV_UPDATE_RATE 4


typedef struct object_state
{
    // TODO: don't think we need acc
    char acc_x, acc_y;

    char vel_x, vel_y;

    unsigned char pos_x, pos_y;
    unsigned char width, height;

    unsigned char is_active;
    //unsigned char state_flags;
    //unsigned char is_on_pad;

} object_state;


typedef struct lander_game_state_t{
    unsigned char h_slide;
    unsigned char v_slide;

    unsigned char drag_cnt;
    unsigned char grav_cnt;

    //lander_ship_state_t ship;
    //lander_pad_state_t pad;
    object_state ship, pad, alien, fuel_can;


    unsigned char lander_ship_fuel;

    // if negative, they lose!
    char ship_on_pad_cnt;

    // Index into the level array
    // May use negative vals for something...
    char lander_level_id;
} lander_game_state_t;

#define IS_LAST_LEVEL 1
typedef struct lander_level_init_callback_t {
        void (*init)(lander_game_state_t *g_state);
        void (*update)(lander_game_state_t *g_state);
        char flags;
} lander_level_init_callback_t;

void lander_level_1_init(lander_game_state_t *g_state);
void lander_level_2_init(lander_game_state_t *g_state);
void lander_level_3_init(lander_game_state_t *g_state);
void lander_level_4_init(lander_game_state_t *g_state);
void lander_level_5_init(lander_game_state_t *g_state);

void lander_update_pad(lander_game_state_t *g_state);

void lander_bound(unsigned char *x_pos, unsigned char *y_pos,
                  unsigned char *x_vel, unsigned char *y_vel);

unsigned char lander_is_overlap(unsigned char p1, unsigned char o1,
                                unsigned char p2, unsigned char o2);

unsigned char lander_is_colliding(unsigned char x1,unsigned char y1,
                                    unsigned char w1,unsigned char h1,
                                  unsigned char x2, unsigned char y2,
                                    unsigned char w2,unsigned char h2);

unsigned char lander_obj_is_colliding(object_state o1,
                                      object_state o2);


void badge_lander_cb();
