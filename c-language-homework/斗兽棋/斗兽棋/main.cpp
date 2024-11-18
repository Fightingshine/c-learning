#include <stdio.h>
#include <easyx.h>
#include<time.h>
#include<stdlib.h>

void action();
//�ж���Ϸʤ��
void victory();
//������ɫ
enum color
{
	blue,
	red,
};
//��ǰ������Ӫ
int current_player;
//ö�ٶ�������
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

//����ͼƬ��Դ
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

//��ȡͼƬ�ĺ���
IMAGE* getImage(int type) {
	//���ܺ��type
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

//�洢ͼƬ�±꼰����
struct Card
{
	int row;
	int col;
	int type;
};
//�꺯��:���ٻ��ÿ�����ƶ�ӦX,Y����
#define CardW() 100
#define CardH() 100
#define CardX(col) (col * (CardW() + 14)+18)
#define CardY(row) (row * (CardH() + 14)+98)
//��������
struct Card cards[4][4];

//card��ʼ������
void card_init(Card* card, int row, int col, int type) {
	card->row = row;
	card->col = col;
	card->type = type;
}
//���ƻ���
void card_draw(Card* card) {
	putimage(CardX(card->col ), CardY(card->row), getImage(card->type));
}
//���ֻ���
void text_draw(int current_player) {
	settextcolor(RED);
	settextstyle(50, 0, "����");
	setbkmode(TRANSPARENT);
	if(current_player == 0)
	outtextxy(14, 20, "��ǰ�ֵ���blue ");
	else
	outtextxy(14, 20, "��ǰ�ֵ���red ");
}
//�����ƺ���
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
//��ʼ������
void init() {
	loadResource();
	//��ʼ��������������ڴ��ҿ���ʱ���������
	srand(time(NULL));
	//��ʼ����������
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			card_init(&cards[i][j], i, j, i * 4 + j + 1);
		}
	}
	//���ҿ�������:�Կ�������ÿһ��Ԫ�ص������������˳��
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
	//���ܣ���һ�����ý������type��Χ�ڣ�type[0,16]+20->[20,36]
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cards[i][j].type += 20;
		}
	}
}
//�ж�ĳ�������Ƿ���һ��������
bool isIn(int x, int y, int left, int top, int w, int h) {
	if (x >= left && x <= left + w && y >= top && y <= top + h) {
		return true;
	}
	else
	{
		return false;
	}
}
//��ȡ������ɫ

int card_color(Card* card) {
	if (card->type>0 && card->type<=8){
		return blue;
	}
	else if (card->type > 8 && card->type <= 16) {
		return red;
	}
}

//�ж����������
enum State
{
	Begin,
	End,
};
struct Index
{
	int row;
	int col;
}beg = { -1,-1 }, end = {-1,-1};//���ε����Ӧ�����±�
int state = Begin;//��ʼ��Ϊ��һ�ε��

//�������Ӧ���Ʒ���
void onMouseLButtonDown(ExMessage*msg){
	//�ж��Ƿ����ĵ�ǰ�±�Ŀ���
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//�ж������Ƿ񱻷���
			if (isIn(msg->x,msg->y,CardX(j),CardY(i),CardW(),CardH())) {
				if (cards[i][j].type >= 20) {
					cards[i][j].type -= 20;
					current_player = (current_player == blue) ? red:blue;
				}
				//��������ˣ������ж��ܲ����ƶ������߳�
				else
				{
					//��֪��ǰ�����ε��±�
					if (state == Begin) {
						beg = { i,j };
						state = End;
					}
					else if(state == End) {
						end = { i,j };
						state = Begin;//��һ����begin��ʼ¼��
					}
				}
			}
		}
	}
	action();
}
//��Ϸ�ж�����
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
//�ж���Ϸʤ��
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
		BeginBatchDraw();//����˫����
		draw();
		EndBatchDraw();//����˫����
		
		//��Ϣ�ṹ�壺������Ϣ
		ExMessage msg;
		//��ȡ��Ϣ
		while (peekmessage(&msg, WH_MOUSE))
		{
			//�ж��Ƿ�Ϊ���������
			if (msg.message == WM_LBUTTONDOWN) {
				onMouseLButtonDown(&msg);
			}
		}
		victory();
	}

	return 0;
}