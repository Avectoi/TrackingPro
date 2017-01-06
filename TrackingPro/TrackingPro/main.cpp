//------------------------------------
// Created by Linghan 2016-09-29
// Changed by Pei Zhang and Yuxuan Hao 2016-12-28
// Changed by Linghan 2016-12-31
//------------------------------------

#include <Windows.h>
#include <time.h>
#include "function.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#define getcolor(i,j,k) frame.at<Vec3b>(i,j)[k]
#define gettime() clock() / CLOCKS_PER_SEC

using namespace std;

const int tracking_avi_w = 1017;
const int tracking_avi_h = 648;
const int plan_avi_w = 312;
const int plan_avi_h = 500;
vector<PlayerRec> vector_null;
bool isInit[2] = { false, false };
vector<PlayerRec> last_frame_pos[2];

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

/**
@brief
@param[in] string s: 文件名
@param[in] int tag: 标记是来自视角1还是视角2
@param[in] vector<PlayerRec> last_pos: 该视角视频上次结束的各点位置，默认值为vector_null
*/
void trackPro(string s, int tag) {
	VideoCapture cap(s);
	if (!cap.isOpened()){
		printf("Can't open file!\n");
		return;
	}
	cout << "now begin processing" << s << endl;
	long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	//cout<<"total Frames:"<<totalFrameNumber<<"帧"<<endl;

	VideoWriter tracking("result/tracking.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(tracking_avi_w, tracking_avi_h));
	VideoWriter plan("result/plan.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(plan_avi_w, plan_avi_h));

	SYSTEMTIME st, et, totTime;

	int imgIndex(0);
	double h = 3.0;
	Mat transMat;

	for (;;) { // deal with each frame

		Mat img;
		cap >> img;
		if (img.empty()) {
			cout << "over" << endl;
			break;
		}

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

		if (imgIndex == 0) { // 该视频段第1帧
			transMat = getTransformMat(frame, field, tag);
			if (!isInit[tag - 1]){
				initialization(frame);
			}
			destroyAllWindows();
			startp = 0;
			GetSystemTime(&st);
			for (int p = startp; p < totPlayers; p++) {
				if (isInit[tag - 1]){
					players[0][p].x1 = last_frame_pos[tag - 1][p - startp].x1;
					players[0][p].x2 = last_frame_pos[tag - 1][p - startp].x2;
					players[0][p].y1 = last_frame_pos[tag - 1][p - startp].y1;
					players[0][p].y2 = last_frame_pos[tag - 1][p - startp].y2;
				}
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
			isInit[tag - 1] = true;
		}

		else { // 该视频段其它帧
			//printf("frame: %d\n", imgIndex);
			for (int p = startp; p < totPlayers; p++) {
				x1 = players[imgIndex - 1][p].x1;
				y1 = players[imgIndex - 1][p].y1;
				x2 = players[imgIndex - 1][p].x2;
				y2 = players[imgIndex - 1][p].y2;

				pointsNo = (x2 - x1 + 1)*(y2 - y1 + 1);
				centerx = (x1 + x2) / 2;
				centery = (y1 + y2) / 2;

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
				char num[5];
				sprintf_s(num, "%d", p);
				putText(frame, num, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x2),
					CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255));
				//rectangle(frame, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x1),
				//cvPoint(players[imgIndex][p].y2, players[imgIndex][p].x2), Scalar(0, 0, 0, 0));
			}

			imshow("src", frame);
			tracking << frame;
			showPlan(imgIndex, transMat, field);
			plan << field;
		} // end else

		cvWaitKey(100);

		imgIndex++;

		auto key = cv::waitKey(30);
		if (key == 27) break; // 按下Esc退出
		if (key == 32){ // 按下空格则暂停
			cout << "--- Pause ---" << endl;
			reSelect(frame, imgIndex);
			cv::waitKey(0); // 任意键重新开始
			cout << "--- Resume ---" << endl;
		}
	} // end for(;;)

	if (imgIndex > totalFrameNumber - 2) {
		cout << "total frames: " << imgIndex << endl;
		totTime = et - st;
		printf("Running Time: %02d:%02d.%03d\n", totTime.wMinute, totTime.wSecond, totTime.wMilliseconds);
		//cv::waitKey(0);

		vector<PlayerRec>().swap(last_frame_pos[tag - 1]);
		for (int i = startp; i < totPlayers; i++){
			last_frame_pos[tag - 1].push_back(players[imgIndex - 1][i]);
		}
		cv::destroyWindow("src");
	}
}

//--------------------------------------------------------------------------------------------------

int main()
{
	char * filePath = ".\\src";
	vector<string> files;
	string file_name[100] = { "" };
	//获取该路径下的所有mp4文件

	getFiles(filePath, "mp4", files);

	int size_of_mp4_files = files.size();
	int processed_file_count = 0;

	while (processed_file_count < size_of_mp4_files){
		Sleep(1000);
		files.clear();
		getFiles(filePath, "mp4", files);
		size_of_mp4_files = files.size();
		for (int i = processed_file_count; i < size_of_mp4_files; i++){
			file_name[i] = files[i].c_str();
			int tag;
			tag = getFileAngle(file_name[i]);
			trackPro(files[i].c_str(), tag);
			cout << files[i].c_str() << endl;
			processed_file_count++;
		}

		//如果视频处理完、则等待
		while (processed_file_count >= size_of_mp4_files){
			Sleep(5000);
			cout << "wait";
			files.clear();
			getFiles(filePath, "mp4", files);
			size_of_mp4_files = files.size();
		}
	}
	return 0;
}

