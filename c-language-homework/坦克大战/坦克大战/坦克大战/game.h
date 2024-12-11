
#ifndef GAME_H
#define GAME_H

#include <easyx.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <MMSystem.h>

#pragma comment(lib, "winmm.lib")

#define widnows_width 832
#define windows_height 832
#define enermy_num 3

enum Direction {
    UP,
    Down,
    Left,
    Right
};

enum Type {
    Enermy,
    player,
};

enum Life {
    dead,
    live
};

enum Game_status {
    Processing,
    victory,
    defeat
};

enum Game_mode {
    one_player,
    two_players
};

// 坦克结构体
typedef struct Tank {
    int x;
    int y;
    Direction direction;
    int speed;
    Type type;
    Life life;
} Tank;

extern Tank player1;
extern Tank player2;
extern Tank enermy[enermy_num];

// 子弹结构体
typedef struct Bullet {
    int pos_x;
    int pos_y;
    Direction direction;
    int speed;
    Type type;
    Life life;
} Bullet;

extern Bullet player1_bullet;
extern Bullet player2_bullet;
extern Bullet enermy_bullet[enermy_num];

extern IMAGE logo;
extern IMAGE block[5];
extern IMAGE player1_Img[4];
extern IMAGE player2_Img[4];
extern IMAGE enermy_Img[4];
extern IMAGE bullet_Img[4];
extern IMAGE boom_Img[2];
extern int current_map[26][26];
extern Game_status flag;
extern Game_mode game_mode;

void set_map(int x, int y, int value);
void bullet_collision(int x, int y, Bullet* bullet);
void bullet_move(Bullet* bullet);
void init_player1();
void init_player2();
void init_enermy();
void load_Res();
void load_map(int map[][26]);
void start_menu();
void enermy_tank_move(Tank* tank, IMAGE* img, Direction direction, int i);
Direction enermy_tank_route(Tank* tank);
void player1_key_handler();
void player2_key_handler();
void init();
void draw();
void game_over();
DWORD WINAPI player1_move_thread(LPVOID lpParameter);
DWORD WINAPI player2_move_thread(LPVOID lpParameter);
DWORD WINAPI enermy_move_thread(LPVOID lpParameter);
DWORD WINAPI bullet_move_thread(LPVOID lpParameter);
DWORD WINAPI draw_thread(LPVOID lpParameter);
int create_threads(HANDLE hThread[], DWORD threadId[]);
void wait_for_threads(HANDLE hThread[]);
void close_threads(HANDLE hThread[]);

#endif // GAME_H
