#include <iostream>
#include "Driver.h"
#include <Winsock2.h>
#include <ws2tcpip.h>

#pragma once

#pragma comment(lib, "ws2_32.lib") // 链接 ws2_32.lib 库

void DataHandler::DataTransmissionFunction(const u_char radarData[16640]) {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error: Failed to initialize Winsock." << std::endl;
    }

    // 设置服务器的IP地址和端口
    //const wchar_t* serverIP = L"192.168.31.183"; // 使用宽字符字符串表示服务器IP地址
    //const int serverPort = 56786;              // 服务器端口号

    const wchar_t* serverIP = L"127.0.0.1"; // 使用宽字符字符串表示服务器IP地址
    const int serverPort = 8080;              // 服务器端口号

    // 创建套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error: Unable to create socket." << std::endl;
        WSACleanup();
    }

    // 设置服务器的地址信息
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    // 使用 InetPtonW 函数将宽字符IP地址转换为二进制
    if (InetPtonW(AF_INET, serverIP, &serverAddress.sin_addr) == 0) {
        std::cerr << "Error: Invalid server IP address." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
    }
    else if (InetPtonW(AF_INET, serverIP, &serverAddress.sin_addr) == -1) {
        std::cerr << "Error: Failed to convert server IP address." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
    }

    int maxRetries = 5; // 最大连接尝试次数
    int retryCount = 0;

    while (retryCount < maxRetries) {
        // 尝试连接到服务器
        if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Error: Unable to connect to the server. Retry #" << retryCount + 1 << std::endl;
            retryCount++;
            Sleep(5000); // 等待一段时间后重试（单位：毫秒）
        }
        else {
            std::cout << "Connected to the server." << std::endl;
            break; // 连接成功，退出循环
        }
    }

    if (retryCount == maxRetries) {
        std::cerr << "Error: Maximum number of connection retries reached." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
    }


        //模拟的雷达数据
       /* for (int i = 0; i < sizeof(radarData); i++) {
            radarData[i] = 'i';
        }*/


        // 发送雷达数据到服务器
        int bytesSent = send(clientSocket, reinterpret_cast<const char*>(radarData), sizeof(u_char) * 16640, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Error: Failed to send radar data." << std::endl;
        }
        else {
            std::cout << "Sent " << bytesSent << " bytes of radar data to the server." << std::endl;
        }

    // 关闭套接字和清理Winsock
    closesocket(clientSocket);
    WSACleanup();

}




