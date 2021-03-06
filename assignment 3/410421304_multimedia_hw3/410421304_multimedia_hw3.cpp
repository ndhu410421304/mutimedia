// 410421304_multimedia_hw3.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include "Windows.h"

using namespace cv;


int main()
{
	Mat i1in = imread("i1.pgm");
	Mat i2in = imread("i2.pgm");

	Mat i1img,i2img;

	cvtColor(i1in, i1img, CV_BGR2GRAY);
	cvtColor(i2in, i2img, CV_BGR2GRAY);

	int imgcols = i1img.cols;
	int imgrows = i1img.rows;
	
	float difference = 256 * 16 * 16;
	float smallestdifference = 256 * 16 * 16;

	float tempd = 0;

	int currentisearch;
	int currentjsearch;

	int z = 0;
	float temp;

	int t1=1;
	int t2=2;

	//sequntial search
	float stotaldifference = 0;
	float ssquare = 0;
	Mat i2p;
	i2p = i1in.clone();

	//timer
	float stime;
	stime = GetTickCount();

	//search window:31*31, macroblock:16*16 => 31-16 = 15, 15/2 = 7
	for (int i = 0; i < imgrows/16; i++)
	{
		for (int j = 0; j < imgcols/16; j++)
		{
			smallestdifference = 256 * 16 * 16;
			for (int isearch = 0; isearch < 31; isearch++)
			{
				for (int jsearch = 0; jsearch < 31; jsearch++)
				{
					difference = 0;
					for (int i2 = 16 * i; i2 <= (16 * (i + 1)); i2++)
					{
						for (int j2 = 16 * j; j2 <= (16 * (j + 1)); j2++)
						{
							if ((((isearch - 7) < 0) || ((jsearch - 7) < 0)) || (((isearch - 7) > imgcols) || ((jsearch - 7) > imgcols)))
								tempd = i2img.at<uchar>(j2, i2);
							else
								tempd = i2img.at<uchar>(j2, i2) - i1img.at<uchar>((j2 + jsearch - 7), (i2 + isearch - 7));
							if (tempd < 0)
								tempd = tempd*(-1);
							difference += tempd;
						}
					}
					if (smallestdifference > difference)
					{
						smallestdifference = difference;
						currentisearch = isearch;
						currentjsearch = jsearch;
					}
				}
			}

			for (int i2 = 16 * i; i2 < (16 * (i + 1)); i2++)
			{
				for (int j2 = 16 * j; j2 < (16 * (j + 1)); j2++)
				{
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2));
					Vec3b pix2 = i2p.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7));
					pix2 = pix;
					if (((j2 + currentjsearch - 7) <= 320) && ((i2 + currentisearch - 7) <= 240))
						i2p.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7)) = pix2;
				}
			}
		}
	}
	//show image

	imshow("reference", i1in);
	imshow("target", i2in);
	imshow("i2p by sequntial search", i2p);
	float stimelast = GetTickCount();
	stimelast = stimelast -stime;
	printf("execute time:%fms\n", stimelast);

	//2D logarithm search v3
	float dsquare = 0;
	float dtotaldifference = 0;
	Mat i2p2;
	i2p2 = i1in.clone();
	float dtime = GetTickCount();
	for (int i = 0; i < (imgrows / 16); i++)
	{

		for (int j = 0; j < (imgcols / 16); j++)
		{
			int searchcenterx = 16 * j;;
			int searchcentery = 16 * i;
			int dvalue = 4;
			int isearch, jsearch;
			int pointvisit = 9;
			int previouscenterx = 16 * j;
			int previouscentery = 16 * i;
			while (dvalue != 0)
			{
				smallestdifference = 256 * 16 * 16;

				for (int run = 0; run < pointvisit; run++)
				{
					if (run == 0)
					{
						jsearch = searchcenterx;
						isearch = searchcentery;
					}
					else if (run == 1)
					{
						jsearch = searchcenterx;
						isearch = searchcentery - dvalue;
					}
					else if (run == 2)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery;
					}
					else if (run == 3)
					{
						jsearch = searchcenterx + dvalue;
						isearch = searchcentery;
					}
					else if (run == 4)
					{
						jsearch = searchcenterx;
						isearch = searchcentery + dvalue;
					}
					//corner
					else if (run == 5)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery - dvalue;
					}
					else if (run == 6)
					{
						jsearch = searchcenterx + dvalue;
						isearch = searchcentery - dvalue ;
					}
					else if (run == 7)
					{
						jsearch = searchcenterx - dvalue;
						isearch = searchcentery + dvalue;
					}
					else if (run == 8)
					{
						jsearch = searchcenterx + dvalue;
						isearch = searchcentery + dvalue;
					}

					difference = 0;
					if (((isearch >= ((16 * i) - 15)) && (jsearch >= ((16 * j) - 15))) && ((isearch <= ((16 * i) + 15)) && (jsearch <= ((16 * j) + 15))))
					{
						for (int i2 = isearch; i2 <= (isearch + 16); i2++)
						{
							for (int j2 = jsearch; j2 <= (jsearch + 16); j2++)
							{
								if (((isearch < 0) || (jsearch < 0)) || ((isearch > imgrows) || (jsearch > imgcols)))
									tempd = i2img.at<uchar>(j2, i2) + 2550000;
								else
									tempd = i2img.at<uchar>(j2, i2) - i1img.at<uchar>((j2 + jsearch), (i2 + isearch));
								if (tempd < 0)
									tempd = tempd*(-1);
								difference += tempd;
							}
						}
					}
					if (smallestdifference > difference)
					{
						smallestdifference = difference;
						currentisearch = isearch;
						currentjsearch = jsearch;
					}
				}
				searchcenterx = currentjsearch;
				searchcentery = currentisearch;
				if (dvalue == 1)
				{
					dvalue = 0;
				}
				if ((searchcenterx == previouscenterx) && (searchcentery == previouscentery))
				{
					dvalue = dvalue / 2;
				}
				else
				{
					previouscenterx = searchcenterx;
					previouscentery = searchcentery;
				}
			}



			for (int i2 = 16 * i; i2 < (16 * (i + 1)); i2++)
			{
				for (int j2 = 16 * j; j2 < (16 * (j + 1)); j2++)
				{
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2));
					Vec3b pix2 = i2p2.at<Vec3b>(Point(searchcenterx, searchcentery));
					pix2 = pix;
					if (((searchcenterx + j2 - 16 * j)< imgcols) && ((searchcentery + i2 - 16 * i) < imgrows))
						i2p2.at<Vec3b>(Point(searchcenterx + j2 - 16 * j, searchcentery + i2 - 16 * i)) = pix2;
				}
			}
		}
	}


	float dtimelast = GetTickCount();
	dtimelast = dtimelast - dtime;
	printf("dtimelast:%f ms\n", dtimelast);

	imshow("i2p by 2D logarithm", i2p2);

	//testing Hierachical Saearch

	float htimestart = GetTickCount();

	float hsquare = 0;
	float htotaldifference = 0;

	Mat i1pyr1,i1pyr2;
	Mat i2pyr1,i2pyr2;

	Mat grayi1pyr1, grayi1pyr2;
	Mat grayi2pyr1, grayi2pyr2;

	pyrDown(i1in, i1pyr1);
	pyrDown(i1pyr1, i1pyr2);

	pyrDown(i2in, i2pyr1);
	pyrDown(i2pyr1, i2pyr2);

	cvtColor(i1pyr1, grayi1pyr1, CV_BGR2GRAY);
	cvtColor(i1pyr2, grayi1pyr2, CV_BGR2GRAY);

	cvtColor(i2pyr1, grayi2pyr1, CV_BGR2GRAY);
	cvtColor(i2pyr2, grayi2pyr2, CV_BGR2GRAY);

	Mat hier = i1in.clone();

	//search window:8*8, macroblock:4*4
	for (int i = 0; i < grayi2pyr2.rows / 4; i++)
	{
		for (int j = 0; j < grayi2pyr2.cols / 4; j++)
		{
			smallestdifference = 256 * 16 * 16;
			for (int isearch = 0; isearch < 8; isearch++)
			{
				for (int jsearch = 0; jsearch < 8; jsearch++)
				{
					difference = 0;
					for (int i2 = 4 * i; i2 <= (4 * (i + 1)); i2++)
					{
						for (int j2 = 4 * j; j2 <= (4 * (j + 1)); j2++)
						{
							if ((((isearch - 2) < 0) || ((jsearch - 2) < 0)) || (((isearch - 2) > imgcols) || ((jsearch - 2) > imgcols)))
								tempd = grayi2pyr2.at<uchar>(j2, i2);
							else
								tempd = grayi2pyr2.at<uchar>(j2, i2) - grayi1pyr2.at<uchar>((j2 + jsearch - 2), (i2 + isearch - 2));
							if (tempd < 0)
								tempd = tempd*(-1);
							difference += tempd;
						}
					}
					if (smallestdifference > difference)
					{
						smallestdifference = difference;
						currentisearch = isearch;
						currentjsearch = jsearch;
					}
				}
			}


			for (int i2 = 16 * i; i2 < (16 * (i + 1)); i2++)
			{
				for (int j2 = 16 * j; j2 < (16 * (j + 1)); j2++)
				{
					Vec3b pix = i1in.at<Vec3b>(Point(j2, i2));
					Vec3b pix2 = hier.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7));
					pix2 = pix;
					if ((((j2 + currentjsearch - 7) <= 320) && ((i2 + currentisearch - 7) <= 240)) && ((j2 + currentjsearch - 7) >= 0) && ((i2 + currentisearch - 7) >= 0))
						hier.at<Vec3b>(Point(j2 + currentjsearch - 7, i2 + currentisearch - 7)) = pix2;
				}
			}

		}
	}
	float htimelast = GetTickCount();
	htimelast = htimelast - htimestart;
	printf("htimelast:%f ms\n", htimelast);

	imshow("i2p by hierachical search", hier);

	// find SNR value
	Mat grayseq;
	cvtColor(i2p, grayseq, CV_BGR2GRAY);

	Mat gray2d;
	cvtColor(i2p2, gray2d, CV_BGR2GRAY);

	Mat grayhier;
	cvtColor(hier, grayhier, CV_BGR2GRAY);

	stotaldifference = 0;
	ssquare = 0;

	dtotaldifference = 0;
	dsquare = 0;

	htotaldifference = 0;
	hsquare = 0;

	int tmptotal = 0;

	for (int i = 0; i < 320; i++)
	{
		for (int j = 0; j < 240; j++)
		{
			tmptotal += i2img.at<uchar>(j, i) - grayseq.at<uchar>(j, i);
			tmptotal = tmptotal * tmptotal;
			stotaldifference += tmptotal;
			ssquare += grayseq.at<uchar>(j, i) * grayseq.at<uchar>(j, i);
			
			tmptotal = 0;

			tmptotal += i2img.at<uchar>(j, i) - gray2d.at<uchar>(j, i);
			tmptotal = tmptotal * tmptotal;
			dtotaldifference += tmptotal;
			dsquare += gray2d.at<uchar>(j, i) * gray2d.at<uchar>(j, i);

			tmptotal = 0;
			
			tmptotal += i2img.at<uchar>(j, i) - grayhier.at<uchar>(j, i);
			tmptotal = tmptotal * tmptotal;
			htotaldifference += tmptotal;
			hsquare += grayhier.at<uchar>(j, i) * grayhier.at<uchar>(j, i);

			tmptotal = 0;
		}
	}

	//count SNR

	Mat i2pimg; 
	Mat i2p2img;

	cvtColor(i2p, i2pimg, CV_BGR2GRAY);
	cvtColor(i2p2, i2p2img, CV_BGR2GRAY);

	float SNR1 = 0;
	float SNR2 = 0;
	float SNR3 = 0;

	float fsquare1 = 0;
	float fdifference1 = 0;

	float fsquare2 = 0;
	float fdifference2 = 0;

	SNR1 = ssquare / stotaldifference;
	SNR2 = dsquare / dtotaldifference;
	SNR3 = hsquare / htotaldifference;

	printf("sequntial search SNR:%f\n", SNR1);
	printf("2D logarithm search SNR:%f\n", SNR2);
	printf("Hierachical search SNR:%f\n", SNR3);

	i1in.release();
	i2in.release();
	i1img.release();
	i2img.release();
	i2p.release();
	i2p2.release();
	i2pimg.release();
	i2p2img.release();

	i1pyr1.release();
	i1pyr2.release();
	i2pyr1.release();
	i2pyr2.release();

	grayi1pyr1.release();
	grayi1pyr2.release();
	grayi2pyr1.release();
	grayi2pyr2.release();

	grayseq.release();
	gray2d.release();
	grayhier.release();

	cvWaitKey();
}

