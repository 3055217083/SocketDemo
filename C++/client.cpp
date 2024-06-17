#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // 链接 ws2_32 库

const char *SERVER_IP = "127.0.0.1";
const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    // 定义一个套接字变量，用于与服务器通信。
    SOCKET clientSocket;
    // 定义一个 sockaddr_in 结构体变量，表示服务器地址。
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // 初始化 Winsock 库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // 创建套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址结构
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 尝试连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    while (true) {
        std::cout << "Enter a message: ";
        std::cin.getline(buffer, BUFFER_SIZE);

        // 发送消息给服务器
        if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed" << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // 接收服务器的响应
        memset(buffer, 0, BUFFER_SIZE);
        int recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recvResult <= 0) {
            if (recvResult == 0) {
                std::cerr << "Connection closed by server" << std::endl;
            } else {
                std::cerr << "Receive failed" << std::endl;
            }
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Server response: " << buffer << std::endl;
    }

    // 关闭套接字
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
