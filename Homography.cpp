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
	this->img[0] = img1;
	this->img[1] = img2;
	cvtColor(img1, imggray[0], COLOR_RGB2GRAY); cvtColor(img2, imggray[1], COLOR_RGB2GRAY);
}

void Homography::readImgs(VideoCapture& v1, VideoCapture& v2)
{
	if (!v1.read(img[0]))throw(Error_IN_Homography_readImgs);
	if (!v2.read(img[1]))throw(Error_IN_Homography_readImgs);
	cvtColor(img[0], imggray[0], COLOR_RGB2GRAY); cvtColor(img[1], imggray[1], COLOR_RGB2GRAY);
}

void Homography::setImgs(Mat Img1, Mat Img2)
{
	Img1.copyTo(this->img[0]);
	Img2.copyTo(this->img[1]);
	cvtColor(img[0], imggray[0], COLOR_RGB2GRAY); cvtColor(img[1], imggray[1], COLOR_RGB2GRAY);
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

void Homography::clearresult()
{
	descriptors[0].release();
	descriptors[1].release();
	matches.clear();
	firstMatches.clear();
	keyPoints[0].clear();
	keyPoints[1].clear();
	selfPoints1.clear();
	selfPoints2.clear();
	inliers.clear();
}

vector<KeyPoint> Homography::getKeyPoints1()
{
	if (keyPoints[0].size() == 0)
	{
		detectKeyPoints();
	}
	return keyPoints[0];
}

vector<KeyPoint> Homography::getKeyPoints2()
{
	if (keyPoints[1].size() == 0)
	{
		detectKeyPoints();
	}
	return keyPoints[1];
}

Mat Homography::getDescriptors1()
{
	if (descriptors[0].data == NULL)
	{
		computeDescriptors();
	}
	return descriptors[0];
}

Mat Homography::getDescriptors2()
{
	if (descriptors[1].data == NULL)
	{
		computeDescriptors();
	}
	return descriptors[1];
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
	cv::drawMatches(img[0], keyPoints[0], img[1], keyPoints[1], matches, matchImage, 255, 255);
	imshow("drawMatches", matchImage);
/*
	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970
*/
}

Mat Homography::getHomography()
{
	return homography;
}

Homography::~Homography()
{
}

void Homography::detectKeyPoints()
{
	detectKeyPointsBlocks();
	//detector->detect(imggray[0], keyPoints[0], Mat());
	//detector->detect(imggray[1], keyPoints[1], Mat());
}

void Homography::computeDescriptors()
{
	if (keyPoints[0].size() == 0 || keyPoints[1].size() == 0)
	{
		detectKeyPoints();
	}
	extractor->compute(imggray[0], keyPoints[0], descriptors[0]);
	extractor->compute(imggray[1], keyPoints[1], descriptors[1]);
	/*
	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

void Homography::match()
{
	if (descriptors[0].data == NULL || descriptors[1].data == NULL)
	{
		computeDescriptors();
	}
	matcher->match(descriptors[0], descriptors[1], firstMatches, Mat());
	/*
	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

void Homography::matchesToSelfPoints()
{
	for (vector<DMatch>::const_iterator it = firstMatches.begin(); it != firstMatches.end(); ++it)
	{
		selfPoints1.push_back(keyPoints[0].at(it->queryIdx).pt);
		selfPoints2.push_back(keyPoints[1].at(it->trainIdx).pt);
	}
	/*
	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970
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
	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970
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
			if (checkmatch(*itM))
			{
				matches.push_back(*itM);
			}
		}
	}
	/*
	————————————————
		版权声明：本文为CSDN博主「czl389」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
		原文链接：https ://blog.csdn.net/czl389/article/details/60325970
	*/
}

bool Homography::checkmatch(DMatch m)
{
	return colorcheck(m);
}

bool Homography::colorcheck(DMatch m)
{
	
	return true;
}

void Homography::Changebright()
{
	int mid_bright;
	int mid_bright1 = mean(imggray[0]).val[0];
	int mid_bright2 = mean(imggray[1]).val[0];
	if (mid_bright1 > mid_bright2)
	{
		mid_bright = mid_bright1;
	}
	else
	{
		mid_bright = mid_bright2;
	}
	for (int y = 0; y < imggray[0].rows; y++)
	{
		for (int x = 0; x < imggray[0].cols; x++)
		{
			imggray[0].at<uchar>(y, x) = saturate_cast<uchar>(imggray[0].at<uchar>(y, x)) - mid_bright1 + mid_bright;
		}
	}
	for (int y = 0; y < imggray[1].rows; y++)
	{
		for (int x = 0; x < imggray[1].cols; x++)
		{
			imggray[1].at<uchar>(y, x) = saturate_cast<uchar>(imggray[1].at<uchar>(y, x)) - mid_bright2 + mid_bright;
		}
	}
	//调整亮度
	return;
}

void Homography::ChangeKeypoint(int bax0, int bay0, KeyPoint& kp)
{
	kp.pt.x += bax0;
	kp.pt.y += bay0;
}

void Homography::detectKeyPointsHere(Rect area,int subscript)
{
	Mat image_cut = Mat(imggray[subscript], area);
	/*namedWindow("我们看看");
	imshow("我们看看", image_cut);
	waitKey(0);*/
	vector<KeyPoint> Detailkeypoint;
	detector->detect(image_cut, Detailkeypoint);
	int thesize = Detailkeypoint.size();
	for (int i = 0; i < thesize; i++)
	{
		Detailkeypoint[i].pt.x += area.tl().x;
		Detailkeypoint[i].pt.y += area.tl().y;
		keyPoints[subscript].push_back(Detailkeypoint[i]);
	}
}

void Homography::detectKeyPointsBlocks(int index)
{
	keyPoints[0].clear(); keyPoints[1].clear();
	int xseg; int yseg;
	Size img1size = imggray[0].size();
	Size img2size = imggray[1].size();
	int seg_num = 3;
	switch (index)
	{
	case 0:
		xseg = img1size.width / seg_num;
		yseg = img1size.height / seg_num;
		for (int i = 0; i < seg_num; i++)
		{
			for (int j = 0; j < seg_num; j++)
			{
				Rect therec(i * xseg, j * yseg, max(xseg, img1size.width - (seg_num - 1) * xseg), max(yseg, img1size.height - (seg_num - 1) * yseg));
				detectKeyPointsHere(therec, 0);
			}
		}
		xseg = img2size.width / seg_num;
		yseg = img2size.height / seg_num;
		for (int i = 0; i < seg_num; i++)
		{
			for (int j = 0; j < seg_num; j++)
			{
				Rect therec(i * xseg, j * yseg, max(xseg, img1size.width - (seg_num - 1) * xseg), max(yseg, img1size.height - (seg_num - 1) * yseg));
				detectKeyPointsHere(therec, 1);
			}
		}
		break;
	default:
		break;
	}
}
