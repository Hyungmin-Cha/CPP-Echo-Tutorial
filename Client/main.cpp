#define _WINSOCK_DEPRECATED_NO_WARNINGS	// line 33, inet_addr()를 사용하기 위해서 선언함.
// deprecated: 유지보수가 중단되어, 사용이 권장되지 않는
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[오류 발생]: " << message << endl;
	system("pause");
	exit(1);
}

int main(void) {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;		// 서버와 동일한 포트 번호를 사용해야 한다.
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// Winsock을 최화 한다.
		ShowErrorMessage("WSAStartup{}");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);	// TCP 소켓을 생성한다.

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));	// 0으로 초기화
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");	// 문자열 IP를 네트워크 바이트 형식으로
	// 127.0.0.1 자기 자신을 의미하는 약속된 IP주소
	serverAddress.sin_port = htons(serverPort);				// 2바이트 정수(short)를 네트워크 바이트 형식으로
	// connect(소켓 변수, 서버주소 구조체, 서버주소 구조체 큭)
	// 클라이언트 측에서 서버에 연결을 요청한다. 소켓 생성에 실패할 경우 -1을 반환한다.
	// IPv4 소켓주소 구조체: 소켓주소 체계(sin_family), 포트 번호(sin_port), IPv4 주소(sin_addr)
	// 실제로 소켓주소 구조체를 이용할 때는 IPv4 소켓주소 구조체를 일반 소켓 구조체(SOCKADDR)로 변환하여 사용해야 한다.
	if (connect(clientSocket, (SOCKADDR*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[현재 상태] connect()\n";

	// echo 클라이언트
	while (1) {		// 반복적으로 서버로 메세지를 전송하고 에코 메세지를 전달받는다.
		cout << "[메세지 전송]: ";
		getline(cin, sent);
		if (sent == "") continue;	// 사용자가 엔터만 입력한 경우 무시하게 만든다.
		// send(소켓 변수, 문자열 버퍼, 문자열 버퍼 크기, 플래그)
		// 상대방에게 데이터를 보낸다. 특정한 소켓으로 문자열 버퍼에 담긴 내용을 전송한다.
		// 플래그는 특별한 경우가 아니라면 0을 설정한다.
		// 데이터 전송에 실패하는 경우 -1을 반환한다.
		send(clientSocket, sent.c_str(), sent.length(), 0);
		// recv(소켓 변수, 문자열 버퍼, 문자열 버퍼 크기, 플래그)
		// 데이터를 수신하여 특정 문자열 버퍼에 담는다.
		// 플래그는 특별한 경우가 아니라면 0을 설정한다.
		// 데이터 수신에 실패하는 경우 -1을 반환한다.
		int length = recv(clientSocket, received, sizeof(received), 0);
		// 마지막 문자를 NULL('\0')로 바꾸면, 예기치 못한 문자가 들어왔을 때 이것을 잘라낼 수 있다.
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[클라이언트 종료]\n";
			break;
		}
		cout << "[서버 메세지]: " << received << endl;
	}
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}
