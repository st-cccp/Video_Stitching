#pragma once

#include <iostream>
#include <opencv.hpp>
using namespace std;
using namespace cv;

#include "Homography.h"

class Stitchingcontroler {
	int total_num;
	VideoCapture* sources;
	Mat* imgs;

public:
	Stitchingcontroler(int totalnum);
	Stitchingcontroler(int totalnum, VideoCapture* vsrc);

	void Catchimgs();
	void Changebright();
	Mat stitch();
};
