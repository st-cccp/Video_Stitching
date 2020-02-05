#pragma once

#include <iostream>
#include <opencv.hpp>
using namespace std;
using namespace cv;

#include "TheErrors.h"

class Homography
{
private:
	Mat img[2];

	Mat imggray[2];

	Ptr<FeatureDetector> detector;
	Ptr<DescriptorExtractor> extractor;
	Ptr<DescriptorMatcher> matcher;

	vector<KeyPoint> keyPoints[2];

	Mat descriptors[2];

	vector<DMatch> firstMatches;
	vector<DMatch> matches;

	vector<Point2f> selfPoints1;
	vector<Point2f> selfPoints2;

	vector<uchar> inliers;

	Mat homography;

public:
	Homography();
	Homography(Mat img1, Mat img2);

	void readImgs(VideoCapture& v1, VideoCapture& v2);
	void setImgs(Mat img1, Mat img2);
	void setFeatureDetector(string detectorName);
	void setDescriptorExtractor(string descriptorName);
	void setDescriptorMatcher(string matcherName);
	void clearresult();

	vector<KeyPoint> getKeyPoints1();
	vector<KeyPoint> getKeyPoints2();

	Mat getDescriptors1();
	Mat getDescriptors2();

	vector<DMatch> getMatches();
	void drawMatches();

	Mat getHomography();

	~Homography();

private:
	void detectKeyPoints();
	void computeDescriptors();
	void match();
	void matchesToSelfPoints();
	void findHomography();
	void matchesFilter();
	
	bool checkmatch(DMatch m);
	bool colorcheck(DMatch m);
	void Changebright();
	void ChangeKeypoint(int bax0, int bay0, KeyPoint& kp);
	void detectKeyPointsHere(Rect area, int subscript);
	void detectKeyPointsBlocks(int index = 0);
/*	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970*/
};
