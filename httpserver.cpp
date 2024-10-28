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
    bool session = true;
    while (session)
    {
        int r = processHttpRequest(slave);
        if (r <= 0)
            session = false;
    }
    slave.close();
}

// Hàm xử lý yêu cầu HTTP
int HttpTcpServer::processHttpRequest(TcpSocket &slave)
{
    char buf[2048];
    int byte_recv;
    try
    {
        // Nhận dữ liệu từ client (request HTTP)
        byte_recv = slave.recv(buf, 2048);
        if (byte_recv > 0)
        {
            // Chuyển đổi buffer nhận được thành chuỗi (request HTTP)
            std::string request(buf, byte_recv);

            // In ra request để debug
            std::cout << "Received HTTP request:\n" << request << std::endl;

            // Tách dòng đầu tiên của request để phân tích cú pháp
            std::istringstream request_stream(request);
            std::string request_line;
            std::getline(request_stream, request_line);

            // Phân tích dòng đầu tiên để lấy phương thức, đường dẫn và phiên bản HTTP
            std::istringstream line_stream(request_line);
            std::string method, path, version;
            line_stream >> method >> path >> version;

            // Chuyển đổi method sang chữ hoa để dễ xử lý
            std::transform(method.begin(), method.end(), method.begin(), ::toupper);

            // Xử lý các phương thức HTTP khác nhau
//            if (method == "GET")
//            {
//                handleGetRequest(slave, path);
//            }
            if (method == "POST")
            {
                handlePostRequest(slave, request);
            }
            else if (method == "PUT")
            {
                handlePutRequest(slave, request);
            }
            else
            {
                handleUnknownRequest(slave);
            }
        }
    }
    catch (SocketException &e)
    {
        byte_recv = -1; // lỗi kết nối
        std::cerr << e.what() << " in processHttpRequest()" << std::endl;
    }
    return byte_recv;
}

// Xử lý yêu cầu GET
//void HttpTcpServer::handleGetRequest(TcpSocket &slave, const std::string &path)
//{
//    // Trả về response cho GET
//    std::string response =
//        "HTTP/1.1 200 OK\r\n"
//        "Content-Type: text/html\r\n"
//        "Content-Length: 13\r\n"
//        "\r\n"
//        "<h1>Hello</h1>";
//    slave.send(response.c_str(), response.size());
//}

// Xử lý yêu cầu POST
void HttpTcpServer::handlePostRequest(TcpSocket &slave, const std::string &request)
{
    // Giả sử phần body nằm sau phần header (tìm vị trí đầu tiên của "\r\n\r\n")
    size_t body_pos = request.find("\r\n\r\n");
    std::string body;
    if (body_pos != std::string::npos)
    {
        body = request.substr(body_pos + 4); // Lấy phần body sau \r\n\r\n
    }

    // In body để debug (nội dung gửi trong POST)
    std::cout << "POST Body:\n" << body << std::endl;

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
    // Tương tự như POST, lấy phần body của PUT
    size_t body_pos = request.find("\r\n\r\n");
    std::string body;
    if (body_pos != std::string::npos)
    {
        body = request.substr(body_pos + 4); // Lấy phần body sau \r\n\r\n
    }

    // In body để debug (nội dung gửi trong PUT)
    std::cout << "PUT Body:\n" << body << std::endl;

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
