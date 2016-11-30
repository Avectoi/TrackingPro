//------------------------------------
// Created by Linghan 2016-09-29
//------------------------------------

#include <Windows.h>
#include "function.h"
#define getcolor(i,j,k) frame.at<Vec3b>(i,j)[k]

#include <sstream>
#include <iostream>
#include <string>

#include <io.h>
#include <vector>
#include <iostream>


using namespace std;

SYSTEMTIME operator-(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl)
{
	SYSTEMTIME t_res;
	FILETIME v_ftime;
	ULARGE_INTEGER v_ui;
	__int64 v_right, v_left, v_res;
	SystemTimeToFileTime(&pSr, &v_ftime);
	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;
	v_right = v_ui.QuadPart;

	SystemTimeToFileTime(&pSl, &v_ftime);
	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;
	v_left = v_ui.QuadPart;

	v_res = v_right - v_left;

	v_ui.QuadPart = v_res;
	v_ftime.dwLowDateTime = v_ui.LowPart;
	v_ftime.dwHighDateTime = v_ui.HighPart;
	FileTimeToSystemTime(&v_ftime, &t_res);
	return t_res;
}

int main1(string s) {
	s = "src/1.mp4";
	int tag = 1;
	VideoCapture cap(s);
	if (!cap.isOpened())
	{
		printf("Can't open file!\n");
		return 0;
	}
	//long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);  
	//cout<<"total Frames:"<<totalFrameNumber<<"帧"<<endl;

	VideoWriter tracking("result/tracking.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(1017, 648));
	VideoWriter plan("result/plan.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(312, 500));

	SYSTEMTIME st, et, totTime;

	int imgIndex(0);
	double h = 3.0;
	Mat transMat;

	for (;;) { // deal with each frame

		Mat img;
		cap >> img;
		if (img.empty()) break;

		Mat field;
		field = imread("src/model.png");

		double imgScale = 0.6;
		Size dsize = Size(img.cols*imgScale, img.rows*imgScale); // shrink image by 60%
		Mat frame = Mat(dsize, CV_32S);
		resize(img, frame, dsize);
		//printf("size:%d, %d",frame.cols, frame.rows);

		char txt[5];
		sprintf_s(txt, "%d", imgIndex);
		putText(frame, txt, Point2f(10, 25), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
		int centerx, centery, x1, y1, x2, y2, pointsNo, lastCenterx, lastCentery;
		int ured, ugreen, ublue, u;

		if (imgIndex == 0) {
			transMat = getTransformMat(frame, field, 1);
			initialization(frame);
			destroyAllWindows();
			startp = 0;
			GetSystemTime(&st);
			for (int p = startp; p < totPlayers; p++) {
				//printf("player: %d\n", p);
				x1 = players[0][p].x1;
				y1 = players[0][p].y1;
				x2 = players[0][p].x2;
				y2 = players[0][p].y2;

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;
				memset(pu[p], 0, sizeof pu[p]);
				memset(stdpu[p], 0, sizeof stdpu[p]);
				//printf("x1y1:(%d, %d) x2y2:(%d, %d)\n", x1, x2, y1, y2);

				for (int i = x1; i <= x2; i++) {
					for (int j = y1; j <= y2; j++) {
						ublue = getcolor(i, j, 0) / scale;
						ugreen = getcolor(i, j, 1) / scale;
						ured = getcolor(i, j, 2) / scale;
						u = ured * niches * niches + ugreen * niches + ublue;
						pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
						stdpu[p][u] += kernelProfile(i, j, centerx, centery, h);
					}
				}

				for (int bgred = 3; bgred <= 3; bgred++)
				for (int bggreen = 3; bggreen <= 4; bggreen++)
				for (int bgblue = 1; bgblue <= 2; bgblue++) {
					pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
					stdpu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
				}
			}

			for (int p = startp; p < totPlayers; p++) {
				rectangle(frame, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x1),
					cvPoint(players[imgIndex][p].y2, players[imgIndex][p].x2), cvScalar(0, 0, 0, 0));
				//此处还没有转换坐标；
			}

			imshow("src", frame);
			tracking << frame;
			showPlan(imgIndex, transMat, field);
			plan << field;
		}

		else {
			//printf("frame: %d\n", imgIndex);
			for (int p = startp; p < totPlayers; p++) {
				x1 = players[imgIndex - 1][p].x1;
				y1 = players[imgIndex - 1][p].y1;
				x2 = players[imgIndex - 1][p].x2;
				y2 = players[imgIndex - 1][p].y2;

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;

				//----------------------- correct wrong ball detection -------------------------
				if (players[0][p].type == 3) { // ball

					int roir = 30;
					Mat ROI = ballCandidates(frame(Rect(centery - roir, centerx - roir, 2 * roir, 2 * roir)));
					imshow("ROI", ROI);

					double hole = 0;
					int startx = roir + x1 - centerx;
					int endx = roir + x2 - centerx;
					int starty = roir + y1 - centery;
					int endy = roir + y2 - centery;

					for (int i = startx; i <= endx; i++)
					for (int j = starty; j <= endy; j++) {
						if (ROI.at<Vec3b>(i, j)[0] == 255) {
							hole += 1;
						}
					}
					double factor = hole / pointsNo;

					if (factor == 0) { // if loss, detect ball in ROI

						printf("ball is lost!\n");
						/*setMouseCallback("src", ballCorrect_mouse, (void*)&imgIndex);
						Mat tempimg;
						while(1){
						frame.copyTo(tempimg);
						if(drawFlag)
						rectangle(tempimg, Point2f(players[imgIndex][0].x1, players[imgIndex][0].y1),
						Point2f(players[imgIndex][0].x2, players[imgIndex][0].y2), cvScalar(0, 255, 255, 0));

						imshow("src",tempimg);
						if(waitKey(10) == 27) break; //keyvalue27:esc
						}*/

						std::vector<std::vector<cv::Point>> contours;
						std::vector<cv::Vec4i> hierarchy;
						cvtColor(ROI, ROI, CV_BGR2GRAY);
						cv::findContours(ROI, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
						ROI.release();

						double maxArea = 0;
						int bestIdx = 0;
						for (int i = 0; i<contours.size(); i++){
							double area = cv::contourArea(contours[i]);
							if (area > maxArea) {
								maxArea = area;
								bestIdx = i;
							}
							//double peri = cv::arcLength(contours[i], true);
							//double cir = abs((peri*peri) / (4*3.1415926*area)-1);
							//printf("area:%.3f, cir:%.3f\n",area, cir);
							//if(cir < minCir) {
							//	minCir = cir;
							//	bestIdx = i;
							//}
						}

						if (maxArea > 5) {

							Point2f c;
							float r;
							minEnclosingCircle(contours[bestIdx], c, r);
							int oldcenterx = centerx;
							int oldcentery = centery;
							centerx = c.y + centerx - roir; //x和y在opencv中是反的？
							centery = c.x + centery - roir;

							x1 = centerx + (x1 - oldcenterx);
							x2 = centerx + (x2 - oldcenterx);
							y1 = centery + (y1 - oldcentery);
							y2 = centery + (y2 - oldcentery);

							pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
							centerx = (x1 + x2) / 2;
							centery = (y1 + y2) / 2;
							printf("update: (%d,%d) -> (%d,%d)\n", oldcenterx, oldcentery, centerx, centery);
							//circle(frame, Point2f(oldcentery,oldcenterx), 3, Scalar(0,0,0), -1);
							//circle(frame, Point2f(centery, centerx), 3, Scalar(0,0,255), -1);

							memset(pu[p], 0, sizeof pu[p]);
							memset(stdpu[p], 0, sizeof stdpu[p]);

							for (int i = x1; i <= x2; i++) {
								for (int j = y1; j <= y2; j++) {
									ublue = getcolor(i, j, 0) / scale;
									ugreen = getcolor(i, j, 1) / scale;
									ured = getcolor(i, j, 2) / scale;
									u = ured * niches * niches + ugreen * niches + ublue;
									pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
									stdpu[p][u] += kernelProfile(i, j, centerx, centery, h);
								}
							}

							for (int bgred = 3; bgred <= 3; bgred++)
							for (int bggreen = 3; bggreen <= 4; bggreen++)
							for (int bgblue = 1; bgblue <= 2; bgblue++) {
								pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
								stdpu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
							}
						} // end if(maxArea>5)
					}// end if(factor==0)
				}// end if(ball)
				//-----------------------------------------------------------------------

				for (int i = 0; i < niches*niches*niches; i++)
					lastpu[i] = stdpu[p][i];

				memset(pu[p], 0, sizeof pu[p]);
				int cnt = 0;

				while (1) {
					cnt++;
					lastCenterx = centerx;
					lastCentery = centery;
					memset(pu[p], 0, sizeof pu[p]);

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							//printf("p:%d, pu[%d, %d]\n",p, i, j);
							if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) { // ensure within window
								ublue = getcolor(i, j, 0) / scale;
								ugreen = getcolor(i, j, 1) / scale;
								ured = getcolor(i, j, 2) / scale;
								u = ured * niches * niches + ugreen * niches + ublue;
								pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
								w[i - x1][j - y1] = sqrt(lastpu[u] / pu[p][u]);
							}
							else {
								printf("%d(%d): out of window!\n", p, players[p][0].type);
							}
						}
					}

					double sumc = 0.0, sumx = 0.0, sumy = 0.0;

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) // ensure within window
								sumc += w[i - x1][j - y1] * gvalue(i, j, centerx, centery, h);
						}
					}

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) { // ensure within window
								sumx += w[i - x1][j - y1] * gvalue(i, j, centerx, centery, h) * i;
								sumy += w[i - x1][j - y1] * gvalue(i, j, centerx, centery, h) * j;
							}
						}
					}
					//printf("sumx: %lf, sumy: %lf, sumc: %lf\n", sumx, sumy, sumc);

					int newx1, newx2, newy1, newy2, newcenterx, newcentery;

					if (sumc == 0) { // miss handle
						newcenterx = lastCenterx;
						newcentery = lastCentery;
					}
					else {
						newcenterx = int(sumx / sumc + 0.5);
						newcentery = int(sumy / sumc + 0.5);
					}

					//printf("x: (%d -> %d), y: (%d -> %d)\n", centerx, newcenterx, centery, newcentery);

					newx1 = newcenterx + (x1 - centerx);
					newx2 = newcenterx + (x2 - centerx);
					newy1 = newcentery + (y1 - centery);
					newy2 = newcentery + (y2 - centery);

					if (abs(lastCenterx - newcenterx)>0 || abs(lastCentery - newcentery)>0) {
						for (int i = x1; i <= x2; i++) {
							for (int j = y1; j <= y2; j++) {
								if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) { // ensure within window
									ublue = getcolor(i, j, 0) / scale;
									ugreen = getcolor(i, j, 1) / scale;
									ured = getcolor(i, j, 2) / scale;
									u = ured * niches * niches + ugreen * niches + ublue;
									pu[p][u] = 0.0;
								}
							}
						}
					}

					//printf("x1: (%d -> %d), x2: (%d -> %d), y1: (%d -> %d), y2: (%d -> %d)\n", x1, newx1, x2, newx2, y1, newy1, y2, newy2);

					x1 = newx1; x2 = newx2; y1 = newy1; y2 = newy2; centerx = newcenterx; centery = newcentery;

					if (abs(lastCenterx - centerx) <= 0 && abs(lastCentery - centery) <= 0) {
						break;
					}
				} // end while

				players[imgIndex][p].x1 = x1;
				players[imgIndex][p].y1 = y1;
				players[imgIndex][p].x2 = x2;
				players[imgIndex][p].y2 = y2;

				//printf("imgIndex: %d, player: %d, x1x2y1y2: (%d, %d, %d, %d)\n", imgIndex, p, x1, x2, y1, y2);
				for (int bgred = 3; bgred <= 3; bgred++)
				for (int bggreen = 3; bggreen <= 4; bggreen++)
				for (int bgblue = 1; bgblue <= 2; bgblue++) {
					pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
				}

			} // end for

			for (int p = startp; p < totPlayers; p++) {
				rectangle(frame, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x1),
					cvPoint(players[imgIndex][p].y2, players[imgIndex][p].x2), cvScalar(0, 0, 0, 0));
			}

			imshow("src", frame);
			tracking << frame;
			showPlan(imgIndex, transMat, field);
			plan << field;
		} // end else

		cvWaitKey(100);

		imgIndex++;
		//if (imgIndex>350) break; // cut the video...
	}

	cout << "total frames: " << imgIndex << endl;
	totTime = et - st;
	printf("Running Time: %02d:%02d.%03d\n", totTime.wMinute, totTime.wSecond, totTime.wMilliseconds);
	cv::waitKey(0);
	return 0;
}

int main2(string s) {
	s = "src/2.mp4";
	int tag = 2;
	VideoCapture cap("src/2.mp4");
	if (!cap.isOpened())
	{
		printf("Can't open file!\n");
		return 0;
	}
	//long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);  
	//cout<<"total Frames:"<<totalFrameNumber<<"帧"<<endl;

	VideoWriter tracking("result/tracking.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(1017, 648));
	VideoWriter plan("result/plan.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(312, 500));

	SYSTEMTIME st, et, totTime;

	int imgIndex(0);
	double h = 3.0;
	Mat transMat;

	for (;;) { // deal with each frame

		Mat img;
		cap >> img;
		if (img.empty()) break;

		Mat field;
		field = imread("src/model.png");

		double imgScale = 0.6;
		Size dsize = Size(img.cols*imgScale, img.rows*imgScale); // shrink image by 60%
		Mat frame = Mat(dsize, CV_32S);
		resize(img, frame, dsize);
		//printf("size:%d, %d",frame.cols, frame.rows);

		char txt[5];
		sprintf_s(txt, "%d", imgIndex);
		putText(frame, txt, Point2f(10, 25), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
		int centerx, centery, x1, y1, x2, y2, pointsNo, lastCenterx, lastCentery;
		int ured, ugreen, ublue, u;

		if (imgIndex == 0) {
			transMat = getTransformMat(frame, field, 2);
			initialization(frame);
			destroyAllWindows();
			startp = 0;
			GetSystemTime(&st);
			for (int p = startp; p < totPlayers; p++) {
				//printf("player: %d\n", p);
				x1 = players[0][p].x1;
				y1 = players[0][p].y1;
				x2 = players[0][p].x2;
				y2 = players[0][p].y2;

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;
				memset(pu[p], 0, sizeof pu[p]);
				memset(stdpu[p], 0, sizeof stdpu[p]);
				//printf("x1y1:(%d, %d) x2y2:(%d, %d)\n", x1, x2, y1, y2);

				for (int i = x1; i <= x2; i++) {
					for (int j = y1; j <= y2; j++) {
						ublue = getcolor(i, j, 0) / scale;
						ugreen = getcolor(i, j, 1) / scale;
						ured = getcolor(i, j, 2) / scale;
						u = ured * niches * niches + ugreen * niches + ublue;
						pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
						stdpu[p][u] += kernelProfile(i, j, centerx, centery, h);
					}
				}

				for (int bgred = 3; bgred <= 3; bgred++)
				for (int bggreen = 3; bggreen <= 4; bggreen++)
				for (int bgblue = 1; bgblue <= 2; bgblue++) {
					pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
					stdpu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
				}
			}

			for (int p = startp; p < totPlayers; p++) {
				rectangle(frame, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x1),
					cvPoint(players[imgIndex][p].y2, players[imgIndex][p].x2), cvScalar(0, 0, 0, 0));
				//此处还没有转换坐标；
			}

			imshow("src", frame);
			tracking << frame;
			showPlan(imgIndex, transMat, field);
			plan << field;
		}

		else {
			//printf("frame: %d\n", imgIndex);
			for (int p = startp; p < totPlayers; p++) {
				x1 = players[imgIndex - 1][p].x1;
				y1 = players[imgIndex - 1][p].y1;
				x2 = players[imgIndex - 1][p].x2;
				y2 = players[imgIndex - 1][p].y2;

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;

				//----------------------- correct wrong ball detection -------------------------
				if (players[0][p].type == 3) { // ball

					int roir = 30;
					Mat ROI = ballCandidates(frame(Rect(centery - roir, centerx - roir, 2 * roir, 2 * roir)));
					//imshow("ROI", ROI);

					double hole = 0;
					int startx = roir + x1 - centerx;
					int endx = roir + x2 - centerx;
					int starty = roir + y1 - centery;
					int endy = roir + y2 - centery;

					for (int i = startx; i <= endx; i++)
					for (int j = starty; j <= endy; j++) {
						if (ROI.at<Vec3b>(i, j)[0] == 255) {
							hole += 1;
						}
					}
					double factor = hole / pointsNo;

					if (factor == 0) { // if loss, detect ball in ROI

						printf("ball is lost!\n");
						/*setMouseCallback("src", ballCorrect_mouse, (void*)&imgIndex);
						Mat tempimg;
						while(1){
						frame.copyTo(tempimg);
						if(drawFlag)
						rectangle(tempimg, Point2f(players[imgIndex][0].x1, players[imgIndex][0].y1),
						Point2f(players[imgIndex][0].x2, players[imgIndex][0].y2), cvScalar(0, 255, 255, 0));

						imshow("src",tempimg);
						if(waitKey(10) == 27) break; //keyvalue27:esc
						}*/

						std::vector<std::vector<cv::Point>> contours;
						std::vector<cv::Vec4i> hierarchy;
						cvtColor(ROI, ROI, CV_BGR2GRAY);
						cv::findContours(ROI, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
						ROI.release();

						double maxArea = 0;
						int bestIdx = 0;
						for (int i = 0; i<contours.size(); i++){
							double area = cv::contourArea(contours[i]);
							if (area > maxArea) {
								maxArea = area;
								bestIdx = i;
							}
							//double peri = cv::arcLength(contours[i], true);
							//double cir = abs((peri*peri) / (4*3.1415926*area)-1);
							//printf("area:%.3f, cir:%.3f\n",area, cir);
							//if(cir < minCir) {
							//	minCir = cir;
							//	bestIdx = i;
							//}
						}

						if (maxArea > 5) {

							Point2f c;
							float r;
							minEnclosingCircle(contours[bestIdx], c, r);
							int oldcenterx = centerx;
							int oldcentery = centery;
							centerx = c.y + centerx - roir; //x和y在opencv中是反的？
							centery = c.x + centery - roir;

							x1 = centerx + (x1 - oldcenterx);
							x2 = centerx + (x2 - oldcenterx);
							y1 = centery + (y1 - oldcentery);
							y2 = centery + (y2 - oldcentery);

							pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
							centerx = (x1 + x2) / 2;
							centery = (y1 + y2) / 2;
							printf("update: (%d,%d) -> (%d,%d)\n", oldcenterx, oldcentery, centerx, centery);
							//circle(frame, Point2f(oldcentery,oldcenterx), 3, Scalar(0,0,0), -1);
							//circle(frame, Point2f(centery, centerx), 3, Scalar(0,0,255), -1);

							memset(pu[p], 0, sizeof pu[p]);
							memset(stdpu[p], 0, sizeof stdpu[p]);

							for (int i = x1; i <= x2; i++) {
								for (int j = y1; j <= y2; j++) {
									ublue = getcolor(i, j, 0) / scale;
									ugreen = getcolor(i, j, 1) / scale;
									ured = getcolor(i, j, 2) / scale;
									u = ured * niches * niches + ugreen * niches + ublue;
									pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
									stdpu[p][u] += kernelProfile(i, j, centerx, centery, h);
								}
							}

							for (int bgred = 3; bgred <= 3; bgred++)
							for (int bggreen = 3; bggreen <= 4; bggreen++)
							for (int bgblue = 1; bgblue <= 2; bgblue++) {
								pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
								stdpu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
							}
						} // end if(maxArea>5)
					}// end if(factor==0)
				}// end if(ball)
				//-----------------------------------------------------------------------

				for (int i = 0; i < niches*niches*niches; i++)
					lastpu[i] = stdpu[p][i];

				memset(pu[p], 0, sizeof pu[p]);
				int cnt = 0;

				while (1) {
					cnt++;
					lastCenterx = centerx;
					lastCentery = centery;
					memset(pu[p], 0, sizeof pu[p]);

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							//printf("p:%d, pu[%d, %d]\n",p, i, j);
							if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) { // ensure within window
								ublue = getcolor(i, j, 0) / scale;
								ugreen = getcolor(i, j, 1) / scale;
								ured = getcolor(i, j, 2) / scale;
								u = ured * niches * niches + ugreen * niches + ublue;
								pu[p][u] += kernelProfile(i, j, centerx, centery, h);// / double(pointsNo);
								w[i - x1][j - y1] = sqrt(lastpu[u] / pu[p][u]);
							}
							else {
								printf("%d(%d): out of window!\n", p, players[p][0].type);
							}
						}
					}

					double sumc = 0.0, sumx = 0.0, sumy = 0.0;

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) // ensure within window
								sumc += w[i - x1][j - y1] * gvalue(i, j, centerx, centery, h);
						}
					}

					for (int i = x1; i <= x2; i++) {
						for (int j = y1; j <= y2; j++) {
							if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) { // ensure within window
								sumx += w[i - x1][j - y1] * gvalue(i, j, centerx, centery, h) * i;
								sumy += w[i - x1][j - y1] * gvalue(i, j, centerx, centery, h) * j;
							}
						}
					}
					//printf("sumx: %lf, sumy: %lf, sumc: %lf\n", sumx, sumy, sumc);

					int newx1, newx2, newy1, newy2, newcenterx, newcentery;

					if (sumc == 0) { // miss handle
						newcenterx = lastCenterx;
						newcentery = lastCentery;
					}
					else {
						newcenterx = int(sumx / sumc + 0.5);
						newcentery = int(sumy / sumc + 0.5);
					}

					//printf("x: (%d -> %d), y: (%d -> %d)\n", centerx, newcenterx, centery, newcentery);

					newx1 = newcenterx + (x1 - centerx);
					newx2 = newcenterx + (x2 - centerx);
					newy1 = newcentery + (y1 - centery);
					newy2 = newcentery + (y2 - centery);

					if (abs(lastCenterx - newcenterx)>0 || abs(lastCentery - newcentery)>0) {
						for (int i = x1; i <= x2; i++) {
							for (int j = y1; j <= y2; j++) {
								if (i >= 0 && i<frame.rows && j >= 0 && j<frame.cols) { // ensure within window
									ublue = getcolor(i, j, 0) / scale;
									ugreen = getcolor(i, j, 1) / scale;
									ured = getcolor(i, j, 2) / scale;
									u = ured * niches * niches + ugreen * niches + ublue;
									pu[p][u] = 0.0;
								}
							}
						}
					}

					//printf("x1: (%d -> %d), x2: (%d -> %d), y1: (%d -> %d), y2: (%d -> %d)\n", x1, newx1, x2, newx2, y1, newy1, y2, newy2);

					x1 = newx1; x2 = newx2; y1 = newy1; y2 = newy2; centerx = newcenterx; centery = newcentery;

					if (abs(lastCenterx - centerx) <= 0 && abs(lastCentery - centery) <= 0) {
						break;
					}
				} // end while

				players[imgIndex][p].x1 = x1;
				players[imgIndex][p].y1 = y1;
				players[imgIndex][p].x2 = x2;
				players[imgIndex][p].y2 = y2;

				//printf("imgIndex: %d, player: %d, x1x2y1y2: (%d, %d, %d, %d)\n", imgIndex, p, x1, x2, y1, y2);
				for (int bgred = 3; bgred <= 3; bgred++)
				for (int bggreen = 3; bggreen <= 4; bggreen++)
				for (int bgblue = 1; bgblue <= 2; bgblue++) {
					pu[p][bgred*niches*niches + bggreen*niches + bgblue] = 0;
				}

			} // end for

			for (int p = startp; p < totPlayers; p++) {
				rectangle(frame, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x1),
					cvPoint(players[imgIndex][p].y2, players[imgIndex][p].x2), cvScalar(0, 0, 0, 0));
			}

			imshow("src", frame);
			tracking << frame;
			showPlan(imgIndex, transMat, field);
			plan << field;
		} // end else

		cvWaitKey(100);

		imgIndex++;
		if (imgIndex>50) break; // cut the video...
	}

	cout << "total frames: " << imgIndex << endl;
	totTime = et - st;
	printf("Running Time: %02d:%02d.%03d\n", totTime.wMinute, totTime.wSecond, totTime.wMilliseconds);
	cv::waitKey(0);
	return 0;
}



void getFiles(string path, string exd, vector<string>& files)
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string pathName, exdName;
	
	if (0 != strcmp(exd.c_str(), ""))
	{
		exdName = "\\*." + exd;
	}
	else
	{
		exdName = "\\*";
	}

	if ((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是文件夹中仍有文件夹,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(pathName.assign(path).append("\\").append(fileinfo.name), exd, files);
			}
			else
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					files.push_back(pathName.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


int main()
{
	string mulu = "src";
	int max = 100;
	string next_in = "";
	string front = "src/";
	string back = ".mp4";
	for (int i = 0;i < 2; i++)
	{
		string c;

		ostringstream oss;
		oss << i;

		c = oss.str();
		next_in = front +c + back;
		//cout << next_in;
	}
	if (next_in == "src/1.mp4")
		cout << 2333;

	char * filePath = "C:\\Users\\avectoi\\Documents\\Visual Studio 2013\\Projects\\ConsoleApplication2\\ConsoleApplication2\\src";
	vector<string> files;

	//获取该路径下的所有mp4文件
	getFiles(filePath, "mp4", files);

	int size = files.size();
	for (int i = 0; i < size; i++)
	{
		cout << files[i].c_str() << endl;
		main2(files[i].c_str());
	}



	//main2("src/2.mp4");
	//main2();
	
}

