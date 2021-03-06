// MouvementDetector.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//
#include "pch.h"
#include <iostream>
#include<opencv2\opencv.hpp>

using namespace cv;
using namespace std;

Mat convertToBinary(Mat frame);


int main()
{

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	Mat frame, secondFrame, binaryFrame, secondBinaryFrame, Mask;
	VideoCapture cap = VideoCapture(0);
		while (true)
		{
			cap.read(frame);
			waitKey(1);
			cap.read(secondFrame);
			binaryFrame = convertToBinary(frame);
			secondBinaryFrame = convertToBinary(secondFrame);

			bitwise_xor(binaryFrame, secondBinaryFrame, Mask);


			erode(Mask, Mask, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
			dilate(Mask, Mask, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

			findContours(Mask, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

	
			//Enlever les petits contours
			for (int i = 0; i < contours.size(); i++) 
			{
				if (contourArea(contours[i], false) < 100) 
				{
					contours.erase(contours.begin() + i);
					i = i - 1;
				}
			}
			
			//Vecteur pour dessiner un cercle
			vector<vector<Point> > contours_poly(contours.size());
			vector<Rect> boundRect(contours.size());
			vector<Point2f>center(contours.size());
			vector<float>radius(contours.size());

			for (int i = 0; i < contours.size(); i++)
			{
				
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));
				minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
			
				
			}

			for (int i = 0; i < contours.size(); i++)
			{
				
				circle(frame, center[i], (int)radius[i], Scalar(255, 255, 0), 2, 8, 0);
				drawContours(frame, contours_poly, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
				rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 0), 2, 8, 0);
			}

			imshow("Contours", frame);
			imshow("Mask", Mask);
		}
}

Mat convertToBinary(Mat frame) 
{
	Mat greyFrame;
	Mat binaryFrame;

	cvtColor(frame, greyFrame, CV_RGB2GRAY);
	threshold(greyFrame, binaryFrame, 100, 255, cv::THRESH_BINARY);
	return binaryFrame;
}


