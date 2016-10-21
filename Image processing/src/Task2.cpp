#include "IMAGE.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "CImg.h"


using namespace cimg_library;
using namespace std;

const int HistogramSize = 256;

int * createhistogramtable(CImg<float> & image, int channel)
{
	int * histogram = new int[HistogramSize]();
	for (int x = 0; x < image.width(); x++)
	{
		for (int y = 0; y < image.height(); y++)
		{
			histogram[(int)image(x, y, 0, channel)]++;
		}
	}
	return histogram;
}


void createhistogramimage(CImg<float> & image, int channel)
{
	int * histogram = createhistogramtable(image, channel);
	int histogramheight = 0;
	for (int i = 0; i < HistogramSize; i++)
	{
		if (histogram[i] > histogramheight)
			histogramheight = histogram[i];
	}
	CImg<float> histogrampicture(HistogramSize, histogramheight/10 + 1, 1, 3);
	for (int x = 0; x < HistogramSize; x+=2)
	{
		for (float y = (histogramheight)/10 ; y > histogramheight/10-histogram[x]/10 ; y--)
		{
			histogrampicture(x, y, 0, 0) = 255;
			histogrampicture(x+1, y, 0, 0) = 255;
		}
	}
	histogrampicture.save("test.bmp");
}