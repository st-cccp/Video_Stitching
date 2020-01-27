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

}

void Stitchingcontroler::Changebright()
{
	return;
}

Mat Stitchingcontroler::stitch()
{
	return Mat();
}

