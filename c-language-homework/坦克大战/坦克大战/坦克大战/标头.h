#include<easyx.h>
#include<stdio.h>
#include<graphics.h>
#include<time.h>
#include<conio.h>

IMAGE logo;
IMAGE block[4];
IMAGE player1_Img[4];
int current_map[26][26];

enum Direction;
enum Type;
enum Life;

typedef struct Tank;
typedef struct Bullet;

//地图组赋值
void set_map(int x, int y, int value);
//子弹碰撞检测
void bullet_collision(int x, int y, Bullet* bullet);
//1号玩家坦克行动
void player1_tank_move(Tank* tank);
//地图组赋值
void set_map(int x, int y, int value);
//子弹碰撞检测
void bullet_collision(int x, int y, Bullet* bullet);