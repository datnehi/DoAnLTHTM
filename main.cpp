#include <iostream>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 1708

void handle_client(SOCKET client_socket) {
    std::string welcome_message = "Ket noi thanh cong.\r\n Hay nhap:\r\n 1-De Post\r\n 2-De Put\r\n";
    send(client_socket, welcome_message.c_str(), welcome_message.length(), 0);

    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);

    std::string request(buffer);
    std::string response;

    if (request.find("1") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST request received.";
    } else if (request.find("2") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPUT request received.";
    } else {
        response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/plain\r\n\r\nServer không hỗ trợ phương thức này.";
    }

    send(client_socket, response.c_str(), response.length(), 0);
    closesocket(client_socket);
}

void start_server() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Khởi tạo Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Tạo socket server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Cấu hình địa chỉ server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Gán địa chỉ cho socket server
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));

    // Lắng nghe kết nối
    listen(server_socket, 3);

    std::cout << "Server is running on port " << PORT << std::endl;

    while (true) {
        // Chấp nhận kết nối từ client
        client_socket = accept(server_socket, (struct sockaddr *)&address, &addrlen);

        // Xử lý client kết nối (không dùng thread, xử lý tuần tự)
        if (client_socket != INVALID_SOCKET) {
            handle_client(client_socket);
        }
    }

    // Đóng socket server
    closesocket(server_socket);
    WSACleanup();
}

int main() {
    start_server();
    return 0;
}
