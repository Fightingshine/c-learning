#include<easyx.h>
#include<stdio.h>
#include<graphics.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<Windows.h>
#include<MMSystem.h>
#include<thread>
#pragma comment(lib,"winmm.lib")

#define widnows_width 832
#define windows_height 832
#define enermy_num 3


enum Direction
{
	UP,
	Down,
	Left,
	Right
};
enum Type
{
	Enermy,
	player,
};
enum Life
{
	dead,
	live
};
enum Game_status
{
	Processing,
	victory,
	defeat
};
enum Game_mode
{
	one_player,
	two_players
};

//坦克结构体
typedef struct Tank {
	int x;
	int y;
	Direction direction;
	int speed;
	Type type;
	Life life;
}Tank;
Tank player1;
Tank player2;
Tank enermy[enermy_num];


//子弹结构体
typedef struct Bullet {
	int pos_x;
	int pos_y;
	Direction direction;
	int speed;
	Type type;
	Life life;
}Bullet;
Bullet player1_bullet;
Bullet player2_bullet;
Bullet enermy_bullet[enermy_num];


IMAGE logo;
IMAGE block[5];
IMAGE player1_Img[4];
IMAGE player2_Img[4];
IMAGE enermy_Img[4];
IMAGE bullet_Img[4];
IMAGE boom_Img[2];
int current_map[26][26];
Game_status flag = Processing;
Game_mode game_mode;

//地图组赋值
void set_map(int x, int y, int value) {
	current_map[y][x] = value;
	current_map[y][x + 1] = value;
	current_map[y + 1][x] = value;
	current_map[y + 1][x + 1] = value;
}
//播放音乐
void play_music(const char* music, int volume, int repeat = false) {
	mciSendString("close all", NULL, 0, NULL);
	char str[100];
	sprintf(str, "open %s alias music", music);
	mciSendString(str, NULL, 0, NULL);
	sprintf(str, "play music repeat");
	mciSendString(str, NULL, 0, NULL);
	sprintf(str, "setaudio music volume to %d", volume);
	mciSendString(str, NULL, 0, NULL);
}
//子弹碰撞检测
void bullet_collision(int x, int y, Bullet* bullet) {
	if (y >= 0 && y < 26 && x >= 0 && x < 26) {
		if (current_map[y][x] == 1) {
			current_map[y][x] = 0;
			bullet->life = dead;
			putimage(x * 32, y * 32, &boom_Img[0]);
            PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
            waveOutSetVolume(0, 0x50005000); // 设置音量
		}
		else if (current_map[y][x] == 2) {
			PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			waveOutSetVolume(0, 0x50005000); // 设置音量
			bullet->life = dead;
		}
		else if (current_map[y][x] == 300 ) {
			player2.life = dead;
			set_map(player2.x, player2.y, 0);
			putimage(player2.x * 32, player2.y * 32, &boom_Img[1]);
			PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			waveOutSetVolume(0, 0x50005000); // 设置音量
			bullet->life = dead;
		}
		else if (current_map[y][x] == 200  ) {
			player1.life = dead;
			set_map(player1.x, player1.y, 0);
			putimage(player1.x * 32, player1.y * 32, &boom_Img[1]);
			PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			waveOutSetVolume(0, 0x50005000); // 设置音量
			bullet->life = dead;

		}
		else if (current_map[y][x] == 100 && bullet->type != Enermy) {
			
			for (int i = 0; i < enermy_num; i++)
			{
				if (enermy[i].x == x && enermy[i].y == y || enermy[i].x == x && enermy[i].y+1 == y || enermy[i].x == x+1 && enermy[i].y == y || enermy[i].x+1 == x && enermy[i].y+1 == y)
				{
					putimage(enermy[i].x * 32, enermy[i].y * 32, &boom_Img[1]);
					PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
					waveOutSetVolume(0, 0x50005000); // 设置音量
					enermy[i].life = dead;
					set_map(enermy[i].x, enermy[i].y, 0);
				}
			}
			bullet->life = dead;
		}
		else if (current_map[y][x] == 100 && bullet->type == Enermy) {
			PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			bullet->life = dead;
		}
		else if (current_map[y][x] == 4) {
			putimage(x * 32, y * 32, &boom_Img[1]);
			PlaySound(_T("./Res/audio/boom.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
			waveOutSetVolume(0, 0x50005000); // 设置音量
			flag = defeat;
			bullet->life = dead;
		}
	}
}
//子弹移动
void bullet_move(Bullet* bullet) {
	int map_x, map_y, map_x1, map_y1;
	map_y = bullet->pos_y / 32;
	map_x = bullet->pos_x / 32;

	if (bullet->life == live) {
		if (bullet->direction == UP) {
			setfillcolor(BLACK);
			solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 16, bullet->pos_y + 32);
			bullet->pos_y -= 32;
			putimage(bullet->pos_x, bullet->pos_y, &bullet_Img[bullet->direction]);
			if (bullet->pos_y < 0) {
				bullet->life = dead;
			}
			else {
				map_y = bullet->pos_y / 32;
				map_x = bullet->pos_x / 32;
				map_x1 = (bullet->pos_x + 16) / 32;
				map_y1 = bullet->pos_y / 32;
				bullet_collision(map_x, map_y - 1, bullet);
				bullet_collision(map_x1, map_y1 - 1, bullet);

			}
		}
		else if (bullet->direction == Down) {
			setfillcolor(BLACK);
			solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 16, bullet->pos_y + 32);
			bullet->pos_y += 32;
			putimage(bullet->pos_x, bullet->pos_y, &bullet_Img[bullet->direction]);
			if (bullet->pos_y > 832) {
				bullet->life = dead;
			}
			else {
				map_y = bullet->pos_y / 32;
				map_x = bullet->pos_x / 32;
				map_x1 = (bullet->pos_x + 16) / 32;
				map_y1 = (bullet->pos_y + 32) / 32;
				bullet_collision(map_x, map_y + 1, bullet);
				bullet_collision(map_x1, map_y1, bullet);
			}
		}
		else if (bullet->direction == Left) {
			setfillcolor(BLACK);
			solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 32, bullet->pos_y + 16);
			bullet->pos_x -= 32;
			putimage(bullet->pos_x, bullet->pos_y, &bullet_Img[bullet->direction]);
			if (bullet->pos_x < 0) {
				bullet->life = dead;
			}
			else {
				map_y = bullet->pos_y / 32;
				map_x = bullet->pos_x / 32;
				map_x1 = bullet->pos_x / 32;
				map_y1 = (bullet->pos_y + 16) / 32;
				bullet_collision(map_x - 1, map_y, bullet);
				bullet_collision(map_x1 - 1, map_y1, bullet);
			}
		}
		else if (bullet->direction == Right) {
			setfillcolor(BLACK);
			solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 32, bullet->pos_y + 16);
			bullet->pos_x += 32;
			putimage(bullet->pos_x, bullet->pos_y, &bullet_Img[bullet->direction]);
			if (bullet->pos_x > 832) {
				bullet->life = dead;
			}
			else {
				map_y = bullet->pos_y / 32;
				map_x = bullet->pos_x / 32;
				map_x1 = (bullet->pos_x + 32) / 32;
				map_y1 = (bullet->pos_y + 16) / 32;
				bullet_collision(map_x + 1, map_y, bullet);
				bullet_collision(map_x1, map_y1, bullet);
			}
		}
		Sleep(25);
	}
}


//初始化玩家1
void init_player1() {
	player1.x = 8;
	player1.y = 24;
	player1.direction = UP;
	player1.life = live;
	set_map(player1.x, player1.y, 200);
	putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
	player1_bullet.type = player;
	player1_bullet.life = dead;
}
//初始化玩家2
void init_player2() {
	player2.x = 8+8;
	player2.y = 24;
	player2.direction = UP;
	player2.life = live;
	set_map(player2.x, player2.y, 300);
	putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
	player2_bullet.type = player;
	player2_bullet.life = dead;
}

//初始化敌人
void init_enermy() {
	for (int i = 0; i < enermy_num; i++)
	{
		enermy[i].x = i * 10;
		enermy[i].y = 0;
		enermy[i].direction = Down;
		enermy[i].life = live;
		set_map(enermy[i].x, enermy[i].y, 100);
		putimage(enermy[i].x * 32, enermy[i].y * 32, &enermy_Img[enermy[i].direction]);
		enermy_bullet[i].life = dead;
		enermy_bullet[i].type = Enermy;
	}
}
//加载图片
void load_Res() {
	loadimage(&block[0], _T("./Res/blk.png"));
	loadimage(&block[1], _T("./Res/brick.jpg"));
	loadimage(&block[2], _T("./Res/iron.jpg"));
	loadimage(&block[3], _T("./Res/headquarter/01.jpg"));
	loadimage(&block[4], _T("./Res/headquarter/02.jpg"));
	loadimage(&player1_Img[UP], _T("./Res/player/tank_up.jpg"));
	loadimage(&player1_Img[Down], _T("./Res/player/tank_down.jpg"));
	loadimage(&player1_Img[Left], _T("./Res/player/tank_left.jpg"));
	loadimage(&player1_Img[Right], _T("./Res/player/tank_right.jpg"));
	loadimage(&player2_Img[UP], _T("./Res/player/tank0.jpg"));
	loadimage(&player2_Img[Down], _T("./Res/player/tank1.jpg"));
	loadimage(&player2_Img[Left], _T("./Res/player/tank2.jpg"));
	loadimage(&player2_Img[Right], _T("./Res/player/tank3.jpg"));
	loadimage(&bullet_Img[UP], _T("./Res/bullet/bullet_up.png"));
	loadimage(&bullet_Img[Down], _T("./Res/bullet/bullet_down.png"));
	loadimage(&bullet_Img[Left], _T("./Res/bullet/bullet_left.png"));
	loadimage(&bullet_Img[Right], _T("./Res/bullet/bullet_right.png"));
	loadimage(&boom_Img[0], _T("./Res/boom/boom_bullet.png"));
	loadimage(&boom_Img[1], _T("./Res/boom/boom_tank.jpg"));
	loadimage(&enermy_Img[UP], _T("./Res/enermy/enermy_up.jpg"));
	loadimage(&enermy_Img[Down], _T("./Res/enermy/enermy_down.jpg"));
	loadimage(&enermy_Img[Left], _T("./Res/enermy/enermy_left.jpg"));
	loadimage(&enermy_Img[Right], _T("./Res/enermy/enermy_right.jpg"));
	}
//预设地图
int map1[26][26] =
{
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
{ 1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1 },
{ 2,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2,2 },
{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,0,0,0,0,0 }
};
//载入地图
void load_map(int map[][26]) {
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			current_map[i][j] = map[i][j];
		}
	}
}
//初始菜单绘制	
void start_menu() {
	mciSendString("open ./Res/audio/saints.mp3 alias start_menu", NULL, 0, NULL);
	mciSendString("play start_menu repeat", NULL, 0, NULL);
	mciSendString("setaudio start_menu volume to 200", NULL, 0, NULL);
	loadimage(&logo, _T("./Res/logo.jpg"));
	putimage(25, 100, &logo);
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(316, 456, 316 + 220, 456 + 50);
	settextcolor(WHITE);
	settextstyle(50, 25, "楷体", 0, 0, 1000, true, false, false);
	setbkmode(TRANSPARENT);
	outtextxy(316, 456, "1 PLAYER ");
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(316, 456+ 100, 316 + 220, 456+ 100 + 50);
	settextcolor(WHITE);
	settextstyle(50, 25, "楷体", 0, 0, 1000, true, false, false);
	setbkmode(TRANSPARENT);
	outtextxy(316, 456+ 100, "2 PLAYER ");
	ExMessage msg;
	while (true)
	{
		peekmessage(&msg, EX_MOUSE);
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= 316 && msg.x <= 316 + 220 && msg.y >= 456 && msg.y <= 456 + 50)
			{
				game_mode = one_player;
				mciSendString("close start_menu", NULL, 0, NULL);
				cleardevice();
				break;
			}
			else if (msg.x >= 316 && msg.x <= 316 + 220 && msg.y >= 456+ 100 && msg.y <= 456+ 100 + 50)
			{
				game_mode = two_players;
				mciSendString("close start_menu", NULL, 0, NULL);
				cleardevice();
				break;
			}
		}
	}
}

//敌方坦克行动
void enermy_tank_move(Tank* tank,IMAGE*img,Direction direction,int i) {
	switch (direction)
	{
	case UP:
		if (tank->direction == UP && tank->y - 1 >= 0 && current_map[tank->y - 1][tank->x] == 0 && current_map[tank->y - 1][tank->x + 1] == 0) {
			set_map(tank->x, tank->y, 0);
			setfillcolor(BLACK);
			solidrectangle(tank->x * 32, tank->y * 32, tank->x * 32 + 64, tank->y * 32 + 64);
			tank->y--;
			set_map(tank->x, tank->y, 100);
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		else if (tank->direction != UP) {
			tank->direction = UP;
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		break;
	case Down:
		if (tank->direction == Down && tank->y + 2 < 26 && current_map[tank->y + 2][tank->x] == 0 && current_map[tank->y + 2][tank->x + 1] == 0) {
			set_map(tank->x, tank->y, 0);
			setfillcolor(BLACK);
			solidrectangle(tank->x * 32, tank->y * 32, tank->x * 32 + 64, tank->y * 32 + 64);
			tank->y++;
			set_map(tank->x, tank->y, 100);
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		else if (tank->direction != Down) {
			tank->direction = Down;
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		break;
	case Left:
		if (tank->direction == Left && tank->x - 1 >= 0 && current_map[tank->y][tank->x - 1] == 0 && current_map[tank->y + 1][tank->x - 1] == 0) {
			set_map(tank->x, tank->y, 0);
			setfillcolor(BLACK);
			solidrectangle(tank->x * 32, tank->y * 32, tank->x * 32 + 64, tank->y * 32 + 64);
			tank->x--;
			set_map(tank->x, tank->y, 100);
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		else if (tank->direction != Left) {
			tank->direction = Left;
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		break;
	case Right:
		if (tank->direction == Right && tank->x + 2 < 26 && current_map[tank->y][tank->x + 2] == 0 && current_map[tank->y + 1][tank->x + 2] == 0) {
			set_map(tank->x, tank->y, 0);
			setfillcolor(BLACK);
			solidrectangle(tank->x * 32, tank->y * 32, tank->x * 32 + 64, tank->y * 32 + 64);
			tank->x++;
			set_map(tank->x, tank->y, 100);
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		else if (tank->direction != Right) {
			tank->direction = Right;
			putimage(tank->x * 32, tank->y * 32, &img[tank->direction]);
		}
		break;
	}
	//敌方发射子弹
	if (enermy_bullet[i].life == dead ) {
		if (enermy[i].direction == UP)
		{
			if (current_map[enermy[i].y - 1][enermy[i].x] != 0 || current_map[enermy[i].y - 1][enermy[i].x + 1] != 0) {
				bullet_collision(enermy[i].x, enermy[i].y - 1, &enermy_bullet[i]);
				bullet_collision(enermy[i].x + 1, enermy[i].y - 1, &enermy_bullet[i]);
			}
			else if (current_map[enermy[i].y - 2][enermy[i].x] != 0 || current_map[enermy[i].y - 2][enermy[i].x + 1] != 0)
			{
				bullet_collision(enermy[i].x, enermy[i].y - 2, &enermy_bullet[i]);
				bullet_collision(enermy[i].x + 1, enermy[i].y - 2, &enermy_bullet[i]);
			}
			else {
				enermy_bullet[i].pos_x = enermy[i].x * 32 + 24;
				enermy_bullet[i].pos_y = (enermy[i].y - 1) * 32;
				enermy_bullet[i].direction = enermy[i].direction;
				enermy_bullet[i].life = live;
			}

		}
		else if (enermy[i].direction == Down)
		{
			if (current_map[enermy[i].y + 2][enermy[i].x] != 0 || current_map[enermy[i].y + 2][enermy[i].x + 1] != 0) {
				bullet_collision(enermy[i].x, enermy[i].y + 2, &enermy_bullet[i]);
				bullet_collision(enermy[i].x + 1, enermy[i].y + 2, &enermy_bullet[i]);

			}
			else if (current_map[enermy[i].y + 3][enermy[i].x] != 0 || current_map[enermy[i].y + 3][enermy[i].x + 1] != 0)
			{
				bullet_collision(enermy[i].x, enermy[i].y + 3, &enermy_bullet[i]);
				bullet_collision(enermy[i].x + 1, enermy[i].y + 3, &enermy_bullet[i]);

			}
			else {
				enermy_bullet[i].pos_x = enermy[i].x * 32 + 24;
				enermy_bullet[i].pos_y = (enermy[i].y + 2) * 32;
				enermy_bullet[i].direction = enermy[i].direction;
				enermy_bullet[i].life = live;
			}
		}
		else if (enermy[i].direction == Left)
		{
			if (current_map[enermy[i].y][enermy[i].x - 1] != 0 || current_map[enermy[i].y + 1][enermy[i].x - 1] != 0) {
				bullet_collision(enermy[i].x - 1, enermy[i].y, &enermy_bullet[i]);
				bullet_collision(enermy[i].x - 1, enermy[i].y + 1, &enermy_bullet[i]);

			}
			else if (current_map[enermy[i].y][enermy[i].x - 2] != 0 || current_map[enermy[i].y + 1][enermy[i].x - 2] != 0)
			{
				bullet_collision(enermy[i].x - 2, enermy[i].y, &enermy_bullet[i]);
				bullet_collision(enermy[i].x - 2, enermy[i].y + 1, &enermy_bullet[i]);

			}
			else {
				enermy_bullet[i].pos_x = (enermy[i].x - 1) * 32;
				enermy_bullet[i].pos_y = enermy[i].y * 32 + 24;
				enermy_bullet[i].direction = enermy[i].direction;
				enermy_bullet[i].life = live;
			}
		}
		else if (enermy[i].direction == Right)
		{
			if (current_map[enermy[i].y][enermy[i].x + 2] != 0 || current_map[enermy[i].y + 1][enermy[i].x + 2] != 0) {
				bullet_collision(enermy[i].x + 2, enermy[i].y, &enermy_bullet[i]);
				bullet_collision(enermy[i].x + 2, enermy[i].y + 1, &enermy_bullet[i]);

			}
			else if (current_map[enermy[i].y][enermy[i].x + 3] != 0 || current_map[enermy[i].y + 1][enermy[i].x + 3] != 0)
			{
				bullet_collision(enermy[i].x + 3, enermy[i].y, &enermy_bullet[i]);
				bullet_collision(enermy[i].x + 3, enermy[i].y + 1, &enermy_bullet[i]);

			}
			else {
				enermy_bullet[i].pos_x = (enermy[i].x + 2) * 32;
				enermy_bullet[i].pos_y = enermy[i].y * 32 + 24;
				enermy_bullet[i].direction = enermy[i].direction;
				enermy_bullet[i].life = live;
			}
		}
	}
	Sleep(50);
}
//敌方坦克路由控制
Direction enermy_tank_route(Tank* tank) {
	while (tank->life == live)
	{
		if (tank->x <= 12 && tank->y < 25) {
			int random = rand() % 2;
			if (random == 1)
			{
				return Right;
			}
			else {
				return Down;
			}
		}
		else if ((tank->x > 12 && tank->y < 25))
		{
			int random = rand() % 2;
			if (random == 1)
			{
				return Left;
			}
			else {
				return Down;
			}
		}
	}
}



// 玩家1的按键处理函数及行动
void player1_key_handler() {
    while (player1.life == live) {
        if (GetAsyncKeyState('W') & 0x8000) {
            if (player1.direction == UP && player1.y - 1 >= 0 && current_map[player1.y - 1][player1.x] == 0 && current_map[player1.y - 1][player1.x + 1] == 0) {
                set_map(player1.x, player1.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player1.x * 32, player1.y * 32, player1.x * 32 + 64, player1.y * 32 + 64);
                player1.y--;
                set_map(player1.x, player1.y, 200);
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
            else if (player1.direction != UP) {
                player1.direction = UP;
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            if (player1.direction == Down && player1.y + 2 < 26 && current_map[player1.y + 2][player1.x] == 0 && current_map[player1.y + 2][player1.x + 1] == 0) {
                set_map(player1.x, player1.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player1.x * 32, player1.y * 32, player1.x * 32 + 64, player1.y * 32 + 64);
                player1.y++;
                set_map(player1.x, player1.y, 200);
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
            else if (player1.direction != Down) {
                player1.direction = Down;
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            if (player1.direction == Left && player1.x - 1 >= 0 && current_map[player1.y][player1.x - 1] == 0 && current_map[player1.y + 1][player1.x - 1] == 0) {
                set_map(player1.x, player1.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player1.x * 32, player1.y * 32, player1.x * 32 + 64, player1.y * 32 + 64);
                player1.x--;
                set_map(player1.x, player1.y, 200);
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
            else if (player1.direction != Left) {
                player1.direction = Left;
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            if (player1.direction == Right && player1.x + 2 < 26 && current_map[player1.y][player1.x + 2] == 0 && current_map[player1.y + 1][player1.x + 2] == 0) {
                set_map(player1.x, player1.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player1.x * 32, player1.y * 32, player1.x * 32 + 64, player1.y * 32 + 64);
                player1.x++;
                set_map(player1.x, player1.y, 200);
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
            else if (player1.direction != Right) {
                player1.direction = Right;
                putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]);
            }
        }
        if (GetAsyncKeyState('J') & 0x8000) {
            if (player1_bullet.life == dead) {
                if (player1.direction == UP) {
                    if (current_map[player1.y - 1][player1.x] != 0 || current_map[player1.y - 1][player1.x + 1] != 0) {
                        bullet_collision(player1.x, player1.y - 1, &player1_bullet);
                        bullet_collision(player1.x + 1, player1.y - 1, &player1_bullet);
                    }
                    else if (current_map[player1.y - 2][player1.x] != 0 || current_map[player1.y - 2][player1.x + 1] != 0) {
                        bullet_collision(player1.x, player1.y - 2, &player1_bullet);
                        bullet_collision(player1.x + 1, player1.y - 2, &player1_bullet);
                    }
                    else {
                        player1_bullet.pos_x = player1.x * 32 + 24;
                        player1_bullet.pos_y = (player1.y - 1) * 32;
                        player1_bullet.direction = player1.direction;
                        player1_bullet.life = live;
                    }
                }
                else if (player1.direction == Down) {
                    if (current_map[player1.y + 2][player1.x] != 0 || current_map[player1.y + 2][player1.x + 1] != 0) {
                        bullet_collision(player1.x, player1.y + 2, &player1_bullet);
                        bullet_collision(player1.x + 1, player1.y + 2, &player1_bullet);
                    }
                    else if (current_map[player1.y + 3][player1.x] != 0 || current_map[player1.y + 3][player1.x + 1] != 0) {
                        bullet_collision(player1.x, player1.y + 3, &player1_bullet);
                        bullet_collision(player1.x + 1, player1.y + 3, &player1_bullet);
                    }
                    else {
                        player1_bullet.pos_x = player1.x * 32 + 24;
                        player1_bullet.pos_y = (player1.y + 2) * 32;
                        player1_bullet.direction = player1.direction;
                        player1_bullet.life = live;
                    }
                }
                else if (player1.direction == Left) {
                    if (current_map[player1.y][player1.x - 1] != 0 || current_map[player1.y + 1][player1.x - 1] != 0) {
                        bullet_collision(player1.x - 1, player1.y, &player1_bullet);
                        bullet_collision(player1.x - 1, player1.y + 1, &player1_bullet);
                    }
                    else if (current_map[player1.y][player1.x - 2] != 0 || current_map[player1.y + 1][player1.x - 2] != 0) {
                        bullet_collision(player1.x - 2, player1.y, &player1_bullet);
                        bullet_collision(player1.x - 2, player1.y + 1, &player1_bullet);
                    }
                    else {
                        player1_bullet.pos_x = (player1.x - 1) * 32;
                        player1_bullet.pos_y = player1.y * 32 + 24;
                        player1_bullet.direction = player1.direction;
                        player1_bullet.life = live;
                    }
                }
                else if (player1.direction == Right) {
                    if (current_map[player1.y][player1.x + 2] != 0 || current_map[player1.y + 1][player1.x + 2] != 0) {
                        bullet_collision(player1.x + 2, player1.y, &player1_bullet);
                        bullet_collision(player1.x + 2, player1.y + 1, &player1_bullet);
                    }
                    else if (current_map[player1.y][player1.x + 3] != 0 || current_map[player1.y + 1][player1.x + 3] != 0) {
                        bullet_collision(player1.x + 3, player1.y, &player1_bullet);
                        bullet_collision(player1.x + 3, player1.y + 1, &player1_bullet);
                    }
                    else {
                        player1_bullet.pos_x = (player1.x + 2) * 32;
                        player1_bullet.pos_y = player1.y * 32 + 24;
                        player1_bullet.direction = player1.direction;
                        player1_bullet.life = live;
                    }
                }
            }
        }
        Sleep(50); // 控制移动速度
    }
}

// 玩家2的按键处理函数及行动
void player2_key_handler() {
    while (player2.life == live) {
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (player2.direction == UP && player2.y - 1 >= 0 && current_map[player2.y - 1][player2.x] == 0 && current_map[player2.y - 1][player2.x + 1] == 0) {
                set_map(player2.x, player2.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player2.x * 32, player2.y * 32, player2.x * 32 + 64, player2.y * 32 + 64);
                player2.y--;
                set_map(player2.x, player2.y, 300);
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
            else if (player2.direction != UP) {
                player2.direction = UP;
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (player2.direction == Down && player2.y + 2 < 26 && current_map[player2.y + 2][player2.x] == 0 && current_map[player2.y + 2][player2.x + 1] == 0) {
                set_map(player2.x, player2.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player2.x * 32, player2.y * 32, player2.x * 32 + 64, player2.y * 32 + 64);
                player2.y++;
                set_map(player2.x, player2.y, 300);
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
            else if (player2.direction != Down) {
                player2.direction = Down;
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (player2.direction == Left && player2.x - 1 >= 0 && current_map[player2.y][player2.x - 1] == 0 && current_map[player2.y + 1][player2.x - 1] == 0) {
                set_map(player2.x, player2.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player2.x * 32, player2.y * 32, player2.x * 32 + 64, player2.y * 32 + 64);
                player2.x--;
                set_map(player2.x, player2.y, 300);
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
            else if (player2.direction != Left) {
                player2.direction = Left;
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (player2.direction == Right && player2.x + 2 < 26 && current_map[player2.y][player2.x + 2] == 0 && current_map[player2.y + 1][player2.x + 2] == 0) {
                set_map(player2.x, player2.y, 0);
                setfillcolor(BLACK);
                solidrectangle(player2.x * 32, player2.y * 32, player2.x * 32 + 64, player2.y * 32 + 64);
                player2.x++;
                set_map(player2.x, player2.y, 300);
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
            else if (player2.direction != Right) {
                player2.direction = Right;
                putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]);
            }
        }
        if (GetAsyncKeyState(VK_ADD) & 0x8000) {
            if (player2_bullet.life == dead) {
                if (player2.direction == UP) {
                    if (current_map[player2.y - 1][player2.x] != 0 || current_map[player2.y - 1][player2.x + 1] != 0) {
                        bullet_collision(player2.x, player2.y - 1, &player2_bullet);
                        bullet_collision(player2.x + 1, player2.y - 1, &player2_bullet);
                    }
                    else if (current_map[player2.y - 2][player2.x] != 0 || current_map[player2.y - 2][player2.x + 1] != 0) {
                        bullet_collision(player2.x, player2.y - 2, &player2_bullet);
                        bullet_collision(player2.x + 1, player2.y - 2, &player2_bullet);
                    }
                    else {
                        player2_bullet.pos_x = player2.x * 32 + 24;
                        player2_bullet.pos_y = (player2.y - 1) * 32;
                        player2_bullet.direction = player2.direction;
                        player2_bullet.life = live;
                    }
                }
                else if (player2.direction == Down) {
                    if (current_map[player2.y + 2][player2.x] != 0 || current_map[player2.y + 2][player2.x + 1] != 0) {
                        bullet_collision(player2.x, player2.y + 2, &player2_bullet);
                        bullet_collision(player2.x + 1, player2.y + 2, &player2_bullet);
                    }
                    else if (current_map[player2.y + 3][player2.x] != 0 || current_map[player2.y + 3][player2.x + 1] != 0) {
                        bullet_collision(player2.x, player2.y + 3, &player2_bullet);
                        bullet_collision(player2.x + 1, player2.y + 3, &player2_bullet);
                    }
                    else {
                        player2_bullet.pos_x = player2.x * 32 + 24;
                        player2_bullet.pos_y = (player2.y + 2) * 32;
                        player2_bullet.direction = player2.direction;
                        player2_bullet.life = live;
                    }
                }
                else if (player2.direction == Left) {
                    if (current_map[player2.y][player2.x - 1] != 0 || current_map[player2.y + 1][player2.x - 1] != 0) {
                        bullet_collision(player2.x - 1, player2.y, &player2_bullet);
                        bullet_collision(player2.x - 1, player2.y + 1, &player2_bullet);
                    }
                    else if (current_map[player2.y][player2.x - 2] != 0 || current_map[player2.y + 1][player2.x - 2] != 0) {
                        bullet_collision(player2.x - 2, player2.y, &player2_bullet);
                        bullet_collision(player2.x - 2, player2.y + 1, &player2_bullet);
                    }
                    else {
                        player2_bullet.pos_x = (player2.x - 1) * 32;
                        player2_bullet.pos_y = player2.y * 32 + 24;
                        player2_bullet.direction = player2.direction;
                        player2_bullet.life = live;
                    }
                }
                else if (player2.direction == Right) {
                    if (current_map[player2.y][player2.x + 2] != 0 || current_map[player2.y + 1][player2.x + 2] != 0) {
                        bullet_collision(player2.x + 2, player2.y, &player2_bullet);
                        bullet_collision(player2.x + 2, player2.y + 1, &player2_bullet);
                    }
                    else if (current_map[player2.y][player2.x + 3] != 0 || current_map[player2.y + 1][player2.x + 3] != 0) {
                        bullet_collision(player2.x + 3, player2.y, &player2_bullet);
                        bullet_collision(player2.x + 3, player2.y + 1, &player2_bullet);
                    }
                    else {
                        player2_bullet.pos_x = (player2.x + 2) * 32;
                        player2_bullet.pos_y = player2.y * 32 + 24;
                        player2_bullet.direction = player2.direction;
                        player2_bullet.life = live;
                    }
                }
            }
        }
        Sleep(50); // 控制移动速度
    }
}


//初始化函数
void init() {
	srand(time(NULL));
	load_Res();
	player2.life = dead;
	if (game_mode == one_player) { init_player1(); }
	if (game_mode == two_players)
	{
		init_player1();
		init_player2();
	}
	init_enermy();
	load_map(map1);
	play_music("./Res/audio/bk.mp3", 200, true);
}
//主绘制函数
void draw() {
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			if (current_map[i][j] == 0) {
				putimage(j * 32, i * 32, &block[0]);
			}
			if (current_map[i][j] == 1) {
				putimage(j * 32, i * 32, &block[1]);
			}
			if (current_map[i][j] == 2) {
				putimage(j * 32, i * 32, &block[2]);
			}
			if (current_map[i][j] == 3) {
				putimage(j * 32, i * 32, &block[3]);
				set_map(j, i, 4);
			}
		}
	}
	if (game_mode == two_players && player2.life == live) { putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]); }
	if (player1.life == live) { putimage(player1.x * 32, player1.y * 32, &player1_Img[player1.direction]); }
	if (game_mode == two_players && player2.life == live) { putimage(player2.x * 32, player2.y * 32, &player2_Img[player2.direction]); }
	for (int i = 0; i < enermy_num; i++)
	{
		if (enermy[i].life == live) {
			putimage(enermy[i].x * 32, enermy[i].y * 32, &enermy_Img[enermy[i].direction]);
		}
	}
}
//胜利或者失败判断
void game_over() {
	if (player1.life == dead && player2.life == dead || flag == defeat) {
		mciSendString("close bk", NULL, 0, NULL);
		mciSendString("open ./Res/audio/gameover.mp3 alias gameover", NULL, 0, NULL);
		mciSendString("play gameover", NULL, 0, NULL);
		mciSendString("setaudio gameover volume to 1000", NULL, 0, NULL);
		settextcolor(RED);
		settextstyle(100, 50, "楷体", 0, 0, 1000, true, false, false);
		setbkmode(TRANSPARENT);
		outtextxy(200, 200, "GAME OVER");
		Sleep(5000);
		exit(0);
	}
	int count = 0;
	for (int i = 0; i < enermy_num; i++){
		if (enermy[i].life == dead) {
			count++;
		}
		if (count == enermy_num) {
			mciSendString("close bk", NULL, 0, NULL);
			mciSendString("open ./Res/audio/victory.mp3 alias victory", NULL, 0, NULL);
			mciSendString("play victory", NULL, 0, NULL);
			mciSendString("setaudio victory volume to 1000", NULL, 0, NULL);
			settextcolor(RED);
			settextstyle(100, 50, "楷体", 0, 0, 1000, true, false, false);
			setbkmode(TRANSPARENT);
			outtextxy(200, 200, "VICTORY");
			Sleep(5000);
			exit(0);
		}
	}
}



// 子线程函数：玩家1移动
DWORD WINAPI player1_move_thread(LPVOID lpParameter) {
	if (player1.life == live) {
		while (1) {
			if (player1.life == live) {
				player1_key_handler();
			
				Sleep(50); // 控制移动速度
			}
		}
	}
	return 0;
}

// 子线程函数：玩家2移动
DWORD WINAPI player2_move_thread(LPVOID lpParameter) {
	if (player2.life == live) {
		while (1) {
			if (game_mode == two_players && player2.life == live) {
				player2_key_handler();
				
				Sleep(50); // 控制移动速度
			}
		}
	}
	return 0;
}

// 子线程函数：敌人移动
DWORD WINAPI enermy_move_thread(LPVOID lpParameter) {
	while (true) { 
			for (int i = 0; i < enermy_num; i++)
			{
				enermy[i].direction = enermy_tank_route(&enermy[i]);
				if (enermy[i].life == live) enermy_tank_move(&enermy[i], enermy_Img, enermy[i].direction, i);
			}
		Sleep(500); // 控制移动速度
	}
	return 0;
}
// 子线程函数：子弹移动
DWORD WINAPI bullet_move_thread(LPVOID lpParameter) {
	while (1) {
		for (int  i = 0; i < enermy_num; i++)
		{
			bullet_move(&enermy_bullet[i]);
		}
		bullet_move(&player1_bullet); 
		if (game_mode == two_players)bullet_move(&player2_bullet);
	}
	return 0;
}
// 主线程函数：绘制界面
DWORD WINAPI draw_thread(LPVOID lpParameter) {
	while (1) {
		draw();
		for (int i = 0; i < 26; i++)
		{
			printf("\n");
			for (int j = 0; j < 26; j++)
			{
				printf("%d ", current_map[i][j]);
			}
		}
		printf("\n\n\n");
		game_over();
		Sleep(250); // 控制绘制速度
	}
	return 0;
}

int main(){
	initgraph(widnows_width, windows_height,EX_SHOWCONSOLE);
	start_menu();
	init();
	
	// 创建子线程
	HANDLE hThread[5];
	DWORD threadId[5];
	hThread[0] = CreateThread(NULL, 0, bullet_move_thread, NULL, 0, &threadId[0]);
	hThread[1] = CreateThread(NULL, 0, player1_move_thread, NULL, 0, &threadId[1]);
	hThread[2] = CreateThread(NULL, 0, player2_move_thread, NULL, 0, &threadId[2]);
	hThread[3] = CreateThread(NULL, 0, enermy_move_thread, NULL, 0, &threadId[3]);
	hThread[4] = CreateThread(NULL, 0, draw_thread, NULL, 0, &threadId[4]);

	// 检查线程是否创建成功
	for (int i = 0; i < 5; i++) {
		if (hThread[i] == NULL) {
			printf("线程 %d 创建失败，错误代码: %d\n", i, GetLastError());
			// 关闭已创建的线程句柄
			for (int j = 0; j < i; j++) {
				CloseHandle(hThread[j]);
			}
			closegraph();
			return 1;
		}
	}

	// 等待子线程结束
	WaitForSingleObject(hThread[0], INFINITE);
	WaitForSingleObject(hThread[1], INFINITE);
	WaitForSingleObject(hThread[2], INFINITE);
	WaitForSingleObject(hThread[3], INFINITE);
	WaitForSingleObject(hThread[4], INFINITE);

	// 关闭线程句柄
	for (int i = 0; i < 5; i++) {
		CloseHandle(hThread[i]);
	}

	closegraph();
	return 0;
}
