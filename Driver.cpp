#include <iostream>
#include <cstring>
#include <windows.h>
#include <vector>
#include "Driver.h"

#pragma once

int main() {
    // ����һ�� DataHandler ���ʵ��
    DataHandler dataHandler;
    // ����ѭ��ִ����������
    while (true) {
        dataHandler.SoftRun();
    }
    return 0;
}

//Softrun
void DataHandler::SoftRun() {

    // ���ڻ�ȡ����ģ��
    int count = 0;
    while (count < 16) {    //��16֡����һ����
        if (!NetworkCaptureFunction()) {
            // ��� NetworkCaptureFunction() ����ʧ�ܣ�ֱ��������ǰѭ��
            return;
        }
        memcpy(&TimeData[1040 * count], pkt_data, 1040);
        count++;
    }

        /*���� DataTransmissionFunction ������ char ����*/
        DataTransmissionFunction(TimeData);


        ///**************���Ӳ���***************/
        //char radarData[1024];
        //memset(radarData, 0, sizeof(radarData));

        //     
        //for (int i = 0; i < sizeof(radarData); i++) {
        //        radarData[i] = 'i';
        //    }

        ////���ݴ���
        //DataTransmissionFunction(radarData);


        // ����һ��ʱ�䣬����0.1�루������Ҫ��������ʱ�䣩
        Sleep(100);
}

