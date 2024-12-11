#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")

#define MAX_CLIENTS 4

#pragma pack(push, 1)
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
typedef struct card
{
    enum card_type type;
    int value;
    enum player_type player;
} card;

typedef struct {
    SOCKET socket;
    int id; // 客户端 ID
} ClientInfo;
#pragma pack(pop)

ClientInfo client_sockets[MAX_CLIENTS];
int client_count = 0;
int next_client_id = 1; // 用于分配客户端 ID
CRITICAL_SECTION cs;

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

// 发送游戏状态的函数
int SendGameState(SOCKET sock)
{
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
        free(buffer);
        return -1; // 返回错误
    }
    else
    {
        printf("数据发送成功，发送字节数：%d\n", sendlen);
    }

    free(buffer);
    return 0; // 返回成功
}

// 接收游戏状态的函数
int RecvGameState(SOCKET sock)
{
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
    if (recvlen <= 0) // 接收错误或连接关闭
    {
        printf("数据接收失败\n");
        free(buffer);
        return -1; // 返回错误
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
    free(buffer);
    return 0; // 返回成功
}

// 游戏结束判断函数
void game_over() {
    EnterCriticalSection(&cs);
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int i;
    for (i = 0; i < 4; i++)
    {
        if (player1_cards[i].player != default_player)count1++;
        if (player2_cards[i].player != default_player)count2++;
        if (player3_cards[i].player != default_player)count3++;
        if (player4_cards[i].player != default_player)count4++;
    }
    if (count1 == 1 && count2 == 1 && count3 == 1 && count4 == 1)turn = default_player;
    LeaveCriticalSection(&cs);
}

// 客户端处理函数
DWORD WINAPI client_handler(LPVOID lpParameter) {
    ClientInfo* client_info = (ClientInfo*)lpParameter;
    SOCKET client_socket = client_info->socket;
    int client_id = client_info->id;

    // 通知客户端其分配的 ID
    char id_msg[256];
    sprintf(id_msg, "您的客户端 ID 是: %d\n", client_id);
    send(client_socket, id_msg, strlen(id_msg), 0);
    // 发送游戏状态
    if (SendGameState(client_socket) < 0) {
        printf("发送游戏状态失败，客户端 %d 已断开连接\n", client_id);
        goto cleanup;
    }

    while (1) {
        // 接收游戏状态
        if (RecvGameState(client_socket) < 0) {
            printf("接收游戏状态失败，客户端 %d 已断开连接\n", client_id);
            break;
        }
        // 游戏结束判断
        game_over();
        // 向所有客户端发送游戏状态
        EnterCriticalSection(&cs);
        int i;
        for (i = 0; i < client_count; i++) {
            if (SendGameState(client_sockets[i].socket) < 0) {
                printf("发送数据到客户端 %d 失败\n", client_sockets[i].id);
            }
        }
        LeaveCriticalSection(&cs);
    }

cleanup:
    // 客户端断开连接，移除套接字
    EnterCriticalSection(&cs);
    int i, j;
    for (i = 0; i < client_count; i++) {
        if (client_sockets[i].id == client_id) {
            for (j = i; j < client_count - 1; j++) {
                client_sockets[j] = client_sockets[j + 1];
            }
            client_count--;
            break;
        }
    }
    LeaveCriticalSection(&cs);
    closesocket(client_socket);
    free(client_info);
    return 0;
}

// 接受客户端连接的线程函数
DWORD WINAPI accept_clients(LPVOID lpParameter) {
    SOCKET socketfd = *(SOCKET*)lpParameter;
    while (1) {
        // 接受客户端连接
        SOCKADDR_IN clientaddr;
        int len = sizeof(clientaddr);
        SOCKET clientfd = accept(socketfd, (SOCKADDR*)&clientaddr, &len);
        if (clientfd == INVALID_SOCKET) {
            printf("接受连接失败\n");
            continue;
        }

        // 分配客户端 ID
        EnterCriticalSection(&cs);
        int client_id = next_client_id++;
        LeaveCriticalSection(&cs);

        printf("客户端连接: IP %s, 端口 %d，分配的 ID: %d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), client_id);

        // 创建客户端信息结构体
        ClientInfo* new_client = (ClientInfo*)malloc(sizeof(ClientInfo));
        new_client->socket = clientfd;
        new_client->id = client_id;

        // 将新客户端添加到列表
        EnterCriticalSection(&cs);
        if (client_count < MAX_CLIENTS) {
            client_sockets[client_count++] = *new_client;
        }
        else {
            printf("已达到最大客户端连接数，拒绝连接\n");
            closesocket(clientfd);
            free(new_client);
            LeaveCriticalSection(&cs);
            continue;
        }
        LeaveCriticalSection(&cs);

        // 为新客户端创建线程
        HANDLE hThread = CreateThread(NULL, 0, client_handler, (LPVOID)new_client, 0, NULL);
        if (hThread == NULL) {
            printf("创建线程失败\n");
            closesocket(clientfd);
        }
        else {
            CloseHandle(hThread);
        }
    }
    return 0;
}

// 玩家卡片赋值函数
void cards_assign(card* preset_cards, card* player_cards, enum player_type player) {
    int i;
    for (i = 0; i < 4; i++) {
        while (1) {
            int randv = rand() % 16;
            if (preset_cards[randv].player == default_player) {
                player_cards[i] = preset_cards[randv];
                player_cards[i].player = player;
                preset_cards[randv].player = player;
                break;
            }
        }
    }
}

// 初始化卡牌牌组函数
void card_init() {
    card preset_cards[16] = {
        {president,3,default_player},{prisoner,0,default_player},{accomplice,0,default_player},{alien,-1,default_player},
        {infected_person,0,default_player},{home_goer,0,default_player},{monitor,2,default_player},{top_student,2,default_player},
        {young_lady,1,default_player},{news,1,default_player},{book,1,default_player},{discipline,1,default_player},
        {health_care,1,default_player},{book,1,default_player},{news,1,default_player},{discipline,1,default_player}
    };
    cards_assign(preset_cards, player1_cards, player1);
    cards_assign(preset_cards, player2_cards, player2);
    cards_assign(preset_cards, player3_cards, player3);
    cards_assign(preset_cards, player4_cards, player4);
}

// 判断先手玩家函数
enum player_type first_player() {
    int i;
    for (i = 0; i < 4; i++)
    {
        if (player1_cards[i].type == president)
            return player1;
        else if (player2_cards[i].type == president)
            return player2;
        else if (player3_cards[i].type == president)
            return player3;
        else if (player4_cards[i].type == president)
            return player4;
    }
    return default_player;
}

// 初始化服务器
void InitializeServer() {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup 初始化失败\n");
        exit(1);
    }

    // 初始化临界区
    InitializeCriticalSection(&cs);
}

// 初始化函数
void init() {
    srand((unsigned)time(NULL));
    card_init();
    turn = first_player();
	InitializeServer();
}
// 创建监听套接字
SOCKET CreateListenSocket() {
    SOCKET socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("套接字创建失败\n");
        exit(1);
    }

    // 绑定地址和端口
    SOCKADDR_IN serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
    serveraddr.sin_port = htons(9999);
    if (bind(socketfd, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("绑定失败\n");
        exit(1);
    }

    // 开始监听
    if (listen(socketfd, 10) == SOCKET_ERROR) {
        printf("监听失败\n");
        exit(1);
    }

    printf("服务器正在监听 9999 端口...\n");
    return socketfd;
}
// 清理服务器资源
void CleanupServer(SOCKET socketfd) {
    DeleteCriticalSection(&cs);
    closesocket(socketfd);
    WSACleanup();
}

int main() {
    init();
    SOCKET socketfd = CreateListenSocket();
    // 创建接受客户端连接的线程
    HANDLE hAcceptThread = CreateThread(NULL, 0, accept_clients, &socketfd, 0, NULL);
    if (hAcceptThread == NULL) {
        printf("创建接受客户端连接线程失败\n");
        return 1;
    }
    // 等待接受客户端连接的线程结束
    WaitForSingleObject(hAcceptThread, INFINITE);
    CloseHandle(hAcceptThread);
    CleanupServer(socketfd);
    return 0;
}
