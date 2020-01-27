#include "Homography.h"

Homography::Homography()
{
	detector = ORB::create();
	extractor = detector;
	matcher = DescriptorMatcher::create("BruteForce");
}

Homography::Homography(Mat img1, Mat img2)
{
	new(this) Homography();
	this->img1 = img1;
	this->img2 = img2;
}

void Homography::readImgs(VideoCapture& v1, VideoCapture& v2)
{
	v1.read(img1);
	v2.read(img2);
}

void Homography::setImgs(Mat Img1, Mat Img2)
{
	Img1.copyTo(this->img1);
	Img2.copyTo(this->img2);
}

void Homography::setFeatureDetector(string detectorName)
{
}

void Homography::setDescriptorExtractor(string descriptorName)
{
}

void Homography::setDescriptorMatcher(string matcherName)
{
	matcher = DescriptorMatcher::create(matcherName);
}

vector<KeyPoint> Homography::getKeyPoints1()
{
	if (keyPoints1.size() == 0)
	{
		detectKeyPoints();
	}
	return keyPoints1;
}

vector<KeyPoint> Homography::getKeyPoints2()
{
	if (keyPoints2.size() == 0)
	{
		detectKeyPoints();
	}
	return keyPoints2;
}

Mat Homography::getDescriptors1()
{
	if (descriptors1.data == NULL)
	{
		computeDescriptors();
	}
	return descriptors1;
}

Mat Homography::getDescriptors2()
{
	if (descriptors2.data == NULL)
	{
		computeDescriptors();
	}
	return descriptors2;
}

vector<DMatch> Homography::getMatches()
{
	if (matches.size() == 0)
	{
		matchesFilter();
	}
	return matches;
}

void Homography::drawMatches()
{
	Mat matchImage;
	if (matches.size() == 0)
	{
		matchesFilter();
	}
	cv::drawMatches(img1, keyPoints1, img2, keyPoints2, matches, matchImage, 255, 255);
	imshow("drawMatches", matchImage);
/*
	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970
*/
}

Mat Homography::getHomography()
{
	return Mat();
}

Homography::~Homography()
{
}

void Homography::detectKeyPoints()
{
	detector->detect(img1, keyPoints1, Mat());
	detector->detect(img2, keyPoints2, Mat());
}

void Homography::computeDescriptors()
{
	if (keyPoints1.size() == 0 || keyPoints2.size() == 0)
	{
		detectKeyPoints();
	}
	extractor->compute(img1, keyPoints1, descriptors1);
	extractor->compute(img2, keyPoints2, descriptors2);
	/*
	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

void Homography::match()
{
	if (descriptors1.data == NULL || descriptors2.data == NULL)
	{
		computeDescriptors();
	}
	matcher->match(descriptors1, descriptors2, firstMatches, Mat());
	/*
	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

void Homography::matchesToSelfPoints()
{
	for (vector<DMatch>::const_iterator it = firstMatches.begin(); it != firstMatches.end(); ++it)
	{
		selfPoints1.push_back(keyPoints1.at(it->queryIdx).pt);
		selfPoints2.push_back(keyPoints2.at(it->trainIdx).pt);
	}
	/*
	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

void Homography::findHomography()
{
	if (firstMatches.size() == 0)
	{
		match();
	}
	if (selfPoints1.size() == 0 || selfPoints2.size() == 0)
	{
		matchesToSelfPoints();
	}
	inliers = vector<uchar>(selfPoints1.size(), 0);
	homography = cv::findHomography(selfPoints1, selfPoints2, inliers, FM_RANSAC, 1.0);
	/*
	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

void Homography::matchesFilter()
{
	if (0 == firstMatches.size())
	{
		findHomography();
	}

	vector<DMatch>::const_iterator itM = firstMatches.begin();
	vector<uchar>::const_iterator itIn = inliers.begin();

	for (; itIn != inliers.end(); ++itIn, ++itM)
	{
		if (*itIn)
		{
			matches.push_back(*itM);
		}
	}
	/*
	��������������������������������
		��Ȩ����������ΪCSDN������czl389����ԭ�����£���ѭ CC 4.0 BY - SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӽ���������
		ԭ�����ӣ�https ://blog.csdn.net/czl389/article/details/60325970
	*/
}
