#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv.hpp"

#define getcolor(i,j,k) frame.at<Vec3b>(i,j)[k]

using namespace std;
using namespace cv;

int visit[1100][1700];
vector<Point2f> srcpt(4);
vector<Point2f> dstpt(4);


//自己添加  第一项为帧数  已有imgIndex可以作为  第二项为 球员 X Y 值；

int position[540][32] = { 0 };

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

PlayerRec players[1001][20];
int totPlayers = 0, startp = 0;
double pu[20][200001];
double stdpu[20][200001];
double lastpu[200001];
double w[100][100];
int scale, niches;



void drawRect(Mat &frame, int y1, int x1, int y2, int x2, int type) {
	//rectangle(frame, cvPoint(y1,x1), cvPoint(y2,x2), cvScalar(0, 0, 0, 0));
	players[0][totPlayers] = PlayerRec(x1, y1, x2, y2, type);
	totPlayers++;
}

void drawRect2(Mat &frame, int x1, int y1, int x2, int y2) {
	rectangle(frame, cvPoint(y1, x1), cvPoint(y2, x2), cvScalar(0, 0, 0, 0));
}

void prepare(Mat &frame) {
	drawRect(frame, 140, 141, 152, 161, 0);
	drawRect(frame, 162, 169, 174, 196, 2);
	drawRect(frame, 124, 207, 141, 242, 1);
	drawRect(frame, 243, 161, 256, 189, 1);
	drawRect(frame, 245, 151, 256, 172, 2);
	drawRect(frame, 298, 154, 307, 178, 2);
	drawRect(frame, 372, 135, 382, 159, 2);
	drawRect(frame, 415, 137, 424, 160, 2);
	drawRect(frame, 468, 169, 480, 189, 1);
	drawRect(frame, 464, 183, 476, 202, 1);
	drawRect(frame, 466, 199, 478, 226, 1);
	drawRect(frame, 450, 198, 465, 228, 2);
	drawRect(frame, 437, 189, 451, 216, 1);
	drawRect(frame, 517, 175, 531, 198, 2);
	drawRect(frame, 554, 178, 568, 200, 1);
	drawRect(frame, 653, 215, 672, 255, 0);
	drawRect(frame, 512, 145, 521, 153, 3);
	//imshow("src", frame);
}
//初始化球员位置

double kernelProfile(int x, int y, int centerx, int centery, double h) {
	double dist = (centerx - x)*(centerx - x) + (centery - y)*(centery - y);
	dist = dist / (h*h);
	//printf("%lf\n", exp(-0.5*dist));
	return exp(-0.5*dist);
}

double g(int x, int y, int centerx, int centery, double h) {
	//printf("enter\n");
	double dist = (centerx - x)*(centerx - x) + (centery - y)*(centery - y);
	dist = dist / (h*h);
	//printf("return\n");
	return -0.5 * exp(-0.5*dist);
}

void paint(Mat frame, int x1, int x2, int y1, int y2) {
	for (int i = x1; i <= x2; i++) {
		for (int j = y1; j <= y2; j++) {
			printf("(%d,%d,%d) ", getcolor(i, j, 2) / scale, getcolor(i, j, 1) / scale, getcolor(i, j, 0) / scale);
		}
		printf("\n");
	}
}

double penaltyLen, goalLen, penaltyWid, goalWid, circleDiam;

Mat court;
CvScalar fieldGreen = Scalar(71, 134, 108), white = Scalar(255, 255, 255, 0);
CvScalar Red = Scalar(0, 0, 200), Yellow = Scalar(0, 200, 200, 0), Blue = Scalar(200, 0, 0, 0), Black = Scalar(0, 0, 0, 0);
vector<Point2f> pts, pts_trans;
Mat transMat;

double dist(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void rePaintField(Mat &court) {
	court = Mat(620, 520, CV_8UC3, fieldGreen);
	rectangle(court, dstpt[0], dstpt[2], white, 1);

	double tx1, ty1, tx2, ty2, diameter;

	// mid
	tx1 = dstpt[0].x;
	ty1 = (dstpt[0].y + dstpt[2].y) / 2;
	tx2 = dstpt[2].x;
	ty2 = ty1;
	rectangle(court, cvPoint(tx1, ty1), cvPoint(tx2, ty2), white, 1);

	pts.clear();
	pts_trans.clear();
	//pts.push_back(Point2f(562, 298));pts.push_back(Point2f(349, 308));pts.push_back(Point2f(250, 207));
	pts.push_back(Point2f(567, 298)); pts.push_back(Point2f(349, 308)); pts.push_back(Point2f(250, 207));

	perspectiveTransform(pts, pts_trans, transMat);

	tx1 = pts_trans[0].x;
	ty1 = pts_trans[0].y;

	tx2 = pts_trans[1].x;
	ty2 = pts_trans[1].y;

	double xx1, xx2, yy1, yy2;
	// down left
	xx1 = tx1;
	yy1 = ty1;
	xx2 = dstpt[2].x - (tx1 - dstpt[3].x);
	yy2 = dstpt[2].y;
	rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

	// down right
	xx1 = tx2;
	yy1 = ty2;
	xx2 = dstpt[2].x - (tx2 - dstpt[3].x);
	yy2 = dstpt[2].y;
	rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

	// up left
	xx1 = tx2;
	yy1 = dstpt[0].y;
	xx2 = dstpt[1].x - (tx2 - dstpt[3].x);
	yy2 = dstpt[1].y + (dstpt[3].y - ty2);
	rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

	// up left
	xx1 = tx1;
	yy1 = dstpt[0].y;
	xx2 = dstpt[1].x - (tx1 - dstpt[3].x);
	yy2 = dstpt[1].y + (dstpt[3].y - ty1);
	rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

	//circle
	xx1 = (dstpt[0].x + dstpt[1].x) / 2.0;
	yy1 = (dstpt[0].y + dstpt[3].y) / 2.0;
	circle(court, cvPoint(xx1, yy1), dist(xx1, yy1, pts_trans[2].x, pts_trans[2].y), white);
}

void drawPoints(Mat &court, vector<Point2f> &pts, vector<Point2f> &pts_trans, int imgIndex) {
	/*while (!pts_trans.empty()) {
	Point2f tpt = pts_trans.back();
	pts_trans.pop_back();
	//court.at<Vec3b>(tpt.y, tpt.x)[0] = fieldGreen.val[0];
	//court.at<Vec3b>(tpt.y, tpt.x)[1] = fieldGreen.val[1];
	//court.at<Vec3b>(tpt.y, tpt.x)[2] = fieldGreen.val[2];
	circle(court, tpt, 5, fieldGreen, -1);
	}*/

	rePaintField(court);

	pts.clear();

	for (int p = startp; p < totPlayers; p++) {
		double tyy = players[imgIndex][p].y1;
		double txx = players[imgIndex][p].x2;
		//if (p == totPlayers - 1) txx = players[imgIndex][p].x2-(players[imgIndex][p].x2-players[imgIndex][p].x1)*0.1;
		pts.push_back(Point2f(tyy, txx));
	}

	perspectiveTransform(pts, pts_trans, transMat);

	int tmpK;
	tmpK = 0;
	for (int p = startp; p < totPlayers; p++) {
		Point2f tpt = pts_trans[p];


		position[imgIndex][tmpK] = (int)tpt.x;
		position[imgIndex][tmpK + 1] = (int)tpt.y;
		tmpK += 2;

		int type = players[0][p].type;
		CvScalar color;

		if (type == 1) color = Red;
		else if (type == 2) color = Yellow;
		else if (type == 0) color = Black;
		else color = Blue;

		/*court.at<Vec3b>(tpt.y, tpt.x)[0] = color.val[0];
		court.at<Vec3b>(tpt.y, tpt.x)[1] = color.val[1];
		court.at<Vec3b>(tpt.y, tpt.x)[2] = color.val[2];*/

		circle(court, tpt, 5, color, -1);
	}
}

int main() {
	VideoCapture cap("1.mp4");
	//一共是533帧
	VideoWriter tracking("tracking.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(1017, 647));
	VideoWriter calibration("calibration.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(520, 620));
	if (!cap.isOpened())
	{
		printf("Can't open file!\n");
		return 0;
	}

	int imgIndex(0);

	freopen("output.txt", "w", stdout);

	scale = 40, niches = 7;
	double h = 3.0;
	//int bgred = 7, bggreen = 13, bgblue = 11;

	for (;;)
	{
		Mat img;
		cap >> img;
		if (img.empty())
		{
			break;
		}
		double imgScale = 0.6;
		Size dsize = Size(img.cols*imgScale, img.rows*imgScale);
		//图片像素大小 原来大小 1696*1080  现在 *0。6
		cout << img.cols*imgScale << img.rows*imgScale;
		//printf("wojiushichangkuan(%d, %d)", int(img.cols*imgScale), int(img.rows*imgScale));
		//return 0;

		//CV_32S is a signed 32bit integer value for each pixel - again useful of you are doing integer maths on the pixels, 
		//but again needs converting into 8bits to save or display. 
		//This is trickier since you need to decide how to convert the much larger range of possible values (+/- 2billion!) into 0-255
		Mat frame = Mat(dsize, CV_32S);
		resize(img, frame, dsize);

		//
		int centerx, centery, x1, y1, x2, y2, pointsNo, lastCenterx, lastCentery;
		int ured, ugreen, ublue, u;

		if (imgIndex == 0) {
			prepare(frame);
			//printf("hello!\n");
			startp = 0;
			totPlayers = 16;
			for (int p = startp; p < totPlayers; p++) {
				//printf("player: %d\n", p);
				x1 = players[0][p].x1;
				y1 = players[0][p].y1;
				x2 = players[0][p].x2;
				y2 = players[0][p].y2;

				//paint(frame, x1, x2, y1, y2);

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;
				memset(pu[p], 0, sizeof pu[p]);
				memset(stdpu[p], 0, sizeof stdpu[p]);
				/*printf("here\n");
				printf("x1x2y1y2: %d, %d, %d, %d\n", x1, x2, y1, y2);
				printf("%d\n", scale);
				printf("width: %d, height: %d\n", frame.cols, frame.rows);*/
				for (int i = x1; i <= x2; i++) {
					for (int j = y1; j <= y2; j++) {
						//if (p == 15) printf("(%d, %d)", i, j);
						ublue = getcolor(i, j, 0) / scale;
						ugreen = getcolor(i, j, 1) / scale;
						ured = getcolor(i, j, 2) / scale;
						u = ured * niches * niches + ugreen * niches + ublue;
						pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
						stdpu[p][u] += kernelProfile(i, j, centerx, centery, h);
					}
				}

				//pu[p][bgred*niches*niches+bggreen*niches+bgblue] = 0;
				for (int bgred = 3; bgred <= 3; bgred++)
				for (int bggreen = 3; bggreen <= 4; bggreen++)
				for (int bgblue = 1; bgblue <= 2; bgblue++) {
					pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
					stdpu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
				}

				/*for (int i = x1; i <= x2; i++) {
				for (int j = y1; j <= y2; j++) {
				ured = getcolor(i,j,0)/scale;
				ugreen = getcolor(i,j,1)/scale;
				ublue = getcolor(i,j,2)/scale;
				u = ured * niches * niches + ugreen * niches + ublue;
				printf("%lf ", pu[p][u]);
				}
				printf("\n");
				}*/
			}
			// int tmpK;
			// tmpK = 0;
			for (int p = startp; p < totPlayers; p++) {
				drawRect2(frame, players[imgIndex][p].x1, players[imgIndex][p].y1, players[imgIndex][p].x2, players[imgIndex][p].y2);
				//此处还没有转换坐标；

			}

			//printf("bye!\n");

			// generate tracking
			tracking << frame;
			//imshow("src", frame);

			//vector<Point2f> srcpt(4);
			srcpt[0] = Point2f(-198, 213);
			srcpt[1] = Point2f(353, 119);
			srcpt[2] = Point2f(988, 159);
			srcpt[3] = Point2f(270, 552);
			//vector<Point2f> dstpt(4);
			dstpt[0] = Point2f(10, 10);
			dstpt[1] = Point2f(510, 10);
			dstpt[2] = Point2f(510, 610);
			dstpt[3] = Point2f(10, 610);

			//printf("xxxxxxxx:%lf\n", dstpt[1].x);

			transMat = getPerspectiveTransform(srcpt, dstpt);

			// setting ratios

			penaltyLen = 0.464808;
			goalLen = 0.252636;

			penaltyWid = 0.631701;
			goalWid = 0.582920;

			circleDiam = 0.194407;

			court = Mat(620, 520, CV_8UC3, fieldGreen);
			rectangle(court, dstpt[0], dstpt[2], white, 1);

			// up large
			double tx1, ty1, tx2, ty2, diameter;
			/*tx1 = (dstpt[1].x-dstpt[0].x)*(1-penaltyLen)/2.0+dstpt[0].x;
			ty1 = dstpt[0].y;
			tx2 = tx1+(dstpt[1].x-dstpt[0].x)*penaltyLen;
			ty2 = ty1+(dstpt[1].x-dstpt[0].x)*penaltyLen*penaltyWid;
			rectangle(court, cvPoint(tx1, ty1), cvPoint(tx2, ty2), white, 1);

			// up small
			tx1 = (dstpt[1].x-dstpt[0].x)*(1-goalLen)/2.0+dstpt[0].x;
			ty1 = dstpt[0].y;
			tx2 = tx1+(dstpt[1].x-dstpt[0].x)*goalLen;
			ty2 = ty1+(dstpt[1].x-dstpt[0].x)*goalLen*goalWid;
			rectangle(court, cvPoint(tx1, ty1), cvPoint(tx2, ty2), white, 1);

			// down large
			tx1 = (dstpt[2].x-dstpt[3].x)*(1-penaltyLen)/2.0+dstpt[3].x;
			ty1 = dstpt[3].y;
			tx2 = tx1+(dstpt[2].x-dstpt[3].x)*penaltyLen;
			ty2 = ty1-(dstpt[2].x-dstpt[3].x)*penaltyLen*penaltyWid;
			rectangle(court, cvPoint(tx1, ty1), cvPoint(tx2, ty2), white, 1);

			// down small
			tx1 = (dstpt[2].x-dstpt[3].x)*(1-goalLen)/2.0+dstpt[3].x;
			ty1 = dstpt[3].y;
			tx2 = tx1+(dstpt[2].x-dstpt[3].x)*goalLen;
			ty2 = ty1-(dstpt[2].x-dstpt[3].x)*goalLen*goalWid;
			rectangle(court, cvPoint(tx1, ty1), cvPoint(tx2, ty2), white, 1);*/

			// mid
			tx1 = dstpt[0].x;
			ty1 = (dstpt[0].y + dstpt[2].y) / 2;
			tx2 = dstpt[2].x;
			ty2 = ty1;
			rectangle(court, cvPoint(tx1, ty1), cvPoint(tx2, ty2), white, 1);

			//circle
			/*tx1 = (tx1+tx2)/2.0;
			ty1 = ty1;
			diameter = (dstpt[1].x-dstpt[0].x)*circleDiam;
			circle(court, cvPoint(tx1, ty1), diameter/2.0, white);*/

			pts.clear();
			pts_trans.clear();
			pts.push_back(Point2f(567, 298)); pts.push_back(Point2f(349, 308)); pts.push_back(Point2f(250, 207));

			perspectiveTransform(pts, pts_trans, transMat);

			tx1 = pts_trans[0].x;
			ty1 = pts_trans[0].y;

			tx2 = pts_trans[1].x;
			ty2 = pts_trans[1].y;

			double xx1, xx2, yy1, yy2;
			// down left
			xx1 = tx1;
			yy1 = ty1;
			xx2 = dstpt[2].x - (tx1 - dstpt[3].x);
			yy2 = dstpt[2].y;
			rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

			// down right
			xx1 = tx2;
			yy1 = ty2;
			xx2 = dstpt[2].x - (tx2 - dstpt[3].x);
			yy2 = dstpt[2].y;
			rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

			// up left
			xx1 = tx2;
			yy1 = dstpt[0].y;
			xx2 = dstpt[1].x - (tx2 - dstpt[3].x);
			yy2 = dstpt[1].y + (dstpt[3].y - ty2);
			rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

			// up left
			xx1 = tx1;
			yy1 = dstpt[0].y;
			xx2 = dstpt[1].x - (tx1 - dstpt[3].x);
			yy2 = dstpt[1].y + (dstpt[3].y - ty1);
			rectangle(court, cvPoint(xx1, yy1), cvPoint(xx2, yy2), white, 1);

			//circle
			xx1 = (dstpt[0].x + dstpt[1].x) / 2.0;
			yy1 = (dstpt[0].y + dstpt[3].y) / 2.0;
			circle(court, cvPoint(xx1, yy1), dist(xx1, yy1, pts_trans[2].x, pts_trans[2].y), white);

			pts.clear();
			pts_trans.clear();
			drawPoints(court, pts, pts_trans, imgIndex);

			calibration << court;
			//imshow("Football Pitch", court);
		}
		else {
			printf("frame: %d\n", imgIndex);
			for (int p = startp; p < totPlayers; p++) {

				//printf("player: %d, total: %d\n", p, totPlayers);

				x1 = players[imgIndex - 1][p].x1;
				y1 = players[imgIndex - 1][p].y1;
				x2 = players[imgIndex - 1][p].x2;
				y2 = players[imgIndex - 1][p].y2;
				//printf("shangmiande shangmiande! imgIndex: %d, player: %d, x1x2y1y2: (%d, %d, %d, %d)\n", imgIndex, p, x1, x2, y1, y2);

				//paint(frame, x1, x2, y1, y2);

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;
				//memset(lastpu, pu[p], sizeof lastpu);

				//printf("check1\n");


				for (int i = 0; i < niches*niches*niches; i++)
					lastpu[i] = stdpu[p][i];

				/*printf("start printf lastpu:\n");
				for (int i = 0; i < niches*niches*niches; i++)
				if (lastpu[i] > 0) {
				printf("%lf ", lastpu[i]);
				}
				printf("\n");*/

				memset(pu[p], 0, sizeof pu[p]);
				int cnt = 0;

				while (1) {
					cnt++;
					lastCenterx = centerx;
					lastCentery = centery;
					memset(pu[p], 0, sizeof pu[p]);
					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							ublue = getcolor(i, j, 0) / scale;
							ugreen = getcolor(i, j, 1) / scale;
							ured = getcolor(i, j, 2) / scale;
							u = ured * niches * niches + ugreen * niches + ublue;
							pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
							//printf("pu[%d, %d](%d, %d, %d)\n",i , j, ured, ugreen, ublue);
						}
					}

					//if (cnt == 1)
					{

						for (int i = x1; i <= x2; i++) {
							for (int j = y1; j <= y2; j++) {
								ublue = getcolor(i, j, 0) / scale;
								ugreen = getcolor(i, j, 1) / scale;
								ured = getcolor(i, j, 2) / scale;
								u = ured * niches * niches + ugreen * niches + ublue;
								w[i - x1][j - y1] = sqrt(lastpu[u] / pu[p][u]);
								//printf("%lf ", w[i-x1][j-y1]);
							}
							//printf("\n");
						}

						/*printf("\n(x1: %d, x2: %d, y1: %d, y2: %d)\n", x1, x2, y1, y2);
						for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
						ured = getcolor(i,j,0)/scale;
						ugreen = getcolor(i,j,1)/scale;
						ublue = getcolor(i,j,2)/scale;
						u = ured * niches * niches + ugreen * niches + ublue;
						printf("%lf ", w[i-x1][j-y1]);
						}
						printf("\n");
						}
						printf("end\n");*/
					}

					double sumc = 0.0, sumx = 0.0, sumy = 0.0;

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							sumc += w[i - x1][j - y1] * g(i, j, centerx, centery, h);
						}
					}

					/*printf("\n(x1: %d, x2: %d, y1: %d, y2: %d) player: %d\n", x1, x2, y1, y2, p);
					for (int i = x1; i <= x2; i++) {
					for (int j = y1; j <= y2; j++) {
					//printf("(w: %lf, w*g: %lf, i: %d, j: %d, centerx: %d, centery: %d)", w[i-x1][j-y1], w[i-x1][j-y1] * g(i, j, centerx, centery, h), i, j, centerx, centery);
					printf("%lf ", w[i-x1][j-y1] * g(i, j, centerx, centery, h));
					}
					printf("\n");
					}
					printf("end\n");*/

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							sumx += w[i - x1][j - y1] * g(i, j, centerx, centery, h) * i;
							sumy += w[i - x1][j - y1] * g(i, j, centerx, centery, h) * j;
						}
					}
					//printf("sumx: %lf, sumy: %lf, sumc: %lf\n", sumx, sumy, sumc);

					int newx1, newx2, newy1, newy2, newcenterx, newcentery;

					newcenterx = int(sumx / sumc + 0.5);
					newcentery = int(sumy / sumc + 0.5);

					//printf("x: (%d -> %d), y: (%d -> %d)\n", centerx, newcenterx, centery, newcentery);

					newx1 = newcenterx + (x1 - centerx);
					newx2 = newcenterx + (x2 - centerx);
					newy1 = newcentery + (y1 - centery);
					newy2 = newcentery + (y2 - centery);

					if (abs(lastCenterx - newcenterx)>0 || abs(lastCentery - newcentery)>0) {
						for (int i = x1; i <= x2; i++) {
							for (int j = y1; j <= y2; j++) {
								ublue = getcolor(i, j, 0) / scale;
								ugreen = getcolor(i, j, 1) / scale;
								ured = getcolor(i, j, 2) / scale;
								u = ured * niches * niches + ugreen * niches + ublue;
								pu[p][u] = 0.0;
							}
						}
					}

					//printf("x1: (%d -> %d), x2: (%d -> %d), y1: (%d -> %d), y2: (%d -> %d)\n", x1, newx1, x2, newx2, y1, newy1, y2, newy2);

					x1 = newx1; x2 = newx2; y1 = newy1; y2 = newy2; centerx = newcenterx; centery = newcentery;

					if (abs(lastCenterx - centerx) <= 0 && abs(lastCentery - centery) <= 0) {
						break;
					}
				}

				players[imgIndex][p].x1 = x1;
				players[imgIndex][p].y1 = y1;
				players[imgIndex][p].x2 = x2;
				players[imgIndex][p].y2 = y2;

				//printf("last haha! imgIndex: %d, player: %d, x1x2y1y2: (%d, %d, %d, %d)\n", imgIndex, p, x1, x2, y1, y2);
				//pu[p][bgred*niches*niches+bggreen*niches+bgblue] = 0;
				for (int bgred = 3; bgred <= 3; bgred++)
				for (int bggreen = 3; bggreen <= 4; bggreen++)
				for (int bgblue = 1; bgblue <= 2; bgblue++) {
					pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
				}
			}

			/*printf("pu[p][u]:\n");
			//int p = startp;
			for (int i = 0; i < niches; i++)
			for (int j = 0; j < niches; j++)
			for (int k = 0; k < niches; k++) {
			int u = i*niches*niches+j*niches+k;
			if (pu[startp][u] != 0) {
			printf("pu(%d, %d, %d):%lf\n", i, j, k, pu[startp][u]);
			}
			}
			printf("pu[p][u] ending\n");*/

			//printf("out\n");

			for (int p = startp; p < totPlayers; p++) {
				drawRect2(frame, players[imgIndex][p].x1, players[imgIndex][p].y1, players[imgIndex][p].x2, players[imgIndex][p].y2);
			}

			// tracking << frame;
			// imshow("src", frame);

			drawPoints(court, pts, pts_trans, imgIndex);

			// calibration << court;
			// imshow("Football Pitch", court);
		}

		/*for (int i = 100; i < 200; i++) {
		getcolor(i, 200, 0) = 0;
		getcolor(i, 200, 1) = 0;
		getcolor(i, 200, 2) = 0;
		}*/



		cvWaitKey(100);

		imgIndex++;

		if (imgIndex>40) break;
		//一共533帧
		//imshow("happy",frame);

	}
	cout << position[1][1];
	cout << "total frames: " << imgIndex << endl;
	return 0;
}
