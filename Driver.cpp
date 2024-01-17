#include <iostream>
#include <cstring>
#include <windows.h>
#include <vector>
#include "Driver.h"

#pragma once

int main() {
    // 创建一个 DataHandler 类的实例
    DataHandler dataHandler;
    // 无限循环执行整个过程
    while (true) {
        dataHandler.SoftRun();
    }
    return 0;
}

//Softrun
void DataHandler::SoftRun() {

    // 网口获取数据模块
    int count = 0;
    while (count < 16) {    //存16帧数据一起发送
        if (!NetworkCaptureFunction()) {
            // 如果 NetworkCaptureFunction() 返回失败，直接跳出当前循环
            return;
        }
        memcpy(&TimeData[1040 * count], pkt_data, 1040);
        count++;
    }

        /*调用 DataTransmissionFunction 并传递 char 数组*/
        DataTransmissionFunction(TimeData);


        ///**************连接测试***************/
        //char radarData[1024];
        //memset(radarData, 0, sizeof(radarData));

        //     
        //for (int i = 0; i < sizeof(radarData); i++) {
        //        radarData[i] = 'i';
        //    }

        ////数据传输
        //DataTransmissionFunction(radarData);


        // 休眠一段时间，例如0.1秒（根据需要调整休眠时间）
        Sleep(100);
}

