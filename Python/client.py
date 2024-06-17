import socket

def main():
    # 创建套接字对象
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # 连接服务器
    client_socket.connect(('127.0.0.1', 8080))
    
    while True:
        # 从用户输入读取消息
        message = input("Enter message: ")

        # 发送消息给服务器
        client_socket.sendall(message.encode())

        # 接收服务器的响应
        data = client_socket.recv(1024)
        print(f"Response from server: {data.decode()}")

if __name__ == "__main__":
    main()
