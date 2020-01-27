#pragma once

#include <iostream>
#include <opencv.hpp>
using namespace std;
using namespace cv;

class Homography
{
private:
	Mat img1;
	Mat img2;

	Ptr<FeatureDetector> detector;
	Ptr<DescriptorExtractor> extractor;
	Ptr<DescriptorMatcher> matcher;

	vector<KeyPoint> keyPoints1;
	vector<KeyPoint> keyPoints2;

	Mat descriptors1;
	Mat descriptors2;

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
/*	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970*/
};

