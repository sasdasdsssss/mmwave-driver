#include "Driver.h"
#include <iostream>
#include <vector>
#include <cmath>

#pragma once

const double PI = 3.14159265358979323846;

void DealDataFunction()
{
	
}


/***********************************************
**函数名：RxDataCrc
**功能：对原始数据进行CRC校验，并删除错误帧,返回正确帧数量
**输入参数1：pBuffer   数据缓存
**输入参数2：u16DataLen  数据长度
**输出参数1：错误数据量
**作者：ZJC
**时间：2023.9.6
************************************************/
int  RxDataCrc(unsigned char* pBuffer, unsigned int u16DataLen)
{
	int reDataLen = 0;
	int HeadnNum = 0;
	int crcSum = 0;
	int framenum = 0;

	framenum = 8;


	//查找头帧
	for (int i = 0; i < framenum; i++)
	{
		if (pBuffer[i] == 0xaa)
		{
			if (pBuffer[i + 1] == 0xaa)
			{
				HeadnNum = i + 1;
				break;
			}
			else
			{
				HeadnNum = i;
				break;
			}
		}
	}

	//平移数组
	for (int i = 0; i < u16DataLen; i++)
	{
		pBuffer[i] = pBuffer[i + HeadnNum];  //此处数据长度以减小HeadnNum
	}

	//计算剩余数组长度
	int NewNum = (u16DataLen - HeadnNum);
	return NewNum;
}


/***********************************************
**函数名：DealDataAnalyze
**功能：将雷达原始数据处理成点云数据
**输入参数1：pBuffer   数据缓存
**输入参数2：u16DataLen  数据长度
**输出参数1：无
**作者：ZJC
**时间：2023.9.6
************************************************/
int  DataHandler::DealDataAnalyze(float* pBuffer, unsigned int u16DataLen)
{
	int reData = 0;
	return reData;
}