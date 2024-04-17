//tcp 고정길이 + 가변길이 전송 클라이언트
#include "Common.h"

char *SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    50

int main(int argc, char* argv[]) {
	int retval;

    //명령행 인수가 있다면
    if(argc>1) SERVERIP = argv[1];

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

    //connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	//데이터 통신에 사용할 변수
	const char* testdata[]={
        "hi",
        "nice to meet you",
        "today~",
        "me, too",
    };
	char buf[BUFSIZE];
    int len;

    //서버와 통신
    for(int i=0; i<4; i++){
        len = (int)strlen(testdata[i]);
        strncpy(buf, testdata[i], len);
        
        //고정길이 데이터 보내기
        retval = send(sock, (char*)&len, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        //가변길이 데이터 보내기
        retval = send(sock, buf, len, 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("TCP client: send %d+%dbyte.\n", (int)sizeof(int), retval);
    }

	//소켓 닫기
	closesocket(sock);

	//윈속 종료
	WSACleanup();
	return 0;
}