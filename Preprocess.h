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

	void calculate(int index, int source1, int source2);//source1表参考图，source2表到达图

public:
	MappingsCenter(int totalnum);
	MappingsCenter(int totalnum,VideoCapture* vsrc);

	Homography& operator()(int i, int j);//从i映射到j的单应性矩阵，如果不存在将throw错误代码

	void calculate();//计算所有单应性矩阵
};
