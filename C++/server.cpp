#include <iostream>
#include <winsock2.h> // 包含 Winsock2 库的头文件，用于网络编程。
#pragma comment(lib, "ws2_32.lib")  // 链接 ws2_32 库

/* 编译服务端和客户端代码
g++ -o server server.cpp -lws2_32
g++ -o client client.cpp -lws2_32
 */
const int PORT = 8080; // 定义服务器监听的端口号为 8080。
const int BUFFER_SIZE = 1024; // 定义缓冲区大小为 1024 字节。

int main() {
    WSADATA wsaData;
    // serverSocket 用于监听连接，clientSocket 用于与客户端通信。
    SOCKET serverSocket, clientSocket;
    // 定义两个 sockaddr_in 结构体变量，分别表示服务器地址和客户端地址。
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    // 字符数组，用作缓冲区
    char buffer[BUFFER_SIZE];

    // 初始化 Winsock 库，指定版本 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // 创建一个套接字，使用 IPv4 协议（AF_INET），流式套接字（SOCK_STREAM）。
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址结构
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 将套接字绑定到指定的端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 监听来自客户端的连接，最多允许 3 个连接请求排队。
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // 接受来自客户端的连接请求。
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    while (true) {
        // 清空缓冲区
        memset(buffer, 0, sizeof(buffer));
        // 接收来自客户端的消息，存储在缓冲区中
        int recvResult = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvResult <= 0) {
            // 如果接收结果为 0，表示连接已被客户端关闭
            if (recvResult == 0) {
                std::cerr << "Connection closed by client" << std::endl;
            } else {
                std::cerr << "Receive failed" << std::endl;
            }
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Received message from client: " << buffer << std::endl;

        // 将消息转换为大写
        for (int i = 0; buffer[i]; ++i)
            buffer[i] = std::toupper(buffer[i]);

        // 发送消息给客户端
        if (send(clientSocket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
            std::cerr << "Send failed" << std::endl;
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
    }

    // 关闭套接字
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
