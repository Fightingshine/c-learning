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
    int id; // �ͻ��� ID
} ClientInfo;
#pragma pack(pop)

ClientInfo client_sockets[MAX_CLIENTS];
int client_count = 0;
int next_client_id = 1; // ���ڷ���ͻ��� ID
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

// ������Ϸ״̬�ĺ���
int SendGameState(SOCKET sock)
{
    // ���������ݴ�С
    int total_size = sizeof(player1_cards) + sizeof(player2_cards)
        + sizeof(player3_cards) + sizeof(player4_cards)
        + sizeof(player1_doubt_list) + sizeof(player2_doubt_list)
        + sizeof(player3_doubt_list) + sizeof(player4_doubt_list)
        + sizeof(preservation) + sizeof(turn);

    char* buffer = (char*)malloc(total_size);
    char* ptr = buffer;

    // �����ݸ��Ƶ�������
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

    // ��������
    int sendlen = send(sock, buffer, total_size, 0);
    if (sendlen == SOCKET_ERROR)
    {
        printf("���ݷ���ʧ��\n");
        free(buffer);
        return -1; // ���ش���
    }
    else
    {
        printf("���ݷ��ͳɹ��������ֽ�����%d\n", sendlen);
    }

    free(buffer);
    return 0; // ���سɹ�
}

// ������Ϸ״̬�ĺ���
int RecvGameState(SOCKET sock)
{
    // ���������ݴ�С
    int total_size = sizeof(player1_cards) + sizeof(player2_cards)
        + sizeof(player3_cards) + sizeof(player4_cards)
        + sizeof(player1_doubt_list) + sizeof(player2_doubt_list)
        + sizeof(player3_doubt_list) + sizeof(player4_doubt_list)
        + sizeof(preservation) + sizeof(turn);
    char* buffer = (char*)malloc(total_size);
    char* ptr = buffer;
    // ��������
    int recvlen = recv(sock, buffer, total_size, 0);
    if (recvlen <= 0) // ���մ�������ӹر�
    {
        printf("���ݽ���ʧ��\n");
        free(buffer);
        return -1; // ���ش���
    }
    else
    {
        printf("���ݽ��ճɹ��������ֽ�����%d\n", recvlen);
    }
    // �����ݸ��Ƶ�����
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
    return 0; // ���سɹ�
}

// ��Ϸ�����жϺ���
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

// �ͻ��˴�����
DWORD WINAPI client_handler(LPVOID lpParameter) {
    ClientInfo* client_info = (ClientInfo*)lpParameter;
    SOCKET client_socket = client_info->socket;
    int client_id = client_info->id;

    // ֪ͨ�ͻ��������� ID
    char id_msg[256];
    sprintf(id_msg, "���Ŀͻ��� ID ��: %d\n", client_id);
    send(client_socket, id_msg, strlen(id_msg), 0);
    // ������Ϸ״̬
    if (SendGameState(client_socket) < 0) {
        printf("������Ϸ״̬ʧ�ܣ��ͻ��� %d �ѶϿ�����\n", client_id);
        goto cleanup;
    }

    while (1) {
        // ������Ϸ״̬
        if (RecvGameState(client_socket) < 0) {
            printf("������Ϸ״̬ʧ�ܣ��ͻ��� %d �ѶϿ�����\n", client_id);
            break;
        }
        // ��Ϸ�����ж�
        game_over();
        // �����пͻ��˷�����Ϸ״̬
        EnterCriticalSection(&cs);
        int i;
        for (i = 0; i < client_count; i++) {
            if (SendGameState(client_sockets[i].socket) < 0) {
                printf("�������ݵ��ͻ��� %d ʧ��\n", client_sockets[i].id);
            }
        }
        LeaveCriticalSection(&cs);
    }

cleanup:
    // �ͻ��˶Ͽ����ӣ��Ƴ��׽���
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

// ���ܿͻ������ӵ��̺߳���
DWORD WINAPI accept_clients(LPVOID lpParameter) {
    SOCKET socketfd = *(SOCKET*)lpParameter;
    while (1) {
        // ���ܿͻ�������
        SOCKADDR_IN clientaddr;
        int len = sizeof(clientaddr);
        SOCKET clientfd = accept(socketfd, (SOCKADDR*)&clientaddr, &len);
        if (clientfd == INVALID_SOCKET) {
            printf("��������ʧ��\n");
            continue;
        }

        // ����ͻ��� ID
        EnterCriticalSection(&cs);
        int client_id = next_client_id++;
        LeaveCriticalSection(&cs);

        printf("�ͻ�������: IP %s, �˿� %d������� ID: %d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), client_id);

        // �����ͻ�����Ϣ�ṹ��
        ClientInfo* new_client = (ClientInfo*)malloc(sizeof(ClientInfo));
        new_client->socket = clientfd;
        new_client->id = client_id;

        // ���¿ͻ�����ӵ��б�
        EnterCriticalSection(&cs);
        if (client_count < MAX_CLIENTS) {
            client_sockets[client_count++] = *new_client;
        }
        else {
            printf("�Ѵﵽ���ͻ������������ܾ�����\n");
            closesocket(clientfd);
            free(new_client);
            LeaveCriticalSection(&cs);
            continue;
        }
        LeaveCriticalSection(&cs);

        // Ϊ�¿ͻ��˴����߳�
        HANDLE hThread = CreateThread(NULL, 0, client_handler, (LPVOID)new_client, 0, NULL);
        if (hThread == NULL) {
            printf("�����߳�ʧ��\n");
            closesocket(clientfd);
        }
        else {
            CloseHandle(hThread);
        }
    }
    return 0;
}

// ��ҿ�Ƭ��ֵ����
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

// ��ʼ���������麯��
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

// �ж�������Һ���
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

// ��ʼ��������
void InitializeServer() {
    // ��ʼ�� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup ��ʼ��ʧ��\n");
        exit(1);
    }

    // ��ʼ���ٽ���
    InitializeCriticalSection(&cs);
}

// ��ʼ������
void init() {
    srand((unsigned)time(NULL));
    card_init();
    turn = first_player();
	InitializeServer();
}
// ���������׽���
SOCKET CreateListenSocket() {
    SOCKET socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("�׽��ִ���ʧ��\n");
        exit(1);
    }

    // �󶨵�ַ�Ͷ˿�
    SOCKADDR_IN serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
    serveraddr.sin_port = htons(9999);
    if (bind(socketfd, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("��ʧ��\n");
        exit(1);
    }

    // ��ʼ����
    if (listen(socketfd, 10) == SOCKET_ERROR) {
        printf("����ʧ��\n");
        exit(1);
    }

    printf("���������ڼ��� 9999 �˿�...\n");
    return socketfd;
}
// �����������Դ
void CleanupServer(SOCKET socketfd) {
    DeleteCriticalSection(&cs);
    closesocket(socketfd);
    WSACleanup();
}

int main() {
    init();
    SOCKET socketfd = CreateListenSocket();
    // �������ܿͻ������ӵ��߳�
    HANDLE hAcceptThread = CreateThread(NULL, 0, accept_clients, &socketfd, 0, NULL);
    if (hAcceptThread == NULL) {
        printf("�������ܿͻ��������߳�ʧ��\n");
        return 1;
    }
    // �ȴ����ܿͻ������ӵ��߳̽���
    WaitForSingleObject(hAcceptThread, INFINITE);
    CloseHandle(hAcceptThread);
    CleanupServer(socketfd);
    return 0;
}
