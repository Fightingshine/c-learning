#include <stdio.h>
#include <easyx.h>
#include<time.h>
#include<stdlib.h>

void action();
//判断游戏胜利
void victory();
//卡牌颜色
enum color
{
	blue,
	red,
};
//当前棋手阵营
int current_player;
//枚举动物类型
enum Animaltype {
	AT_None,
	AT_mouse,
	AT_cat,
	AT_dog,
	AT_wolf,
	AT_leopard,
	AT_tiger,
	AT_lion,
	AT_elephant,
	AT_Max
};

IMAGE bk;
IMAGE img_unknown;
IMAGE img_animals[17];

//载入图片资源
void loadResource() {
	loadimage(&bk, "./picture/bg1.jpg");
	loadimage(&img_unknown, "./picture/unknown.png");

	loadimage(img_animals + 0, "./picture/block.jpg");
	//Blue faction
	loadimage(img_animals + 1, "./picture/Bmouse.png");
	loadimage(img_animals + 2, "./picture/Bcat.png");
	loadimage(img_animals + 3, "./picture/Bdog.png");
	loadimage(img_animals + 4, "./picture/Bwolf.png");
	loadimage(img_animals + 5, "./picture/Bleopard.png");
	loadimage(img_animals + 6, "./picture/Btiger.png");
	loadimage(img_animals + 7, "./picture/Blion.png");
	loadimage(img_animals + 8, "./picture/Belephant.png");
	//Red faction
	loadimage(img_animals + 1 + 8, "./picture/Rmouse.png");
	loadimage(img_animals + 2 + 8, "./picture/Rcat.png");
	loadimage(img_animals + 3 + 8, "./picture/Rdog.png");
	loadimage(img_animals + 4 + 8, "./picture/Rwolf.png");
	loadimage(img_animals + 5 + 8, "./picture/Rleopard.png");
	loadimage(img_animals + 6 + 8, "./picture/Rtiger.png");
	loadimage(img_animals + 7 + 8, "./picture/Rlion.png");
	loadimage(img_animals + 8 + 8, "./picture/Relephant.png");
	
}

//获取图片的函数
IMAGE* getImage(int type) {
	//加密后的type
	if (type >= 20) {
		return &img_unknown;
	}

	//blue
	if (type >= 0 && type <= 8) {
		return img_animals + type;
	}
	//red
	else if (type > 8 && type < 2 * AT_Max - 1) {
		return img_animals + type;
	}
	return NULL;
}

//存储图片下标及类型
struct Card
{
	int row;
	int col;
	int type;
};
//宏函数:快速获得每个卡牌对应X,Y坐标
#define CardW() 100
#define CardH() 100
#define CardX(col) (col * (CardW() + 14)+18)
#define CardY(row) (row * (CardH() + 14)+98)
//卡牌数组
struct Card cards[4][4];

//card初始化函数
void card_init(Card* card, int row, int col, int type) {
	card->row = row;
	card->col = col;
	card->type = type;
}
//卡牌绘制
void card_draw(Card* card) {
	putimage(CardX(card->col ), CardY(card->row), getImage(card->type));
}
//文字绘制
void text_draw(int current_player) {
	settextcolor(RED);
	settextstyle(50, 0, "楷体");
	setbkmode(TRANSPARENT);
	if(current_player == 0)
	outtextxy(14, 20, "当前轮到：blue ");
	else
	outtextxy(14, 20, "当前轮到：red ");
}
//主绘制函数
void draw() {
	putimage(0, 0, &bk);
	text_draw(current_player);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			card_draw(&cards[i][j]);
		}
	}
}
//初始化函数
void init() {
	loadResource();
	//初始化随机数种子用于打乱卡牌时的随机函数
	srand(time(NULL));
	//初始化卡牌数组
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			card_init(&cards[i][j], i, j, i * 4 + j + 1);
		}
	}
	//打乱卡牌数组:对卡牌数组每一个元素的类型随机交换顺序
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int r = rand()%4;
			int c = rand()%4;
			int temp = cards[i][j].type ;
			cards[i][j].type = cards[r][c].type;
			cards[r][c].type = temp;
		}
	}
	//加密（加一个数让结果不在type范围内）type[0,16]+20->[20,36]
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cards[i][j].type += 20;
		}
	}
}
//判断某个坐标是否在一个矩形里
bool isIn(int x, int y, int left, int top, int w, int h) {
	if (x >= left && x <= left + w && y >= top && y <= top + h) {
		return true;
	}
	else
	{
		return false;
	}
}
//获取卡牌颜色

int card_color(Card* card) {
	if (card->type>0 && card->type<=8){
		return blue;
	}
	else if (card->type > 8 && card->type <= 16) {
		return red;
	}
}

//判断两次鼠标点击
enum State
{
	Begin,
	End,
};
struct Index
{
	int row;
	int col;
}beg = { -1,-1 }, end = {-1,-1};//两次点击对应数组下标
int state = Begin;//初始化为第一次点击

//鼠标点击对应卡牌翻开
void onMouseLButtonDown(ExMessage*msg){
	//判断是否点击的当前下标的卡牌
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//判断牌子是否被翻开
			if (isIn(msg->x,msg->y,CardX(j),CardY(i),CardW(),CardH())) {
				if (cards[i][j].type >= 20) {
					cards[i][j].type -= 20;
					current_player = (current_player == blue) ? red:blue;
				}
				//如果翻开了，继续判断能不能移动，或者吃
				else
				{
					//需知道前后两次的下标
					if (state == Begin) {
						beg = { i,j };
						state = End;
					}
					else if(state == End) {
						end = { i,j };
						state = Begin;//下一次又begin开始录入
					}
				}
			}
		}
	}
	action();
}
//游戏行动规则
void action(){
	int color_beg = card_color(&cards[beg.row][beg.col]);
	int color_end = card_color(&cards[end.row][end.col]);
	if (current_player == blue) {
		if (beg.row == end.row) {
			if (beg.col + 1 == end.col || beg.col - 1 == end.col) {
				if (color_beg != color_end) {
					if (color_beg == blue && cards[beg.row][beg.col].type >= cards[end.row][end.col].type - 8) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = red;
					}
					else if (color_beg == blue && cards[beg.row][beg.col].type == 1 && cards[end.row][end.col].type - 8 == 8) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = red;
					}
				}
			}
		}
		if (beg.col == end.col) {
			if (beg.row + 1 == end.row || beg.row - 1 == end.row) {
				if (color_beg != color_end) {
					if (color_beg == blue && cards[beg.row][beg.col].type >= cards[end.row][end.col].type - 8  ) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = red;
					}
					else if (color_beg == blue && cards[beg.row][beg.col].type == 1 && cards[end.row][end.col].type - 8 == 8) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = red;
					}
				}
			}
		}
	}
	if (current_player == red) {
		if (beg.row == end.row) {
			if (beg.col + 1 == end.col || beg.col - 1 == end.col) {
				if (color_beg != color_end) {
					if (color_beg == red && cards[beg.row][beg.col].type - 8 >= cards[end.row][end.col].type) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = blue;
					}
					else if (color_beg == red && cards[beg.row][beg.col].type - 8 == 1 && cards[end.row][end.col].type == 8) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = blue;
					}
				}
			}
		}
		if (beg.col == end.col) {
			if (beg.row + 1 == end.row || beg.row - 1 == end.row) {
				if (color_beg != color_end) {
					if (color_beg == red && cards[beg.row][beg.col].type - 8 >= cards[end.row][end.col].type) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = blue;
					}
					else if (color_beg == red && cards[beg.row][beg.col].type - 8 == 1 && cards[end.row][end.col].type == 8) {
						cards[end.row][end.col].type = cards[beg.row][beg.col].type;
						cards[beg.row][beg.col].type = AT_None;
						current_player = blue;
					}
				}
			}
		}
	}
	if (state == Begin) {
		beg = { -1,-1 };
		end = { -1,-1 };
	}
}
//判断游戏胜利
void victory() {
	int count_blue = 0;
	int count_red = 0;
	int count_Belephant = 0;
	int count_Relephant = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (cards[i][j].type >0  && cards[i][j].type <= 8 || cards[i][j].type > 20 && cards[i][j].type <= 28) {
				count_blue++;
			}
			if (cards[i][j].type == 8)count_Belephant++;
			if (cards[i][j].type > 8 && cards[i][j].type <= 16 || cards[i][j].type > 28 && cards[i][j].type <= 36) {
				count_red++;
			}
			if (cards[i][j].type == 16)count_Relephant++;
		}
	}
	if (count_blue == 0) {
		MessageBox(NULL, "red win !", "victory", MB_OKCANCEL);
	}
	else if (count_red == 0) {
		MessageBox(NULL, "blue win !", "victory", MB_OKCANCEL);
	}
}

int main(){
	initgraph(482, 642);
	init();
	while(1){
		BeginBatchDraw();//开启双缓冲
		draw();
		EndBatchDraw();//结束双缓冲
		
		//消息结构体：处理消息
		ExMessage msg;
		//获取消息
		while (peekmessage(&msg, WH_MOUSE))
		{
			//判断是否为鼠标左键点击
			if (msg.message == WM_LBUTTONDOWN) {
				onMouseLButtonDown(&msg);
			}
		}
		victory();
	}

	return 0;
}