#include <iostream>
#include <pcap.h>
#include "Driver.h"
#include <vector>
#include <string>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

#define MAX_PACKETS 1000 // 设置最大捕获的数据包数量


pcap_if_t* d;
int i = 0;
pcap_if_t* alldevs;		//网卡接口信息
char errbuf[PCAP_ERRBUF_SIZE];
int selectedInterfaceNumber;


std::vector<std::string> DataHandler::GetPhysicalInterfaces()
{
    std::vector<std::string> physicalInterfaces;
    ULONG bufLen = sizeof(IP_ADAPTER_ADDRESSES);
    PIP_ADAPTER_ADDRESSES adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);

    if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses, &bufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(adapterAddresses);
        adapterAddresses = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);
    }

    if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses, &bufLen) == NO_ERROR)
    {
        int interfaceNumber = 1; // 用于标识物理接口的序号

        for (PIP_ADAPTER_ADDRESSES adapter = adapterAddresses; adapter != nullptr; adapter = adapter->Next)
        {
            if (adapter->OperStatus == IfOperStatusUp && adapter->PhysicalAddressLength > 0)
            {
                physicalInterfaces.push_back(adapter->AdapterName);
                interfaceNumber++;
            }
        }
    }

    free(adapterAddresses);

    return physicalInterfaces;
}

//过滤器：只获取该地址里的数据包    arp -a命令来查看
char filter[] = "ether host ff:ff:ff:ff:ff:ff or host 169.254.14.62";
//char* filter = nullptr; // 不设置过滤器，捕获所有数据包

bpf_u_int32 NetMask;
struct bpf_program fcode;
pcap_t* handle;

bool DataHandler::OpenSelectedInterface()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    //pcap_t* handle = pcap_open_live(selectedInterface.c_str(), BUFSIZ, 1, 1000, errbuf);
    
    char source[] = "rpcap://";
    pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
    
    //for (d = alldevs, i = 0; i < selectedInterfaceNumber ; d = d->next, i++);
    
    //找到描述为Realtek PCIe GbE Family Controller的接口，到时候可以查看具体是哪个修改
    for (d = alldevs; d != NULL; d = d->next) {
        if (strstr(d->description, "Realtek PCIe GbE Family Controller") != NULL) {
            break; // 找到匹配的接口，退出循环
        }
    }


     if((handle = pcap_open(d->name, //设备名
        65536,         //捕获数据的最大字节数
        1,     // 1混杂模式,0为正常模式
        1000,         //读响应时间
        NULL,
        errbuf         //如果错误存放的位置
    )) == NULL)
     {
         std::cout << "打开设备失败！" << std::endl;
         pcap_freealldevs(alldevs);	//释放设备
         return false;
     }

     if (filter != NULL)
     {
         NetMask = 0xffffff;

         if (pcap_compile(handle, &fcode, filter, 1, NetMask) < 0)
         {      
             std::cout << "编译错误！" << std::endl;
             std::cout << "错误消息: " << pcap_geterr(handle) << std::endl; // 打印错误消息
             pcap_close(handle);
             return false;
         }

         if (pcap_setfilter(handle, &fcode) < 0)
         {
             std::cout << "设置错误！" << std::endl;
             std::cout << "错误消息: " << pcap_geterr(handle) << std::endl; // 打印错误消息
             pcap_close(handle);
             return false;
         }
     }

        std::cout << "Successfully opened interface: " <<std::endl;

        //struct pcap_pkthdr header;
        //const u_char* packet;
       
        //while ((packet = pcap_next(handle, &header)) != nullptr)
        //{
        //    packetHandler(nullptr, &header, packet);
        //}
        int res;

        while ((res = pcap_next_ex(handle, &header1, &pkt_data)) <= 0) {         
                continue;           
        }

        pcap_close(handle);
        return true;

}

bool DataHandler::NetworkCaptureFunction()
{
    // 获取物理接口列表
    std::vector<std::string> physicalInterfaces = GetPhysicalInterfaces();

    if (physicalInterfaces.empty())
    {
        std::cerr << "No connected physical network interfaces found." << std::endl;
        return false;
    }

    //// 要求用户选择一个接口
    //std::cout << "Please select a physical interface by entering its number: ";
    //std::cin >> selectedInterfaceNumber;

    //// 检查输入是否有效
    //if (selectedInterfaceNumber < 1 || selectedInterfaceNumber > physicalInterfaces.size())
    //{
    //    std::cerr << "Invalid interface number." << std::endl;
    //    return false;
    //}

    //// 打开用户选择的接口
    //const std::string& selectedInterface = physicalInterfaces[selectedInterfaceNumber - 1];

    return OpenSelectedInterface();
}



