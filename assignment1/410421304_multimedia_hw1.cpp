// 410421304_multimedia_hw1.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include "math.h"


using namespace cv;


int main(int argc, char argv[])
{
	//printf("test111");

	/*
	IplImage *test;
	test = cvLoadImage("test.png");
	cvNamedWindow("test");
	cvShowImage("test", test);

	IplImage *gray;
	//8 bit depth gray
	gray = cvCreateImage(cvGetSize(test), 8, 1);

	//use BGR2GRAY to perform change
	cvCvtColor(test, gray, CV_BGR2GRAY);
	cvNamedWindow("gray");
	cvShowImage("gray", gray);
	*/

	//version 2 end


	Mat test = imread("test.png");


	//print color image
	imshow("color", test);

	Mat gray = test.clone();

	//opencv use bgr order

	int i, j;

	for (i = 0; i<test.rows; i++)
	{
		for (j = 0; j<test.cols; j++)
		{
			Vec3b pix = gray.at<Vec3b>(Point(j, i));

			uchar blu = pix[0];
			uchar gre = pix[1];
			uchar red = pix[2];
			//gray = (blue + green + red)/3
			uchar gra = ((blu + gre + red) / 3);

			pix[0] = gra;
			pix[1] = gra;
			pix[2] = gra;

			gray.at<Vec3b>(Point(j, i)) = pix;
		}
	}

	imshow("gray", gray);
	
	//try method L2-12
	//use mat to create image
	//r,c,img(8U,3channel)

	Mat imgbw;
	imgbw.create((i * 4) ,(j * 4) ,CV_8UC3);

	for (i = 0; i < test.rows; i++)
	{
		for (j = 0; j < test.cols; j++)
		{
			Vec3b pix = gray.at<Vec3b>(Point(j, i));

			uchar gra = pix[0];

			int a = 2 * i;
			int b = 2 * j;

			int setc = 0;

			if (gra < 51)
				setc = 0;
			else if (gra < 101)
				setc = 1;
			else if (gra < 151)
				setc = 2;
			else if (gra < 201)
				setc = 3;
			else if (gra <= 255)
				setc = 4;

			for (int c = a; c < a + 2; c++)
			{
				for (int d = b; d < b + 2; d++)
				{
					uchar paint;
					if (setc == 0)
						paint = 255;
					else if (setc == 4)
						paint = 0;
					else
					{
						if (setc == 1)
						{
							if ((d == b) && (c != a))
								paint = 0;
							else
								paint = 255;
						}
						else if (setc == 3)
						{
							if ((d != b) && (c == a))
								paint = 255;
							else
								paint = 0;
						}
						else if (setc == 2)
						{
							if (((d == b) && (c == a)) || ((d == b) && (c == a)))
								paint = 255;
							else
								paint = 0;
						}
					}

					Vec3b pix = imgbw.at<Vec3b>(Point(d, c));

					uchar blu = pix[0];
					uchar gre = pix[1];
					uchar red = pix[2];
					//pai = black / white depend on paint
					uchar pai = paint;

					pix[0] = pai;
					pix[1] = pai;
					pix[2] = pai;

					imgbw.at<Vec3b>(Point(d, c)) = pix;
				}
			}
		}
	}

	
	imshow("L2-12", imgbw);

	
	//try method L2-13
	//use mat to create image
	//r,c,img(8U,3channel)

	Mat imgbw2 = gray.clone();
	
	Mat dith;
	dith.create(4, 4, CV_8UC3);

	//build up dither matix
	//row,column
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			uchar paint;
			
			if ((i == 0) && (j == 0))
				paint = 0;
			else if ((i == 0) && (j == 1))
				paint = 12;
			else if ((i == 0) && (j == 2))
				paint = 3;
			else if ((i == 0) && (j == 3))
				paint = 15;
			else if ((i == 1) && (j == 0))
				paint = 8;
			else if ((i == 1) && (j == 1))
				paint = 4;
			else if ((i == 1) && (j == 2))
				paint = 11;
			else if ((i == 1) && (j == 3))
				paint = 7;
			else if ((i == 2) && (j == 0))
				paint = 2;
			else if ((i == 2) && (j == 1))
				paint = 14;
			else if ((i == 2) && (j == 2))
				paint = 1;
			else if ((i == 2) && (j == 3))
				paint = 13;
			else if ((i == 3) && (j == 0))
				paint = 10;
			else if ((i == 3) && (j == 1))
				paint = 6;
			else if ((i == 3) && (j == 2))
				paint = 9;
			else if ((i == 3) && (j == 3))
				paint = 5;
			
			Vec3b pix = dith.at<Vec3b>(Point(j, i));

			uchar blu = pix[0];
			uchar gre = pix[1];
			uchar red = pix[2];
			
			uchar num = paint;

			pix[0] = num;
			pix[1] = num;
			pix[2] = num;

			dith.at<Vec3b>(Point(j, i)) = pix;
		}
	}
	

	//imshow("dither test", dith);
	
	//try to perform method L2-13
	for (i = 0; i < test.rows; i++)
	{
		for (j = 0; j < test.cols; j++)
		{
			//4*4 matrix, so mod 4
			int x = i % 4;
			int y = j % 4;

			uchar paint;

			Vec3b pix = imgbw2.at<Vec3b>(Point(j, i));

			uchar blu = pix[0];
			uchar gre = pix[1];
			uchar red = pix[2];

			uchar num = pix[0];
			
			//fix the matrix, divide into 16 parts
			num = num / 16;

			Vec3b pix2 = dith.at<Vec3b>(Point(y, x));
			uchar dit = pix2[0];
			if (num > dit)
				paint = 255;
			else
				paint = 0;

			num = paint;

			pix[0] = num;
			pix[1] = num;
			pix[2] = num;

			imgbw2.at<Vec3b>(Point(j, i)) = pix;
		}
	}
	
	imshow("L2-13", imgbw2);
	
	
	//
	//Try to modify the arrangement of the values in the Dither Matrix arbitrarily to compare the results
	//

	Mat imgbw3 = gray.clone();
	//imshow("gray33", gray);
	//imshow("color33", test);
	Mat dith2;
	dith2.create(4, 4, CV_8UC3);

	//build up dither matix
	//row,column
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			uchar paint;

			if ((i == 0) && (j == 0))
				paint = 0;
			else if ((i == 0) && (j == 1))
				paint = 1;
			else if ((i == 0) && (j == 2))
				paint = 2;
			else if ((i == 0) && (j == 3))
				paint = 3;
			else if ((i == 1) && (j == 0))
				paint = 4;
			else if ((i == 1) && (j == 1))
				paint = 5;
			else if ((i == 1) && (j == 2))
				paint = 6;
			else if ((i == 1) && (j == 3))
				paint = 7;
			else if ((i == 2) && (j == 0))
				paint = 8;
			else if ((i == 2) && (j == 1))
				paint = 9;
			else if ((i == 2) && (j == 2))
				paint = 10;
			else if ((i == 2) && (j == 3))
				paint = 11;
			else if ((i == 3) && (j == 0))
				paint = 12;
			else if ((i == 3) && (j == 1))
				paint = 13;
			else if ((i == 3) && (j == 2))
				paint = 14;
			else if ((i == 3) && (j == 3))
				paint = 15;

			Vec3b pix = dith2.at<Vec3b>(Point(j, i));

			uchar blu = pix[0];
			uchar gre = pix[1];
			uchar red = pix[2];

			uchar num = paint;

			pix[0] = num;
			pix[1] = num;
			pix[2] = num;

			dith2.at<Vec3b>(Point(j, i)) = pix;
		}
	}

	//try to perform method L2-13
	for (i = 0; i < test.rows; i++)
	{
		for (j = 0; j < test.cols; j++)
		{
			//4*4 matrix, so mod 4
			int x = i % 4;
			int y = j % 4;

			uchar paint;

			Vec3b pix = imgbw3.at<Vec3b>(Point(j, i));

			uchar blu = pix[0];
			uchar gre = pix[1];
			uchar red = pix[2];

			uchar num = pix[0];

			//fix the matrix, divide into 16 parts
			num = num / 16;

			Vec3b pix2 = dith2.at<Vec3b>(Point(y, x));
			uchar dit = pix2[0];
			if (num > dit)
				paint = 255;
			else
				paint = 0;

			num = paint;

			pix[0] = num;
			pix[1] = num;
			pix[2] = num;

			imgbw3.at<Vec3b>(Point(j, i)) = pix;
		}
	}
	imshow("L2-13(2)", imgbw3);

	cvWaitKey();

	return 0;
}

