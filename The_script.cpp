// The_script.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <opencv.hpp>
using namespace std;
using namespace cv;

#include "Preprocess.h"
#include "Homography.h"
#include "Stiprocess.h"
//Homography类参考 https://blog.csdn.net/czl389/article/details/60325970 ，将做出局部修改

DenoteTime denotetime;
bool stop = false;

int main()
{
	denotetime();
	int total_cameras = 2;//相机总数
	VideoCapture* videos = new VideoCapture[total_cameras];//视频源
	{
		string* names = new string[total_cameras];
		names[0] = "\0";
		//names[0] = "http://admin:admin@192.168.31.202:8081";
		names[1] = "http://admin:admin@192.168.31.15:8081";

		for (int i = 0; i < total_cameras; i++)
		{
			Openvideo(videos[i], names[i]);
		}
	}
	denotetime();//打开视频阶段，视频源生成为videos[i]，默认以total_cameras/2下表位置视频源的为参考进行拼接，输入保证按空间顺序连续输入
	
	MappingsCenter mapcenter(total_cameras, videos);//图像间映射管理中心
	while(!stop)
	{
		mapcenter.calculate();//计算可能有的所有单应性矩阵
		mapcenter.drawmatches(); if (waitKey(10) > 0)stop = true;
	}
	denotetime();//求解单应性矩阵阶段，使用MappingsCenter类集中管理，计算

	namedWindow("Result");

	Stitchingcontroler sticontroler(total_cameras, videos);
	bool stop = false;
	while (!stop)
	{
		sticontroler.Catchimgs();
		//默认直接截取，考虑到同时性问题，可能需要后续增加缓冲区等保证拼到一起的为同时刻の图像
		sticontroler.Changebright();
		//根据中间参考图像（或者综合考虑？如果时间复杂度允许）调整各张图像的整体亮度
		printmatrix(mapcenter(1, 0).getHomography());
		//imshow("Result", sticontroler.stitch());
		if (cv::waitKey(25) > 0)
			stop = true;
	}
	denotetime();//实时拼接阶段，分步骤进行，首先改亮度（也可改下对比度）

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
