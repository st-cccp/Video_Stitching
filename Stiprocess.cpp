#include "Stiprocess.h"

Stitchingcontroler::Stitchingcontroler(int totalnum)
{
	total_num = totalnum;
}

Stitchingcontroler::Stitchingcontroler(int totalnum, VideoCapture* vsrc)
{
	total_num = totalnum;
	sources = vsrc;
}

void Stitchingcontroler::Catchimgs()
{
	for (int i = 0; i++; i < total_num) {
		sources[i] >> imgs[i];
	}
	return;
}

void Stitchingcontroler::Changebright()
{
	Scalar* scalar = new Scalar[total_num];
	int* bright_difference = new int[total_num];
	int mid_num = total_num >> 1;
	int mid_bright = mean(imgs[mid_num]).val[0];
	for (int i = 0; i < total_num; i++) {
		scalar[i] = mean(imgs[i]);
		bright_difference[i] = scalar[i].val[0] - mid_bright;	//转换为灰度图计算亮度以及亮度差
		for (int y = 0; y < imgs[i].rows; y++)
		{
			for (int x = 0; x < imgs[i].cols; x++)
			{
				for (int c = 0; c < 3; c++)
				{
					imgs[i].at<Vec3b>(y, x)[c] = saturate_cast<uchar>(imgs[i].at<Vec3b>(y, x)[c]) + bright_difference[i];
				}
			}
		}
	}
	//调整亮度
	return;
}

Mat Stitchingcontroler::stitch()
{
	int mid_num = total_num >> 1;
	Mat* warp = new Mat[total_num];
	Mat* H = new Mat[total_num];
	Mat* h = new Mat[total_num];
	imgs[mid_num].copyTo(warp[mid_num]);
	for (int i = 0; i < mid_num; i++) {
		Homography homo(imgs[i], imgs[i + 1]);
		h[i] = homo.getHomography();
		H[i] = h[i];
	}
	for (int i = 0; i < mid_num; i++) {
			for (int j = i+1; j < mid_num; j++) {
				H[i] = h[j] * H[i];
			}
			float scale_H = H[i].at<double>(2, 2);
			H[i] = H[i] / scale_H;
			warpPerspective(imgs[i],warp[i],H[i],Size(imgs[mid_num].cols*total_num, imgs[mid_num].rows));
	}
	for (int i = total_num - 1; i > mid_num; i--) {
		Homography homo(imgs[i-1], imgs[i]);
		h[i] = homo.getHomography();
		H[i] = h[i];
	}
	for (int i = total_num - 1; i > mid_num; i--) {
			for (int j = i; j > mid_num; j--) {
				H[i] = H[i]*h[j];
			}
			H[i] = H[i].inv(DECOMP_LU);
			float scale_H = H[i].at<double>(2, 2);
			H[i] = H[i] / scale_H;
			warpPerspective(imgs[i], warp[i], H[i], Size(imgs[mid_num].cols*total_num, imgs[mid_num].rows));
	}
	int *x = new int[total_num];
	x[total_num-2]= imgs[mid_num].cols * 2 / 5;
	for (int i = total_num-2; i >=0; i--) {
		x[i] = x[i + 1] + h[i].at<double>(0, 2);
	}

	Mat canvas(imgs[mid_num].rows, imgs[mid_num].cols * total_num, CV_8UC3);
	warp[1](Range::all(), Range(x[1], x[1] + imgs[mid_num].cols)).copyTo(canvas(Range::all(), Range(x[1], x[1] + imgs[mid_num].cols)));
	for (int i = 1; i < total_num - 1; i++) {
		warp[i](Range::all(), Range(x[i], x[i - 1])).copyTo(canvas(Range::all(), Range(x[i], x[i - 1])));
	}
	warp[total_num-1].copyTo(canvas(Range::all(), Range(0, imgs[mid_num].cols)));
	return canvas;
}

