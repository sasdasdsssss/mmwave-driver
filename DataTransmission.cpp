#include <iostream>
#include "Driver.h"
#include <Winsock2.h>
#include <ws2tcpip.h>

#pragma once

#pragma comment(lib, "ws2_32.lib") // ���� ws2_32.lib ��

void DataHandler::DataTransmissionFunction(const u_char radarData[16640]) {
    // ��ʼ��Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error: Failed to initialize Winsock." << std::endl;
    }

    // ���÷�������IP��ַ�Ͷ˿�
    //const wchar_t* serverIP = L"192.168.31.183"; // ʹ�ÿ��ַ��ַ�����ʾ������IP��ַ
    //const int serverPort = 56786;              // �������˿ں�

    const wchar_t* serverIP = L"127.0.0.1"; // ʹ�ÿ��ַ��ַ�����ʾ������IP��ַ
    const int serverPort = 8080;              // �������˿ں�

    // �����׽���
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error: Unable to create socket." << std::endl;
        WSACleanup();
    }

    // ���÷������ĵ�ַ��Ϣ
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    // ʹ�� InetPtonW ���������ַ�IP��ַת��Ϊ������
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

    int maxRetries = 5; // ������ӳ��Դ���
    int retryCount = 0;

    while (retryCount < maxRetries) {
        // �������ӵ�������
        if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Error: Unable to connect to the server. Retry #" << retryCount + 1 << std::endl;
            retryCount++;
            Sleep(5000); // �ȴ�һ��ʱ������ԣ���λ�����룩
        }
        else {
            std::cout << "Connected to the server." << std::endl;
            break; // ���ӳɹ����˳�ѭ��
        }
    }

    if (retryCount == maxRetries) {
        std::cerr << "Error: Maximum number of connection retries reached." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
    }


        //ģ����״�����
       /* for (int i = 0; i < sizeof(radarData); i++) {
            radarData[i] = 'i';
        }*/


        // �����״����ݵ�������
        int bytesSent = send(clientSocket, reinterpret_cast<const char*>(radarData), sizeof(u_char) * 16640, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Error: Failed to send radar data." << std::endl;
        }
        else {
            std::cout << "Sent " << bytesSent << " bytes of radar data to the server." << std::endl;
        }

    // �ر��׽��ֺ�����Winsock
    closesocket(clientSocket);
    WSACleanup();

}




