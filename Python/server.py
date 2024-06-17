import socket

def main():
    # 创建套接字对象
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # 绑定主机地址和端口
    server_socket.bind(('127.0.0.1', 8080))
    
    # 监听连接
    server_socket.listen(5)
    print("Server is listening on port 8080...")

    while True:
        # 接受客户端连接
        client_socket, address = server_socket.accept()
        print(f"Connection from {address} has been established.")

        # 处理客户端请求
        while True:
            data = client_socket.recv(1024)
            if not data:
                break
            print(f"Received: {data.decode()}")

            # 回送消息给客户端
            client_socket.sendall(data)

        # 关闭客户端连接
        client_socket.close()

if __name__ == "__main__":
    main()
