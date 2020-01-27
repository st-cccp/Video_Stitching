#include "Preprocess.h"

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "TheErrors.h"

void Openvideo(VideoCapture& v, string s)
{
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

void MappingsCenter::calculate(int index, int source1, int source2)
{
	_homos[index].readImgs(sources[source1], sources[source2]);
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


