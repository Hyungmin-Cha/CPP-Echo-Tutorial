#define _WINSOCK_DEPRECATED_NO_WARNINGS	// line 33, inet_addr()�� ����ϱ� ���ؼ� ������.
// deprecated: ���������� �ߴܵǾ�, ����� ������� �ʴ�
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {
	cout << "[���� �߻�]: " << message << endl;
	system("pause");
	exit(1);
}

int main(void) {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;		// ������ ������ ��Ʈ ��ȣ�� ����ؾ� �Ѵ�.
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// Winsock�� ��ȭ �Ѵ�.
		ShowErrorMessage("WSAStartup{}");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);	// TCP ������ �����Ѵ�.

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));	// 0���� �ʱ�ȭ
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");	// ���ڿ� IP�� ��Ʈ��ũ ����Ʈ ��������
	// 127.0.0.1 �ڱ� �ڽ��� �ǹ��ϴ� ��ӵ� IP�ּ�
	serverAddress.sin_port = htons(serverPort);				// 2����Ʈ ����(short)�� ��Ʈ��ũ ����Ʈ ��������
	// connect(���� ����, �����ּ� ����ü, �����ּ� ����ü Ū)
	// Ŭ���̾�Ʈ ������ ������ ������ ��û�Ѵ�. ���� ������ ������ ��� -1�� ��ȯ�Ѵ�.
	// IPv4 �����ּ� ����ü: �����ּ� ü��(sin_family), ��Ʈ ��ȣ(sin_port), IPv4 �ּ�(sin_addr)
	// ������ �����ּ� ����ü�� �̿��� ���� IPv4 �����ּ� ����ü�� �Ϲ� ���� ����ü(SOCKADDR)�� ��ȯ�Ͽ� ����ؾ� �Ѵ�.
	if (connect(clientSocket, (SOCKADDR*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[���� ����] connect()\n";

	// echo Ŭ���̾�Ʈ
	while (1) {		// �ݺ������� ������ �޼����� �����ϰ� ���� �޼����� ���޹޴´�.
		cout << "[�޼��� ����]: ";
		getline(cin, sent);
		if (sent == "") continue;	// ����ڰ� ���͸� �Է��� ��� �����ϰ� �����.
		// send(���� ����, ���ڿ� ����, ���ڿ� ���� ũ��, �÷���)
		// ���濡�� �����͸� ������. Ư���� �������� ���ڿ� ���ۿ� ��� ������ �����Ѵ�.
		// �÷��״� Ư���� ��찡 �ƴ϶�� 0�� �����Ѵ�.
		// ������ ���ۿ� �����ϴ� ��� -1�� ��ȯ�Ѵ�.
		send(clientSocket, sent.c_str(), sent.length(), 0);
		// recv(���� ����, ���ڿ� ����, ���ڿ� ���� ũ��, �÷���)
		// �����͸� �����Ͽ� Ư�� ���ڿ� ���ۿ� ��´�.
		// �÷��״� Ư���� ��찡 �ƴ϶�� 0�� �����Ѵ�.
		// ������ ���ſ� �����ϴ� ��� -1�� ��ȯ�Ѵ�.
		int length = recv(clientSocket, received, sizeof(received), 0);
		// ������ ���ڸ� NULL('\0')�� �ٲٸ�, ����ġ ���� ���ڰ� ������ �� �̰��� �߶� �� �ִ�.
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[Ŭ���̾�Ʈ ����]\n";
			break;
		}
		cout << "[���� �޼���]: " << received << endl;
	}
	closesocket(clientSocket);
	WSACleanup();

	system("pause");
	return 0;
}
