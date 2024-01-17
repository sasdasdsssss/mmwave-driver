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
**��������RxDataCrc
**���ܣ���ԭʼ���ݽ���CRCУ�飬��ɾ������֡,������ȷ֡����
**�������1��pBuffer   ���ݻ���
**�������2��u16DataLen  ���ݳ���
**�������1������������
**���ߣ�ZJC
**ʱ�䣺2023.9.6
************************************************/
int  RxDataCrc(unsigned char* pBuffer, unsigned int u16DataLen)
{
	int reDataLen = 0;
	int HeadnNum = 0;
	int crcSum = 0;
	int framenum = 0;

	framenum = 8;


	//����ͷ֡
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

	//ƽ������
	for (int i = 0; i < u16DataLen; i++)
	{
		pBuffer[i] = pBuffer[i + HeadnNum];  //�˴����ݳ����Լ�СHeadnNum
	}

	//����ʣ�����鳤��
	int NewNum = (u16DataLen - HeadnNum);
	return NewNum;
}


/***********************************************
**��������DealDataAnalyze
**���ܣ����״�ԭʼ���ݴ���ɵ�������
**�������1��pBuffer   ���ݻ���
**�������2��u16DataLen  ���ݳ���
**�������1����
**���ߣ�ZJC
**ʱ�䣺2023.9.6
************************************************/
int  DataHandler::DealDataAnalyze(float* pBuffer, unsigned int u16DataLen)
{
	int reData = 0;
	return reData;
}