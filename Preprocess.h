#pragma once

#include <iostream>
#include <opencv.hpp>
using namespace std;
using namespace cv;

#include "Homography.h"

void Openvideo(VideoCapture& v, string s);
void Openvideo(VideoCapture& v, long long id = 0);

class MappingsCenter {
	int total_num;
	Homography* _homos;
	VideoCapture* sources;

	void calculate(int index, int source1, int source2);//source1��ο�ͼ��source2����ͼ

public:
	MappingsCenter(int totalnum);
	MappingsCenter(int totalnum,VideoCapture* vsrc);

	Homography& operator()(int i, int j);//��iӳ�䵽j�ĵ�Ӧ�Ծ�����������ڽ�throw�������

	void calculate();//�������е�Ӧ�Ծ���
};
