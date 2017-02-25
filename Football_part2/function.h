//------------------------------------
// Created by Linghan 2016-01-03
//------------------------------------
#ifndef FUNCTION_H
#define FUNCTION_H

#include <io.h>
#include <stdio.h>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <QTextStream>
#include <QImage>
#include <QLabel>
#include <QDebug>
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

extern PlayerRec players[][25];
extern int position[][32];
extern double pu[][200001];
extern double stdpu[][200001];
extern double lastpu[];
extern double w[][100];
extern int TrackingStatus[];

extern int REGULAR;
extern int OUT_OF_WINDOW;
extern int NOT_INCLUDED;

extern int FOOTBALL;
extern int TEAM_1_GOALKEEPER;
extern int TEAM_1_PLAYER;
extern int TEAM_2_GOALKEEPER;
extern int TEAM_2_PLAYER;

extern vector<Point2f> srcpt;
extern vector<Point2f> dstpt;
//--- function declarations ---

/**
  @brief ����·���õ��ļ�
  @param[in] string path: �ļ�·��
  @param[in] string exd: �ļ���չ��
  @param[in] vector<string>& files: �����ļ�������
*/
void getFiles(string path, string exd, vector<string>& files);

/**
  @brief �������Ʒ�����Ƶ�����ӽ�1�����ӽ�2
*/
int getFileAngle(string filename);

/**
 @brief �õ�transform Mat
*/
Mat getTransformMat(Mat &src, Mat &dst, int tag);

/**
 @brief ��ʼ��, �ֹ��궨��������Ա��1�Ӷ�Ա��2�Ӷ�Աλ��
*/
void initialization(Mat &frame);


double kernelProfile(int x, int y, int centerx, int centery, double h);
double gvalue(int x, int y, int centerx, int centery, double h);
void showPlan(int imgIndex, Mat transMat, Mat &field);
void reSelect(Mat &frame, int imgIndex, int &x, int &y, int c);

#endif // FUNCTION_H


