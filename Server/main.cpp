#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) {			// ���� �ڵ鸵 �Լ��� �ʼ��̴�.
	cout << "[���� �߻�]: " << message << endl;
	system("pause");
	exit(1);
}

int main(void) {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;
	// IPv4 ���� �ּ� ����ü: ���� �ּ�ü��(sin_family), ��Ʈ(sin_port), IPv4 �ּ�(sin_addr)

	int serverPort = 9876;
	char received[256];

	// WSAStartup(���� ����, WSA ������ ����)
	// ����� Winsock�� ������ �����Ѵ�.
	// ���� ���� ������ �ޱ� ���� WSADATA ������ �Բ� �־��ش�.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// Winsock�� �ʱ�ȭ�մϴ�.
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// TCP ������ �����մϴ�.
	// PF_INET->IPv4, SOCK_STREAM->TCP

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");
	memset(&serverAddress, 0, sizeof(serverAddress));	// ���� 0�� ������ �ʱ�ȭ
	serverAddress.sin_family = AF_INET;					// IPv4(AF_INET), IPv6(AF_INET6)
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// 4����Ʈ ����(long)�� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort);			// 2����Ʈ ����(short)�� ��Ʈ��ũ ����Ʈ ��������
	// bind(���� ����, �����ּ� ����ü, �����ּ� ����ü�� ũ��)
	// �������� ���Ͽ� IP�� ��Ʈ ��ȣ�� �Ҵ��Ͽ� ��ũ��ũ �������̽��� ���� �� �ֵ��� �Ѵ�.
	// ������ �����ּ� ����ü�� �̿��� ���� IPv4 ���� �ּ� ����ü�� �Ϲ� ���� ����ü(SOCKADDR)�� ��ȯ�Ͽ� ����ؾ� �Ѵ�.
	if (bind(serverSocket, (SOCKADDR*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	// listen(���� ����, �� �α� ť�� ũ��)
	// Ŭ���̾�Ʈ�κ��� ���� ��û�� ��ٸ���. �� �α׶� ���ÿ� ������ �õ��ϴ� �ִ� Ŭ���̾�Ʈ ���� �ǹ��Ѵ�.
	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	// accept(���Ϻ���, Ŭ���̾�Ʈ �ּ� ����ü ����, Ŭ���̾�Ʈ �ּ� ����ü ũ��)
	// ���� ������ Ŭ���̾�Ʈ�� ������ �����Ѵ�.(���� �߻��� -1 ��ȯ)
	// accept()�Լ� ���ο��� Ŭ���̾�Ʈ �ּҸ� ������ �ڿ� ��ſ� ����� Ŭ���̾�Ʈ�� ������ ��ȯ�Ѵ�.
	// ������ �����ּ� ����ü��  �̿��� ���� IPv4 ���� �ּ� ����ü�� �Ϲ� ���� ����ü(SOCKADDR)�� ��ȯ�Ͽ� ����ؾ� �Ѵ�.
	clientSocket = accept(serverSocket, (SOCKADDR*)& clientAddress, &sizeClientAddress);
	cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) {		// Ŭ���̾�Ʈ�� �޼����� �޾Ƽ� �״�� �����Ѵ�. (echo ���α׷�)
		// recv(���� ����, ���ڿ� ����, ���ڿ� ���� ũ��, �÷���)
		// �����͸� �����Ͽ� Ư�� ���ڿ� ���ۿ� ��´�.
		// �÷��״� Ư���� ��찡 �ƴ϶�� 0�� �����Ѵ�.
		// ������ ���ſ� �����ϴ� ��� -1�� ��ȯ�Ѵ�.
		int length = recv(clientSocket, received, sizeof(received), 0);
		// ������ ���ڸ� NULL�� �ٲٸ�, ����ġ ���� ���ڰ� ������ �� �̰��� �߶� �� �ִ�.
		received[length] = NULL;	// '\0'
		cout << "[Ŭ���̾�Ʈ �޼���]: " << received << endl;
		cout << "[�޼��� ����]: " << received << endl;
		if (strcmp(received, "[exit]") == 0) {
			// �޼����� ������ �� ���ͷ� ���� �Էµ� ������ ���鹮�ڸ� �����ϱ� ���� 1�� ����.
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[���� ����]\n";
			break;
		}
		// send(���� ����, ���ڿ� ����, ���ڿ� ���� ũ��, �÷���)
		// ���濡�� �����͸� ������. Ư���� �������� ���ڿ� ���ۿ� ��� ������ �����Ѵ�.
		// �÷��״� Ư���� ��찡 �ƴ϶�� 0�� �����Ѵ�.
		// ������ ���ۿ� �����ϴ� ��� -1�� ��ȯ�Ѵ�.
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	// closesocket(���� ����)
	// ���� ������ �ݴ´�.
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();	// Winsock�� ���̻� ������� ������ ����

	system("pause");
	return 0;

}
