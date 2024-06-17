package main

import (
	"bufio"
	"fmt"
	"net"
)

func main() {
	fmt.Println("Starting the server...")

	// 监听端口
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		fmt.Println("Error starting server:", err)
		return
	}
	defer listener.Close()

	fmt.Println("Server is listening on port 8080")

	for {
		// 接受客户端连接
		conn, err := listener.Accept()
		if err != nil {
			fmt.Println("Error accepting connection:", err)
			return
		}

		// 处理客户端连接
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	fmt.Println("Client connected")

	reader := bufio.NewReader(conn)
	for {
		message, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error reading message:", err)
			return
		}
		fmt.Printf("Received: %s", message)

		// Echo 回显消息给客户端
		_, err = conn.Write([]byte("Echo: " + message))
		if err != nil {
			fmt.Println("Error sending message:", err)
			return
		}
	}
}
