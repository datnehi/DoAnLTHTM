#include "httpserver.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

HttpTcpServer::HttpTcpServer(unsigned short port) : TCPServer(port)
{
}

HttpTcpServer::~HttpTcpServer()
{
}

void HttpTcpServer::startNewSession(TcpSocket slave)
{
    std::cout << "Client connect ok." << std::endl;
    connectRequest(slave);
    bool session = true;
    while (session)
    {
        int r = processHttpRequest(slave);
        if (r <= 0) session = false;
    }
    std::cout << "Client disconnect." << std::endl;
    slave.close();
}

// Hàm xử lý yêu cầu HTTP
int HttpTcpServer::processHttpRequest(TcpSocket &slave)
{
    std::string headers;
    char buf[2048];
    int byte_recv;

    try
    {
        // Đọc toàn bộ phần header
        while (true)
        {
            byte_recv = slave.recvLine(buf, 2048);
            if (byte_recv < 0)
                return -1; // Lỗi hoặc kết nối bị đóng

            std::string line(buf, byte_recv);
            headers += line;

            // Kiểm tra kết thúc header
            if (headers.find("\r\n\r\n") != std::string::npos)
                break;
        }

        // In ra header để debug
        std::cout << "Received HTTP headers:\n" << headers << std::endl;

        // Đọc phần body cho đến khi gặp chuỗi kết thúc "\r\n\r\n" (hoặc một delimiter khác)
        std::string body;
        size_t delimiter_pos = headers.find("\r\n\r\n");
        if (delimiter_pos != std::string::npos)
        {
            // Tiến hành đọc body cho đến khi gặp delimiter
            char* body_buf = new char[2048];  // Đặt kích thước buffer phù hợp với dữ liệu
            while (true)
            {
                byte_recv = slave.recv(body_buf, 2048);
                if (byte_recv < 0)
                    break;  // Lỗi hoặc kết nối bị đóng

                body.append(body_buf, byte_recv);

                // Kiểm tra nếu body đã chứa delimiter
                if (body.find("\r\n\r\n") != std::string::npos)
                    break;
            }

            std::cout << "Received HTTP body:\n" << body << std::endl;

            delete[] body_buf;
        }

        // Xử lý request dựa trên method
        std::istringstream request_stream(headers);
        std::string request_line;
        std::getline(request_stream, request_line);

        std::istringstream line_stream(request_line);
        std::string method, path, version;
        line_stream >> method >> path >> version;

        // Chuyển method sang chữ hoa
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);

        if (method == "POST")
        {
            handlePostRequest(slave, body);
        }
        else if (method == "PUT")
        {
            handlePutRequest(slave, body);
        }
        else
        {
            handleUnknownRequest(slave);
        }
    }
    catch (SocketException &e)
    {
        byte_recv = -1; // lỗi kết nối
        std::cerr << e.what() << " in processHttpRequest()" << std::endl;
    }
    return byte_recv;
}




// Xử lý yêu cầu POST
void HttpTcpServer::handlePostRequest(TcpSocket &slave, const std::string &request)
{
    // In body để debug (nội dung gửi trong POST)
    std::cout << "POST Body:\n" << request << std::endl;

    // Trả về response cho POST
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 14\r\n"
        "\r\n"
        "Post received!\r\n";
    slave.send(response.c_str(), response.size());
}

// Xử lý yêu cầu PUT
void HttpTcpServer::handlePutRequest(TcpSocket &slave, const std::string &request)
{
    // In body để debug (nội dung gửi trong PUT)
    std::cout << "PUT Body:\n" << request << std::endl;

    // Trả về response cho PUT
    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Put received!\r\n";
    slave.send(response.c_str(), response.size());
}

// Xử lý các yêu cầu không xác định
void HttpTcpServer::handleUnknownRequest(TcpSocket &slave)
{
    std::string response =
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 23\r\n"
        "\r\n"
        "Method not supported!\r\n";
    slave.send(response.c_str(), response.size());
}

void HttpTcpServer::connectRequest(TcpSocket &slave)
{
    std::string response = "Connect server complete.\r\n";
    slave.send(response.c_str(), response.size());
}
