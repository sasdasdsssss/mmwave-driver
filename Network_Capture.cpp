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

#define MAX_PACKETS 1000 // ������󲶻�����ݰ�����


pcap_if_t* d;
int i = 0;
pcap_if_t* alldevs;		//�����ӿ���Ϣ
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
        int interfaceNumber = 1; // ���ڱ�ʶ����ӿڵ����

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

//��������ֻ��ȡ�õ�ַ������ݰ�    arp -a�������鿴
char filter[] = "ether host ff:ff:ff:ff:ff:ff or host 169.254.14.62";
//char* filter = nullptr; // �����ù������������������ݰ�

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
    
    //�ҵ�����ΪRealtek PCIe GbE Family Controller�Ľӿڣ���ʱ����Բ鿴�������ĸ��޸�
    for (d = alldevs; d != NULL; d = d->next) {
        if (strstr(d->description, "Realtek PCIe GbE Family Controller") != NULL) {
            break; // �ҵ�ƥ��Ľӿڣ��˳�ѭ��
        }
    }


     if((handle = pcap_open(d->name, //�豸��
        65536,         //�������ݵ�����ֽ���
        1,     // 1����ģʽ,0Ϊ����ģʽ
        1000,         //����Ӧʱ��
        NULL,
        errbuf         //��������ŵ�λ��
    )) == NULL)
     {
         std::cout << "���豸ʧ�ܣ�" << std::endl;
         pcap_freealldevs(alldevs);	//�ͷ��豸
         return false;
     }

     if (filter != NULL)
     {
         NetMask = 0xffffff;

         if (pcap_compile(handle, &fcode, filter, 1, NetMask) < 0)
         {      
             std::cout << "�������" << std::endl;
             std::cout << "������Ϣ: " << pcap_geterr(handle) << std::endl; // ��ӡ������Ϣ
             pcap_close(handle);
             return false;
         }

         if (pcap_setfilter(handle, &fcode) < 0)
         {
             std::cout << "���ô���" << std::endl;
             std::cout << "������Ϣ: " << pcap_geterr(handle) << std::endl; // ��ӡ������Ϣ
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
    // ��ȡ����ӿ��б�
    std::vector<std::string> physicalInterfaces = GetPhysicalInterfaces();

    if (physicalInterfaces.empty())
    {
        std::cerr << "No connected physical network interfaces found." << std::endl;
        return false;
    }

    //// Ҫ���û�ѡ��һ���ӿ�
    //std::cout << "Please select a physical interface by entering its number: ";
    //std::cin >> selectedInterfaceNumber;

    //// ��������Ƿ���Ч
    //if (selectedInterfaceNumber < 1 || selectedInterfaceNumber > physicalInterfaces.size())
    //{
    //    std::cerr << "Invalid interface number." << std::endl;
    //    return false;
    //}

    //// ���û�ѡ��Ľӿ�
    //const std::string& selectedInterface = physicalInterfaces[selectedInterfaceNumber - 1];

    return OpenSelectedInterface();
}



