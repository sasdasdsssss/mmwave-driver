#ifndef DRIVER_H
#define DRIVER_H

#include <iostream>
#include <vector>
#include <windows.h>
#include<WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <pcap.h>

#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

class DataHandler
{

private:
    std::vector<std::string> GetPhysicalInterfaces();
    bool OpenSelectedInterface();


public:

    //Run
    void SoftRun();

    //DealData
    void DealDataFunction();
    double DealData1;
    double DealData2;

    int  RxDataCrc(unsigned char* pBuffer, unsigned int u16DataLen);
    int  DealDataAnalyze(float* pBuffer, unsigned int u16DataLen);


    //Network_Capture

    bool NetworkCaptureFunction();

    const u_char* pkt_data; //存储捕获的数据包
    struct pcap_pkthdr* header1;
    unsigned char TimeData[24576] = { 0 };

    //DataTransmission

    void DataTransmissionFunction(const u_char radarData[1024]);

};


#endif // DRIVER_H