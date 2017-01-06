//------------------------------------
// Created by Linghan 2016-09-29
// Changed by Pei Zhang and Yuxuan Hao 2016-12-28
// Changed by Linghan 2016-12-31
//------------------------------------
#ifndef __Functions__
#define __Functions__

#include <io.h>
#include <vector>
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
/**
@brief 根据路径得到文件
@param[in] string path: 文件路径
@param[in] string exd: 文件扩展名
@param[in] vector<string>& files: 所有文件名集合
*/
void getFiles(string path, string exd, vector<string>& files);

/**
@brief 根据名称分析视频来自视角1还是视角2
*/
int getFileAngle(string filename);

/**
@brief 得到transform Mat
*/
Mat getTransformMat(Mat &src, Mat &dst, int tag);

/**
@brief 初始化, 手工标定球、守门员、1队队员、2队队员位置
*/
void initialization(Mat &frame);


double kernelProfile(int x, int y, int centerx, int centery, double h);
double gvalue(int x, int y, int centerx, int centery, double h);
void showPlan(int imgIndex, Mat transMat, Mat &field);
void reSelect(Mat &frame, int imgIndex);
void correct_mouse(int event, int x, int y, int type, void *param);

#endif


