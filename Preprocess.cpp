#include "Preprocess.h"

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "TheErrors.h"

void Openvideo(VideoCapture& v, string s)
{
	if (s == "\0")
		v.open(0);
	else
		v.open(s);
	if (!v.isOpened())
	{
		std::cout << "video not open." << std::endl;
		throw("video not open of " + s);
	}
}
void Openvideo(VideoCapture& v, long long id)
{
	v.open(id);
	if (!v.isOpened())
	{
		std::cout << "video not open." << std::endl;
		throw("video not open ");
	}
}


void onMouse(int event, int x, int y, int flags, void* param)
{
	Mat* im = reinterpret_cast<Mat*>(param);
	switch (event)
	{
	case EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度
		std::cout << "at(" << x << "," << y << ")value is:"
			<< static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
	case EVENT_RBUTTONDOWN:    //鼠标右键按下响应：输入坐标并返回该坐标的灰度
		std::cout << "input(x,y)" << endl;
		std::cout << "x =" << endl;
		cin >> x;
		std::cout << "y =" << endl;
		cin >> y;
		std::cout << "at(" << x << "," << y << ")value is:"
			<< static_cast<int>(im->at<uchar>(cv::Point(x, y))) << std::endl;
		break;
	}
}
/*
————————————————
版权声明：本文为CSDN博主「iracer」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https ://blog.csdn.net/iracer/article/details/49048481
*/

void Releasevideo(VideoCapture& v)
{
	v.release();
}

void MappingsCenter::calculate(int index, int source1, int source2)
{
	_homos[index].readImgs(sources[source1], sources[source2]);
	_homos[index].getMatches();
}

MappingsCenter::MappingsCenter(int totalnum)
{
	sources = nullptr;
	total_num = totalnum;
	_homos = new Homography[totalnum - (long long)1];
}

MappingsCenter::MappingsCenter(int totalnum, VideoCapture* vsrc)
{
	total_num = totalnum;
	_homos = new Homography[totalnum - (long long)1];
	sources = vsrc;
}

Homography& MappingsCenter::operator()(int i, int j)
{
	if (abs(i - j) != 1)
	{
		throw(Error_IN_MappingsCenter_operator_ijnotneighbor);
	}
	if (i > j)
	{
		if (i > total_num)throw(Error_IN_MappingsCenter_operator_ijposerror);
		else return _homos[j];
	}
	else
	{
		if (i < total_num)throw(Error_IN_MappingsCenter_operator_ijposerror);
		else return _homos[i];
	}
	// TODO: 在此处插入 return 语句
}

void MappingsCenter::calculate()
{
	for (int i = 0; i < total_num / 2; i++)
	{
		calculate(i, i + 1, i);
	}
	for (int i = total_num / 2; i < total_num - 1; i++)
	{
		calculate(i, i, i + 1);
	}
}

void MappingsCenter::drawmatches()
{
	for (int i = 0; i < total_num - 1; i++)
	{
		_homos[i].drawMatches();
		waitKey(50);
	}
}

DenoteTime::DenoteTime()
{
	time = 0;
}

void DenoteTime::operator()()
{
	if (!time)
	{
		time = getTickCount() / getTickFrequency();
	}
	else
	{
		printf("Time passed after the last node : %lld\n", int64(getTickCount() / getTickFrequency()) - time);
		time = getTickCount() / getTickFrequency();
	}
}
