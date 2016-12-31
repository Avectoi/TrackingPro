//------------------------------------
// Created by Linghan 2016-09-29
//------------------------------------
#ifndef __Functions__
#define __Functions__

#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv.hpp"

using namespace cv;

class PlayerRec {
public:
	int x1, y1, x2, y2;
	int type;

	PlayerRec(int tx1, int ty1, int tx2, int ty2, int t) {
		x1 = tx1;
		x2 = tx2;
		y1 = ty1;
		y2 = ty2;
		type = t;
	}

	PlayerRec() {
		x1 = x2 = y2 = y1 = type = 0;
	}
};

//--- global parameters ---
extern int scale;
extern int niches;
extern bool drawFlag;
extern bool selectFlag;
extern int totPlayers;
extern int startp;

extern PlayerRec players[][20];
extern int position[][32];
extern double pu[][200001];
extern double stdpu[][200001];
extern double lastpu[];
extern double w[][100];

//--- function declarations ---
Mat getTransformMat(Mat &src, Mat &dst, int tag);
void initialization(Mat &frame);
double kernelProfile(int x, int y, int centerx, int centery, double h);
double gvalue(int x, int y, int centerx, int centery, double h);
void showPlan(int imgIndex, Mat transMat, Mat &field);
Mat ballCandidates(Mat img);
void ballCorrect_mouse(int event, int x, int y, int type, void *param);

#endif


