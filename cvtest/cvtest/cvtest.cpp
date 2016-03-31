// cvtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/**
* @file objectDetection.cpp
* @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
* @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream
*/
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include "facedetect-dll.h"
using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);
void detectBylibfacedetect(Mat gray);

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
String face_cascade_name = "../haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "../haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

/**
* @function main
*/
void dopro()
{
	double va[] = { 1, 2, 3,4,5,6,7,8,9};
	double vb[] = { 0, 0, 1 ,2,2,1,1,2,3};
	double vc[3];

	CvMat Va = cvMat(3, 3, CV_64FC1, va);
	CvMat Vb = cvMat(3, 3, CV_64FC1, vb);
	CvMat Vc = cvMat(3, 3, CV_64FC1, vc);

//	double res = ConvDotProduct(&Va, &Vb); // dot product:   Va . Vb -> res
//	cout << res << endl;
}

union UnionIp
{
	struct MyStruct
	{
		unsigned char a;
		unsigned char b;
		unsigned char c;
		unsigned char d;
	} num;
	unsigned int ip;
};
void test(unsigned int num)
{
	UnionIp a;
	a.ip = num;
	ostringstream os;
	os << (int)a.num.d << "."
		<< (int)a.num.c << "."
		<< (int)a.num.b << "."
		<< (int)a.num.a << endl;
	cout << os.str() << endl;
}
int main(void)
{
	test(3232235777);
	dopro();
	return 1;
	VideoCapture capture;
	Mat frame;
	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading\n"); return -1; };

	//-- 2. Read the video stream
	capture.open(0);
	if (capture.isOpened())
	{
		for (;;)
		{
			capture >> frame;

			//-- 3. Apply the classifier to the frame
			if (!frame.empty())
			{
//				imshow(window_name, frame);
				detectAndDisplay(frame);
				//				detectBylibfacedetect(frame);
			}
			else
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}

			int c = waitKey(10);
			if ((char)c == 'c') { break; }

		}
	}
	return 0;
}

void detectBylibfacedetect(Mat frame)
{
	Mat gray;
	cvtColor(frame, gray, COLOR_BGR2GRAY);
	int * pResults = NULL;

	///////////////////////////////////////////
	// frontal face detection 
	// it's fast, but cannot detect side view faces
	//////////////////////////////////////////
	//!!! The input image must be a gray one (single-channel)
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_multiview_reinforce((unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, gray.step,
		1.1f, 3, 24);
//	printf("%d frontal faces detected.\n", (pResults ? *pResults : 0));
	//print the detection results
	for (int i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short * p = ((short*)(pResults + 1)) + 6 * i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];

// 		printf("face_rect=[%d, %d, %d, %d], neighbors=%d\n", x, y, w, h, neighbors);

		Point center(x + w / 2, y + h / 2);
		ellipse(frame, center, Size(w / 2, h / 2), 0, 0, 360, Scalar(255, 0, 255), 2, 8, 0);

	}
	imshow(window_name, frame);
}
/**
* @function detectAndDisplay
*/
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 2, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 3, 8, 0);
		}
	}
	//-- Show what you got
	imshow(window_name, frame);
}
