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

//��ͼ�鸳ֵ
void set_map(int x, int y, int value);
//�ӵ���ײ���
void bullet_collision(int x, int y, Bullet* bullet);
//1�����̹���ж�
void player1_tank_move(Tank* tank);
//��ͼ�鸳ֵ
void set_map(int x, int y, int value);
//�ӵ���ײ���
void bullet_collision(int x, int y, Bullet* bullet);