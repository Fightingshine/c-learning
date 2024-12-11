#include "client.h"

card player1_cards[4];
card player2_cards[4];
card player3_cards[4];
card player4_cards[4];
card player1_doubt_list[12];
card player2_doubt_list[12];
card player3_doubt_list[12];
card player4_doubt_list[12];
card preservation[16];
enum player_type turn;

int client_id;
int menu1_open = 0;
int menu2_open = 0;
ExMessage msg;
SOCKET socketfd;

IMAGE card_image[14];
IMAGE block_mini[4];
IMAGE preservation_image;
IMAGE block_minimum;
IMAGE block_minimum_plus;
IMAGE block_mini_dark[4];

CRITICAL_SECTION cs1;
CRITICAL_SECTION cs2;
CONDITION_VARIABLE cv1;
CONDITION_VARIABLE cv2;
int data_ready = 1;
int data_updated = 0; // 添加标志位

// 发送游戏状态的函数
void SendGameState(SOCKET sock)
{
    EnterCriticalSection(&cs1);

    // 计算总数据大小
    int total_size = sizeof(player1_cards) + sizeof(player2_cards)
        + sizeof(player3_cards) + sizeof(player4_cards)
        + sizeof(player1_doubt_list) + sizeof(player2_doubt_list)
        + sizeof(player3_doubt_list) + sizeof(player4_doubt_list)
        + sizeof(preservation) + sizeof(turn);

    char* buffer = (char*)malloc(total_size);
    char* ptr = buffer;

    // 将数据复制到缓冲区
    memcpy(ptr, player1_cards, sizeof(player1_cards));
    ptr += sizeof(player1_cards);

    memcpy(ptr, player2_cards, sizeof(player2_cards));
    ptr += sizeof(player2_cards);

    memcpy(ptr, player3_cards, sizeof(player3_cards));
    ptr += sizeof(player3_cards);

    memcpy(ptr, player4_cards, sizeof(player4_cards));
    ptr += sizeof(player4_cards);

    memcpy(ptr, player1_doubt_list, sizeof(player1_doubt_list));
    ptr += sizeof(player1_doubt_list);

    memcpy(ptr, player2_doubt_list, sizeof(player2_doubt_list));
    ptr += sizeof(player2_doubt_list);

    memcpy(ptr, player3_doubt_list, sizeof(player3_doubt_list));
    ptr += sizeof(player3_doubt_list);

    memcpy(ptr, player4_doubt_list, sizeof(player4_doubt_list));
    ptr += sizeof(player4_doubt_list);

    memcpy(ptr, preservation, sizeof(preservation));
    ptr += sizeof(preservation);

    memcpy(ptr, &turn, sizeof(turn));
    ptr += sizeof(turn);

    // 发送数据
    int sendlen = send(sock, buffer, total_size, 0);
    if (sendlen == SOCKET_ERROR)
    {
        printf("数据发送失败\n");
    }
    else
    {
        printf("数据发送成功，发送字节数：%d\n", sendlen);
    }

    free(buffer);

    data_ready = 1;
    WakeConditionVariable(&cv1);
    LeaveCriticalSection(&cs1);
}
// 接收游戏状态的函数
void RecvGameState(SOCKET sock)
{
    EnterCriticalSection(&cs2);
    // 计算总数据大小
    int total_size = sizeof(player1_cards) + sizeof(player2_cards)
        + sizeof(player3_cards) + sizeof(player4_cards)
        + sizeof(player1_doubt_list) + sizeof(player2_doubt_list)
        + sizeof(player3_doubt_list) + sizeof(player4_doubt_list)
        + sizeof(preservation) + sizeof(turn);
    char* buffer = (char*)malloc(total_size);
    char* ptr = buffer;
    // 接收数据
    int recvlen = recv(sock, buffer, total_size, 0);
    if (recvlen == SOCKET_ERROR)
    {
        printf("数据接收失败\n");
    }
    else
    {
        printf("数据接收成功，接收字节数：%d\n", recvlen);
    }
    // 将数据复制到变量
    memcpy(player1_cards, ptr, sizeof(player1_cards));
    ptr += sizeof(player1_cards);
    memcpy(player2_cards, ptr, sizeof(player2_cards));
    ptr += sizeof(player2_cards);
    memcpy(player3_cards, ptr, sizeof(player3_cards));
    ptr += sizeof(player3_cards);
    memcpy(player4_cards, ptr, sizeof(player4_cards));
    ptr += sizeof(player4_cards);
    memcpy(player1_doubt_list, ptr, sizeof(player1_doubt_list));
    ptr += sizeof(player1_doubt_list);
    memcpy(player2_doubt_list, ptr, sizeof(player2_doubt_list));
    ptr += sizeof(player2_doubt_list);
    memcpy(player3_doubt_list, ptr, sizeof(player3_doubt_list));
    ptr += sizeof(player3_doubt_list);
    memcpy(player4_doubt_list, ptr, sizeof(player4_doubt_list));
    ptr += sizeof(player4_doubt_list);
    memcpy(preservation, ptr, sizeof(preservation));
    ptr += sizeof(preservation);
    memcpy(&turn, ptr, sizeof(turn));
    ptr += sizeof(turn);

    // 打印接收到的数据
    for (int i = 0; i < 4; i++)
    {
        printf("player1_cards[%d].type: %d\n", i, player1_cards[i].type);
        printf("player1_cards[%d].value: %d\n", i, player1_cards[i].value);
        printf("player1_cards[%d].player: %d\n", i, player1_cards[i].player);
        printf("player2_cards[%d].type: %d\n", i, player2_cards[i].type);
        printf("player2_cards[%d].value: %d\n", i, player2_cards[i].value);
        printf("player2_cards[%d].player: %d\n", i, player2_cards[i].player);
        printf("player3_cards[%d].type: %d\n", i, player3_cards[i].type);
        printf("player3_cards[%d].value: %d\n", i, player3_cards[i].value);
        printf("player3_cards[%d].player: %d\n", i, player3_cards[i].player);
        printf("player4_cards[%d].type: %d\n", i, player4_cards[i].type);
        printf("player4_cards[%d].value: %d\n", i, player4_cards[i].value);
        printf("player4_cards[%d].player: %d\n", i, player4_cards[i].player);
    }
    free(buffer);

    data_ready = 0;
    data_updated = 1; // 设置标志位
    WakeConditionVariable(&cv2); 
    LeaveCriticalSection(&cs2);
}
//下一个回合
player_type next_turn(player_type turn) {
    if (turn == player1) {
        return player2;
    }
    else if (turn == player2) {
        return player3;
    }
    else if (turn == player3) {
        return player4;
    }
    else if (turn == player4) {
        return player1;
    }
}
// 接收客户端 ID 的函数
int RecvClientID(SOCKET sock, int* client_id)
{
    char id_msg[256];
    int recvlen = recv(sock, id_msg, 20, 0);
    if (recvlen == SOCKET_ERROR)
    {
        printf("接收客户端 ID 失败\n");
    }
    else
    {
        sscanf(id_msg, "您的客户端 ID 是: %d\n", client_id);
        printf("接收到的客户端 ID: %d\n", *client_id);
        return *client_id;
    }
    return -1;
}
// 读取卡牌图片
void LoadCardImage()
{
	loadimage(&card_image[block], _T("./Res/block.jpg"));
    loadimage(&card_image[president], _T("./Res/president.jpg"));
    loadimage(&card_image[monitor], _T("./Res/monitor.jpg"));
    loadimage(&card_image[top_student], _T("./Res/top_student.jpg"));
    loadimage(&card_image[young_lady], _T("./Res/young_lady.jpg"));
    loadimage(&card_image[news], _T("./Res/news.jpg"));
    loadimage(&card_image[book], _T("./Res/book.jpg"));
    loadimage(&card_image[discipline], _T("./Res/discipline.jpg"));
    loadimage(&card_image[health_care], _T("./Res/health_care.jpg"));
    loadimage(&card_image[home_goer], _T("./Res/home_goer.jpg"));
    loadimage(&card_image[prisoner], _T("./Res/prisoner.jpg"));
    loadimage(&card_image[accomplice], _T("./Res/accomplice.jpg"));
    loadimage(&card_image[infected_person], _T("./Res/infected_person.jpg"));
    loadimage(&card_image[alien], _T("./Res/alien.jpg"));
    loadimage(&block_mini[up], _T("./Res/block_up.jpg"));
    loadimage(&block_mini[down], _T("./Res/block_down.jpg"));
    loadimage(&block_mini[left], _T("./Res/block_left.jpg"));
    loadimage(&block_mini[right], _T("./Res/block_right.jpg"));
	loadimage(&preservation_image, _T("./Res/preservation.jpg"));
	loadimage(&block_minimum, _T("./Res/block_minimum.jpg"));
	loadimage(&block_mini_dark[up], _T("./Res/block_mini_dark_up.jpg"));
	loadimage(&block_mini_dark[down], _T("./Res/block_mini_dark_down.jpg"));
	loadimage(&block_mini_dark[left], _T("./Res/block_mini_dark_left.jpg"));
	loadimage(&block_mini_dark[right], _T("./Res/block_mini_dark_right.jpg"));
	loadimage(&block_minimum_plus, _T("./Res/block_minimum_plus.jpg"));
}
//改变窗口标题
 void changeWindowTitle(const char* title) {
    HWND hnd = GetHWnd(); // 获取窗口句柄
    SetWindowText(hnd, title); // 设置窗口标题
}
// 获取图像函数
IMAGE* get_image(card_type type, direction dirc) {
    if (type != block) {
		return &block_mini[dirc];
    }
    else {
		return &block_mini_dark[dirc];
    }
}
//文字绘制函数
void text_draw(int x, int y, const char* text) {
	settextstyle(20, 0, _T("宋体"));
	setbkmode(TRANSPARENT);
	outtextxy(x, y, text);
}
//覆盖函数
void fulfill(int x, int y,int width,int length) {
	setlinecolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(x, y, x + width, y + length);
}
//一级菜单绘制函数
void menu_draw_1(int x, int y) {
	setfillcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(x, y, x + menu_length, y + menu_height);
    fillrectangle(x, y+20, x + menu_length, y + 20+ menu_height);
    text_draw(x, y, "调和");
    text_draw(x, y+20, "怀疑");
	menu1_open = 1;
}
//二级菜单绘制函数
void menu_draw_2(int x, int y) {
	setfillcolor(BLACK);
	setbkmode(TRANSPARENT);
    if (client_id == 1) {
        fillrectangle(x , y - 20, x + 40, y + 20);
        fillrectangle(x , y, x + 40, y + 20);
        fillrectangle(x , y + 20, x + 40, y + 20);
        text_draw(x, y - 20, "玩家二");
        text_draw(x, y, "玩家三");
        text_draw(x, y + 20, "玩家四");
        menu1_open = 0;
        menu2_open = 1;
    }
    else if (client_id == 2) {
        fillrectangle(x , y - 20, x + 40, y + 20);
        fillrectangle(x , y, x + 40, y + 20);
        fillrectangle(x , y + 20, x + 40, y + 20);
		text_draw(x , y - 20, "玩家一");
		text_draw(x , y, "玩家三");
		text_draw(x , y + 20, "玩家四");
        menu1_open = 0;
        menu2_open = 1;
       
    }
	else if (client_id == 3) {
		fillrectangle(x , y - 20, x  + 40, y + 20);
		fillrectangle(x , y, x + 40 , y + 20);
		fillrectangle(x , y + 20, x  + 40, y + 20);
		text_draw(x , y - 20, "玩家一");
		text_draw(x , y , "玩家二");
		text_draw(x , y + 20, "玩家四");
        menu1_open = 0;
        menu2_open = 1;
	}
	else if (client_id == 4) {
		fillrectangle(x , y - 20, x + 40, y + 20);
		fillrectangle(x , y, x + 40, y + 20);
		fillrectangle(x , y + 20, x + 40, y + 20);
		text_draw(x , y - 20, "玩家一");
		text_draw(x , y , "玩家二");
		text_draw(x , y + 20, "玩家三");		
        menu1_open = 0;
        menu2_open = 1;
	}
	
}
//卡牌转移函数
void card_transfer(card* ori, card* des) {
	card temp;
	temp = *ori;
	*ori = *des;
	*des = temp;
}
//转换到怀疑列表
void transfer_to_doubt_list(int x, int y, int i) {
    if (client_id == 1)
    {
        if (msg.x >= x && msg.y >= y && msg.x <= x + 60 && msg.y <= y + 20) {
            for (int j =0; j < 12; j++) {
                if (player2_doubt_list[j].player == default_player) {
                    card_transfer(&player1_cards[i], &player2_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 20 && msg.x <= x + 60 && msg.y <= y + 20 + 20) {
            for (int j =0; j < 12; j++) {
                if (player3_doubt_list[j].player == default_player) {
                    card_transfer(&player1_cards[i], &player3_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 40 && msg.x <= x + 60 && msg.y <= y + 40 + 20) {
            for (int j =0 ; j < 12; j++) {
                if (player4_doubt_list[j].player == default_player) {
                    card_transfer(&player1_cards[i], &player4_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
    }
    else if (client_id == 2) {
        if (msg.x >= x && msg.y >= y && msg.x <= x + 60 && msg.y <= y + 20) {
            for (int j =0; j < 12; j++) {
                if (player1_doubt_list[j].player == default_player) {
                    card_transfer(&player2_cards[i], &player1_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 20 && msg.x <= x + 60 && msg.y <= y + 20 + 20) {
            for (int j =0; j < 12; j++) {
                if (player3_doubt_list[j].player == default_player) {
                    card_transfer(&player2_cards[i], &player3_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);                    
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 40 && msg.x <= x + 60 && msg.y <= y + 40 + 20) {
            for (int j =0; j < 12; j++) {
                if (player4_doubt_list[j].player == default_player) {
                    card_transfer(&player2_cards[i], &player4_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
    }
    else if (client_id == 3) {
        if (msg.x >= x && msg.y >= y && msg.x <= x + 60 && msg.y <= y + 20) {
            for (int j =0; j < 12; j++) {
                if (player1_doubt_list[j].player == default_player) {
                    card_transfer(&player3_cards[i], &player1_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 20 && msg.x <= x + 60 && msg.y <= y + 20 + 20) {
            for (int j =0 ; j < 12; j++) {
                if (player2_doubt_list[j].player == default_player) {
                    card_transfer(&player3_cards[i], &player2_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 40 && msg.x <= x + 60 && msg.y <= y + 40 + 20) {
            for (int j =0; j < 12; j++) {
                if (player4_doubt_list[j].player == default_player) {
                    card_transfer(&player3_cards[i], &player4_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
    }
    else if (client_id == 4) {
        if (msg.x >= x && msg.y >= y && msg.x <= x + 60 && msg.y <= y + 20) {
            for (int j =0; j < 12; j++) {
                if (player1_doubt_list[j].player == default_player) {
                    card_transfer(&player4_cards[i], &player1_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 20 && msg.x <= x + 60 && msg.y <= y + 20 + 20) {
            for (int j =0; j < 12; j++) {
                if (player2_doubt_list[j].player == default_player) {
                    card_transfer(&player4_cards[i], &player2_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
        else if (msg.x >= x && msg.y >= y + 40 && msg.x <= x + 60 && msg.y <= y + 40 + 20) {
            for (int j =0; j < 12; j++) {
                if (player3_doubt_list[j].player == default_player) {
                    card_transfer(&player4_cards[i], &player3_doubt_list[j]);
                    fulfill(x, y, 60, 60);
					turn = next_turn(turn);
					SendGameState(socketfd);
                    break;
                }
            }
        }
    }
}
//转移到调和列表
void transfer_to_preservation(int i) {
    if (client_id == 1) {
        for (int j = 0; j < 16; j++)
        {
            if (preservation[j].player == default_player)
            {
                card_transfer(&player1_cards[i], &preservation[j]);
				turn = next_turn(turn);
				SendGameState(socketfd);
                break;
            }
        }
    }
	else if (client_id == 2) {
		for (int j = 0; j < 16; j++)
		{
			if (preservation[j].player == default_player)
			{
				card_transfer(&player2_cards[i], &preservation[j]);
				turn = next_turn(turn);
				SendGameState(socketfd);
				break;
			}
		}
	}
	else if (client_id == 3) {
		for (int j = 0; j < 16; j++)
		{
			if (preservation[j].player == default_player)
			{
				card_transfer(&player3_cards[i], &preservation[j]);
				turn = next_turn(turn);
				SendGameState(socketfd);
				break;
			}
		}
	}
	else if (client_id == 4) {
		for (int j = 0; j < 16; j++)
		{
			if (preservation[j].player == default_player)
			{
				card_transfer(&player4_cards[i], &preservation[j]);
				turn = next_turn(turn);
				SendGameState(socketfd);
				break;
			}
		}
	}
	
}
//调和列表绘制
void preservation_draw() {
    for (int i = 0; i < 8; i++)
    {
        if(preservation[i].player != default_player)
		{
			putimage(30 * i + 180+120+10, 120, &block_minimum);
		}
    }
    for (int j = 8; j < 16; j++)
    {
        if (preservation[j].player != default_player)
        {
            putimage(30 * (j-8) + 180 + 120 + 10, 120+55, &block_minimum);
        }
    }
}
//各个分区怀疑列表绘制
void doubt_list_pre_draw(card* player_doubt_list1, card* player_doubt_list2, card* player_doubt_list3, card* player_doubt_list4) {
    for (int i = 0; i < 12; i++)
    {
        if (i < 3 && player_doubt_list1[i].player != default_player) {
            putimage(10 + i * (30 + 5), 300+45 , &block_minimum);
        }
        else if (i >= 3 && i < 6 && player_doubt_list1[i].player != default_player) {
            putimage(10 + (i-3) * (30 + 5), 300 +45 +55, &block_minimum);
        }
        else if (i >= 6 && i < 9 && player_doubt_list1[i].player != default_player) {
            putimage(10+600 + (i-6) * (30 + 5), 300+45, &block_minimum);
        }
        else if (i >= 9 && i < 12 && player_doubt_list1[i].player != default_player) {
            putimage(10+600 + (i-9) * (30 + 5), 300 +45+ 55, &block_minimum);
        }
    }
    for (int i = 0; i < 12; i++)
    {
        if (i < 6 && player_doubt_list2[i].player != default_player) {
            putimage(600, 60 + i * 30, &block_minimum_plus);
        }
        else if (i >= 6 && player_doubt_list2[i].player != default_player) {
            putimage(600 - 20, 60 + (i - 6) * 30, &block_minimum_plus);
        }
    }
    for (int i = 0; i < 12; i++)
    {
        if (i < 4 && player_doubt_list3[i].player != default_player) {
            putimage(480 + i * 20, 0, &block_minimum_plus);
        }
        else if (i >= 4 && i < 8 && player_doubt_list3[i].player != default_player) {
            putimage(480 + (i - 4) * 20, 30, &block_minimum_plus);
        }
        else if (i >= 8 && i < 12 && player_doubt_list3[i].player != default_player) {
            putimage(120 + (i - 8) * 20, 300 + 45, &block_minimum_plus);
        }
    }
    for (int i = 0; i < 12; i++)
    {
        if (i < 6 && player_doubt_list4[i].player != default_player) {
            putimage(90, 60 + i * 30, &block_minimum_plus);
        }
        else if (i >= 6 && player_doubt_list4[i].player != default_player) {
            putimage(90 + 20, 60 + (i - 6) * 30, &block_minimum_plus);
        }
    }
    
}
//总的怀疑列表绘制
void doubt_list_draw() {
    if (client_id == 1) {
		doubt_list_pre_draw(player1_doubt_list, player2_doubt_list, player3_doubt_list, player4_doubt_list);
	}
    else if (client_id == 2) {
		doubt_list_pre_draw(player2_doubt_list, player3_doubt_list, player4_doubt_list, player1_doubt_list);
    }
	else if (client_id == 3) {
		doubt_list_pre_draw(player3_doubt_list, player4_doubt_list, player1_doubt_list, player2_doubt_list);
	}
    else if (client_id == 4) {
        doubt_list_pre_draw(player4_doubt_list, player1_doubt_list, player2_doubt_list, player3_doubt_list);
    }
}
//当前回合提醒绘制
void turn_draw(){
	if (turn == player1) {
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(0, 0, 100, 20);
		text_draw(0, 0, "玩家一回合");
	}
	else if (turn == player2) {
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(0, 0, 100, 20);
		text_draw(0, 0, "玩家二回合");
	}
	else if (turn == player3) {
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(0, 0, 100, 20);
		text_draw(0, 0, "玩家三回合");
	}
	else if (turn == player4) {
		setfillcolor(BLACK);
		setlinecolor(BLACK);
		fillrectangle(0, 0, 100, 20);
		text_draw(0, 0, "玩家四回合");
	}
}
// 卡牌绘制函数
void card_draw() {
    
    if (client_id == 1) {
        for (int i = 0; i < 4; i++)
        {
            putimage((i + 1) * 120, 300, &card_image[player1_cards[i].type]);
			putimage(630, 60 * i + 60, get_image(player2_cards[i].type, left));
            text_draw(630, 40, "玩家二");
			putimage(60 * i + 240, 0, get_image(player3_cards[i].type, up));
            text_draw(240, 90, "玩家三");
			putimage(0, 60 * i + 60, get_image(player4_cards[i].type, right));
            text_draw(0, 40, "玩家四");
        }
    }
    else if (client_id == 2) {
        for (int i = 0; i < 4; i++)
        {
            putimage((i + 1) * 120, 300, &card_image[player2_cards[i].type]);
            putimage(630, 60 * i + 60, get_image(player3_cards[i].type, left));
            text_draw(630, 40, "玩家三");
            putimage(60 * i + 240, 0, get_image(player4_cards[i].type, up));
            text_draw(240, 90, "玩家四");
            putimage(0, 60 * i + 60, get_image(player1_cards[i].type, right));
            text_draw(0, 40, "玩家一");

        }
    }
    else if (client_id == 3) {
        for (int i = 0; i < 4; i++)
        {
            putimage((i + 1) * 120, 300, &card_image[player3_cards[i].type]);
            putimage(630, 60 * i + 60, get_image(player4_cards[i].type, left));
            text_draw(630, 40, "玩家四");
            putimage(60 * i + 240, 0, get_image(player1_cards[i].type, up));
            text_draw(240, 90, "玩家一");
            putimage(0, 60 * i + 60, get_image(player2_cards[i].type, right));
            text_draw(0, 40, "玩家二");
        }
    }
    else if (client_id == 4) {
        for (int i = 0; i < 4; i++)
        {
            putimage((i + 1) * 120, 300, &card_image[player4_cards[i].type]);
            putimage(630, 60 * i + 60, get_image(player1_cards[i].type, left));
            text_draw(630, 40, "玩家一");
            putimage(60 * i + 240, 0, get_image(player2_cards[i].type, up));
            text_draw(240, 90, "玩家二");
            putimage(0, 60 * i + 60, get_image(player3_cards[i].type, right));
            text_draw(0, 40, "玩家三");
        }
    }

  
}
// 主绘制函数
void draw() {
    EnterCriticalSection(&cs1);
    while (data_updated == 0)
    {
        SleepConditionVariableCS(&cv2, &cs1, INFINITE);
    }
    // 进行绘制操作
    putimage(180, 120, &preservation_image);
    card_draw();
    turn_draw();
    preservation_draw();
	doubt_list_draw();  
    data_updated = 0; 
    LeaveCriticalSection(&cs1);
}
// 初始化网络连接
SOCKET InitNetwork()
{
    // 初始化winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return INVALID_SOCKET;
    }

    // 创建socket
    SOCKET socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("socket create failed\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    // 创建服务器地址结构体
    SOCKADDR_IN serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(9999);

    // 连接服务器
    if (connect(socketfd, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("connect failed\n");
        closesocket(socketfd);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return socketfd;
}
// 接收线程
DWORD WINAPI RecvThread(LPVOID lpParam)
{
    SOCKET sock = (SOCKET)lpParam;
    while (1)
    {
        RecvGameState(sock);
    }
    return 0;
}
//绘制线程
DWORD WINAPI DrawThread(LPVOID lpParam)
{
	while (1)
	{
		draw();
	}
	return 0;
}
//被监禁结算
player_type imprison() {
    int sum_doubt[4] = { 0 };
    int max = 0;
    int max_index = 0;
    for (int i = 0; i < 12; i++)
    {
        sum_doubt[0] += player1_doubt_list[i].value;
        sum_doubt[1] += player2_doubt_list[i].value;
        sum_doubt[2] += player3_doubt_list[i].value;
        sum_doubt[3] += player4_doubt_list[i].value;
    }

    for (int i = 0; i < 4; i++)
    {
        if (sum_doubt[i] > max)
        {
            max = sum_doubt[i];
            max_index = i;
        }
    }
	if (max != 0) {
		if (max_index == 0)
		{
			return player1;
		}
		else if (max_index == 1)
		{
			return player2;
		}
		else if (max_index == 2)
		{
			return player3;
		}
		else if (max_index == 3)
		{
			return player4;
		}
	}
    else {
        return default_player;
    }
}
//游戏结束
void game_over() {
	if (turn == default_player) {
		EnterCriticalSection(&cs1);
		int sum = 0;
		char buffer[1024];
		player_type imprison_player = imprison();
		HWND hwnd = GetHWnd();
		for (int i = 0; i < 16; i++)
		{
			sum += preservation[i].value;
		}
		if (imprison_player != default_player) {
			snprintf(buffer, 1024, "游戏结束，玩家%d被监禁，调和值为%d", imprison_player, sum);
		}
		else {
			snprintf(buffer, 1024, "游戏结束，无人被监禁，调和值为%d", sum);
		}
		LeaveCriticalSection(&cs1);
		int yes = MessageBox(hwnd, buffer, "游戏结束", MB_OK);
		if (yes == IDOK) {
			exit(0);
		}
	}
}
// 初始化函数
void Initialize()
{
    initgraph(720, 480, EX_SHOWCONSOLE);
    LoadCardImage();
    InitializeCriticalSection(&cs1);
    InitializeCriticalSection(&cs2);
    InitializeConditionVariable(&cv1);
    InitializeConditionVariable(&cv2);
    socketfd = InitNetwork();
    if (socketfd == INVALID_SOCKET) {
        exit(-1);
    }
    RecvClientID(socketfd, &client_id);
    HANDLE hThread = CreateThread(NULL, 0, RecvThread, (LPVOID)socketfd, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, DrawThread, NULL, 0, NULL);
    printf("客户端 ID: %d\n", client_id);
    if (client_id == 1) {
        changeWindowTitle("玩家一");
    }
    else if (client_id == 2) {
        changeWindowTitle("玩家二");
    }
    else if (client_id == 3) {
        changeWindowTitle("玩家三");
    }
    else if (client_id == 4) {
        changeWindowTitle("玩家四");
    }
}
// 处理消息
void ProcessMessage()
{
    getmessage(&msg, EX_MOUSE);
    if (msg.message == WM_LBUTTONDOWN) {
        if (client_id == 1 && turn == player1 || client_id == 2 && turn == player2
            || client_id == 3 && turn == player3 || client_id == 4 && turn == player4) {
            if (msg.x >= 120 && msg.x <= 120 + 120 && msg.y >= 300 && msg.y <= 480) {
                fulfill(240, 300 - 40, 40, 40);
                fulfill(360, 300 - 40, 40, 40);
                fulfill(480, 300 - 40, 40, 40);
                menu_draw_1(120, 300 - 40);
            }
            else if (msg.x >= 240 && msg.x <= 240 + 120 && msg.y >= 300 && msg.y <= 480) {
                fulfill(120, 300 - 40, 40, 40);
                fulfill(360, 300 - 40, 40, 40);
                fulfill(480, 300 - 40, 40, 40);
                menu_draw_1(240, 300 - 40);
            }
            else if (msg.x >= 360 && msg.x <= 360 + 120 && msg.y >= 300 && msg.y <= 480) {
                fulfill(120, 300 - 40, 40, 40);
                fulfill(240, 300 - 40, 40, 40);
                fulfill(480, 300 - 40, 40, 40);
                menu_draw_1(360, 300 - 40);
            }
            else if (msg.x >= 480 && msg.x <= 480 + 120 && msg.y >= 300 && msg.y <= 480) {
                fulfill(120, 300 - 40, 40, 40);
                fulfill(240, 300 - 40, 40, 40);
                fulfill(360, 300 - 40, 40, 40);
                menu_draw_1(480, 300 - 40);
            }
            else {
                fulfill(120, 300 - 40, 40, 40);
                fulfill(240, 300 - 40, 40, 40);
                fulfill(360, 300 - 40, 40, 40);
                fulfill(480, 300 - 40, 40, 40);
            }

            if (menu1_open == 1) {
                if (msg.x >= 120 && msg.y >= 300 - 40 + 20 && msg.x <= 120 + 40 && msg.y <= 300 - 40 + 20 + 20)
                {
                    menu_draw_2(120 + 40, 300 - 40);
                }
                else if (msg.x >= 240 && msg.y >= 300 - 40 + 20 && msg.x <= 240 + 40 && msg.y <= 300 - 40 + 20 + 20)
                {
                    menu_draw_2(240 + 40, 300 - 40);
                }
                else if (msg.x >= 360 && msg.y >= 300 - 40 + 20 && msg.x <= 360 + 40 && msg.y <= 300 - 40 + 20 + 20)
                {
                    menu_draw_2(360 + 40, 300 - 40);
                }
                else if (msg.x >= 480 && msg.y >= 300 - 40 + 20 && msg.x <= 480 + 40 && msg.y <= 300 - 40 + 20 + 20)
                {
                    menu_draw_2(480 + 40, 300 - 40);
                }
                else {
                    fulfill(120 + 40, 300 - 40 - 20, 60, 60);
                    fulfill(240 + 40, 300 - 40 - 20, 60, 60);
                    fulfill(360 + 40, 300 - 40 - 20, 60, 60);
                    fulfill(480 + 40, 300 - 40 - 20, 60, 60);
                }
            }
            if (menu2_open == 1) {
                if (msg.x >= 120 + 40 && msg.y >= 300 - 60 && msg.x <= 120 + 40 + 60 && msg.y <= 300) {
                    transfer_to_doubt_list(120 + 40, 300 - 60, 0);
					menu2_open = 0;
                }
                else if (msg.x >= 240 + 40 && msg.y >= 300 - 60 && msg.x <= 240 + 40 + 60 && msg.y <= 300) {
                    transfer_to_doubt_list(240 + 40, 300 - 60, 1);
					menu2_open = 0;
                }
                else if (msg.x >= 360 + 40 && msg.y >= 300 - 60 && msg.x <= 360 + 40 + 60 && msg.y <= 300) {
                    transfer_to_doubt_list(360 + 40, 300 - 60, 2);
					menu2_open = 0;
                }
                else if (msg.x >= 480 + 40 && msg.y >= 300 - 60 && msg.x <= 480 + 40 + 60 && msg.y <= 300) {
                    transfer_to_doubt_list(480 + 40, 300 - 60, 3);
					menu2_open = 0;
                }
            }

            if (menu1_open == 1) {
                if (msg.x >= 120 && msg.y >= 300 - 40 && msg.x <= 120 + 40 && msg.y <= 300 - 40 + 20)
                {
                    transfer_to_preservation(0);
                }
                else if (msg.x >= 240 && msg.y >= 300 - 40 && msg.x <= 240 + 40 && msg.y <= 300 - 40 + 20)
                {
                    transfer_to_preservation(1);
                }
                else if (msg.x >= 360 && msg.y >= 300 - 40 && msg.x <= 360 + 40 && msg.y <= 300 - 40 + 20)
                {
                    transfer_to_preservation(2);
                }
                else if (msg.x >= 480 && msg.y >= 300 - 40 && msg.x <= 480 + 40 && msg.y <= 300 - 40 + 20)
                {
                    transfer_to_preservation(3);
                }
            }
        }
    }
    game_over();
}
// 清理资源
void Cleanup()
{
    closesocket(socketfd);
    WSACleanup();
    DeleteCriticalSection(&cs1);
    DeleteCriticalSection(&cs2);
}

// 主函数
int main()
{
    Initialize();
    while (true)
    {
        ProcessMessage();
    }
    Cleanup();
    return 0;
}