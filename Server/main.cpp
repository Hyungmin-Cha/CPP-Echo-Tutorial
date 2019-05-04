#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {			// 에러 핸들링 함수는 필수이다.
	cout << "[오류 발생]: " << message << endl;
	system("pause");
	exit(1);
}

int main(void) {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;
	// IPv4 소켓 주소 구조체: 소켓 주소체계(sin_family), 포트(sin_port), IPv4 주소(sin_addr)

	int serverPort = 9876;
	char received[256];

	// WSAStartup(윈속 버전, WSA 데이터 변수)
	// 사용할 Winsock의 버전을 설정한다.
	// 관련 설정 정보를 받기 위해 WSADATA 변수를 함께 넣어준다.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// Winsock을 초기화합니다.
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// TCP 소켓을 생성합니다.
	// PF_INET->IPv4, SOCK_STREAM->TCP

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");
	memset(&serverAddress, 0, sizeof(serverAddress));	// 전부 0의 값으로 초기화
	serverAddress.sin_family = AF_INET;					// IPv4(AF_INET), IPv6(AF_INET6)
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// 4바이트 정수(long)를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort);			// 2바이트 정수(short)를 네트워크 바이트 형식으로
	// bind(소켓 변수, 서버주소 구조체, 서버주소 구조체의 크기)
	// 서버측의 소켓에 IP와 포트 번호를 할당하여 네크워크 인터페이스와 묶일 수 있도록 한다.
	// 실제로 소켓주소 구조체를 이용할 때는 IPv4 소켓 주소 구조체를 일반 소켓 구조체(SOCKADDR)로 변환하여 사용해야 한다.
	if (bind(serverSocket, (SOCKADDR*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	// listen(소켓 변수, 백 로그 큐의 크기)
	// 클라이언트로부터 연결 요청을 기다린다. 백 로그란 동시에 연결을 시도하는 최대 클라이언트 수를 의미한다.
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	// accept(소켓변수, 클라이언트 주소 구조체 변수, 클라이언트 주소 구조체 크기)
	// 서버 측에서 클라이언트의 연결을 수락한다.(오류 발생시 -1 반환)
	// accept()함수 내부에서 클라이언트 주소를 설정한 뒤에 통신에 사용할 클라이언트의 소켓을 반환한다.
	// 실제로 소켓주소 구조체를  이용할 때는 IPv4 소켓 주소 구조체를 일반 소켓 구조체(SOCKADDR)로 변환하여 사용해야 한다.
	clientSocket = accept(serverSocket, (SOCKADDR*)& clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) {		// 클라이언트의 메세지를 받아서 그대로 전달한다. (echo 프로그램)
		// recv(소켓 변수, 문자열 버퍼, 문자열 버퍼 크기, 플래그)
		// 데이터를 수신하여 특정 문자열 버퍼에 담는다.
		// 플래그는 특별한 경우가 아니라면 0을 설정한다.
		// 데이터 수신에 실패하는 경우 -1을 반환한다.
		int length = recv(clientSocket, received, sizeof(received), 0);
		// 마지막 문자를 NULL로 바꾸면, 예기치 못한 문자가 들어왔을 때 이것을 잘라낼 수 있다.
		received[length] = NULL;	// '\0'
		cout << "[클라이언트 메세지]: " << received << endl;
		cout << "[메세지 전송]: " << received << endl;
		if (strcmp(received, "[exit]") == 0) {
			// 메세지를 전송할 때 엔터로 인해 입력된 마지막 공백문자를 제거하기 위해 1을 뺀다.
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break;
		}
		// send(소켓 변수, 문자열 버퍼, 문자열 버퍼 크기, 플래그)
		// 상대방에게 데이터를 보낸다. 특정한 소켓으로 문자열 버퍼에 담긴 내용을 전송한다.
		// 플래그는 특별한 경우가 아니라면 0을 설정한다.
		// 데이터 전송에 실패하는 경우 -1을 반환한다.
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	// closesocket(소켓 변수)
	// 열린 소켓을 닫는다.
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();	// Winsock을 더이상 사용하지 않음을 설정

	system("pause");
	return 0;

}
