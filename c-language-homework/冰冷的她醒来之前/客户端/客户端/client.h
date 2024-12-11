
#ifndef CLIENT_H
#define CLIENT_H

#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN  // 定义该宏以精简 windows.h

#define menu_length 40
#define menu_height 20

#include <winsock2.h>        // 必须在 windows.h 之前包含
#include <windows.h>         // Windows API 头文件
#include <easyx.h>           // 图形库头文件
#include <stdio.h>           // 标准输入输出
#include <string.h>          // 字符串操作
#include <stdlib.h>          // 标准库函数

#pragma comment(lib, "ws2_32.lib")  // 链接 Winsock 库

// 枚举和结构体定义
enum card_type
{
    block,
    president,
    monitor,
    top_student,
    young_lady,
    news,
    book,
    discipline,
    health_care,
    home_goer,
    prisoner,
    accomplice,
    infected_person,
    alien
};

enum player_type
{
    default_player,
    player1,
    player2,
    player3,
    player4
};

enum direction
{
    up,
    down,
    left,
    right
};

#pragma pack(push, 1)
typedef struct card
{
    enum card_type type;
    int value;
    enum player_type player;
} card;
#pragma pack(pop)

// 全局变量声明
extern card player1_cards[4];
extern card player2_cards[4];
extern card player3_cards[4];
extern card player4_cards[4];
extern card player1_doubt_list[12];
extern card player2_doubt_list[12];
extern card player3_doubt_list[12];
extern card player4_doubt_list[12];
extern card preservation[16];
extern enum player_type turn;
extern int client_id;
extern int menu1_open;
extern int menu2_open;
extern ExMessage msg;
extern SOCKET socketfd;

extern IMAGE card_image[14];
extern IMAGE block_mini[4];
extern IMAGE preservation_image;
extern IMAGE block_minimum;
extern IMAGE block_minimum_plus;
extern IMAGE block_mini_dark[4];

extern CRITICAL_SECTION cs1;
extern CRITICAL_SECTION cs2;
extern CONDITION_VARIABLE cv1;
extern CONDITION_VARIABLE cv2;
extern int data_ready;
extern int data_updated;


void Initialize();
void Cleanup();
int main();

SOCKET InitNetwork();
int RecvClientID(SOCKET sock, int* client_id);
void SendGameState(SOCKET sock);
void RecvGameState(SOCKET sock);
DWORD WINAPI RecvThread(LPVOID lpParam);


void LoadCardImage();
void draw();
void card_draw();
void preservation_draw();
void doubt_list_draw();
void doubt_list_pre_draw(card* player_doubt_list1, card* player_doubt_list2, card* player_doubt_list3, card* player_doubt_list4);
void turn_draw();

void ProcessMessage();
IMAGE* get_image(enum card_type type, enum direction dirc);
void text_draw(int x, int y, const char* text);
void fulfill(int x, int y, int width, int length);
void menu_draw_1(int x, int y);
void menu_draw_2(int x, int y);


enum player_type next_turn(enum player_type turn);
void card_transfer(card* ori, card* des);
void transfer_to_doubt_list(int x, int y, int i);
void transfer_to_preservation(int i);
void game_over();


void changeWindowTitle(const char* title);


DWORD WINAPI DrawThread(LPVOID lpParam);

#endif // CLIENT_H
