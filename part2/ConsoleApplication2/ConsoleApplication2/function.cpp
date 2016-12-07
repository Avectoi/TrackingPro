//------------------------------------
// Created by Linghan 2016-09-29
//------------------------------------

#include "function.h"

int count;
int scale = 40;
int niches = 7; // upper(256/40)=7
bool drawFlag = false;
bool selectFlag = true;
Rect rect;
int totPlayers = 0;
int startp = 0;

vector<Point2f> srcpt(4);
vector<Point2f> dstpt(4);

PlayerRec players[1001][20];
int position[540][32] = { 0 };
double pu[20][200001];
double stdpu[20][200001];
double lastpu[200001];
double w[100][100];

//----------------------------------------------------------------

void pickPointSrc(int event, int x, int y, int type, void *param) {

	if (event == EVENT_LBUTTONDOWN && count<4) {
		srcpt[count] = Point2f(x, y);
		printf("srcpt[%d]:(%.1f, %.1f)\n", count, srcpt[count].x, srcpt[count].y);
		count++;
	}

	if (event == EVENT_LBUTTONDOWN && count == 4)
		destroyWindow("videoView");
}

//----------------------------------------------------------------

void pickPointDst(int event, int x, int y, int type, void *param) {

	if (event == EVENT_LBUTTONDOWN && count<4) {
		dstpt[count] = Point2f(x, y);
		printf("dstpt[%d]:(%.1f, %.1f)\n", count, dstpt[count].x, dstpt[count].y);
		count++;
	}

	if (event == EVENT_LBUTTONDOWN && count == 4)
		destroyWindow("birdView");
}

//----------------------------------------------------------------

Mat getTransformMat(Mat &src, Mat &dst, int tag) {

	/*count = 0;
	cv::namedWindow("videoView");
	imshow("videoView", src);
	cv::setMouseCallback("videoView", pickPointSrc);
	waitKey(0);

	count = 0;
	cv::namedWindow("birdView");
	imshow("birdView", dst);
	cv::setMouseCallback("birdView", pickPointDst);
	waitKey(0);*/

	//----- 1.mp4 -----
	if (tag == 1){
		srcpt[0] = Point2f(-198, 213);
		srcpt[1] = Point2f(353, 119);
		srcpt[2] = Point2f(988, 159);
		srcpt[3] = Point2f(270, 552);

		dstpt[0] = Point2f(8, 22);
		dstpt[1] = Point2f(303, 22);
		dstpt[2] = Point2f(303, 478);
		dstpt[3] = Point2f(8, 478);
	}

	//----- 2.mp4 -----
	if (tag == 2){
		srcpt[0] = Point2f(462, 270);
		srcpt[1] = Point2f(849, 117);
		srcpt[2] = Point2f(592, 85);
		srcpt[3] = Point2f(134, 169);

		dstpt[0] = Point2f(243, 406);
		dstpt[1] = Point2f(243, 95);
		dstpt[2] = Point2f(68, 95);
		dstpt[3] = Point2f(68, 406);
	}


	Mat transMat = getPerspectiveTransform(srcpt, dstpt);

	vector<Point2f> points, points_trans;
	for (int i = 0; i<src.rows; i++){
		for (int j = 0; j<src.cols; j++) {
			points.push_back(Point2f(j, i));
		}
	}
	perspectiveTransform(points, points_trans, transMat);
	Mat img_trans = Mat::zeros(dst.rows, dst.cols, CV_8UC3);
	int count = 0;
	for (int i = 0; i<src.rows; i++){
		uchar* p = src.ptr<uchar>(i);
		for (int j = 0; j<src.cols; j++){
			int y = points_trans[count].y;
			int x = points_trans[count].x;
			//printf("%d, %d, y:%d, 3x:%d\n",dst.rows, dst.cols, y, 3*x);
			if (y >= 0 && y<dst.rows && x >= 0 && x<dst.cols) {
				uchar* t = img_trans.ptr<uchar>(y);
				t[x * 3] = p[j * 3];
				t[x * 3 + 1] = p[j * 3 + 1];
				t[x * 3 + 2] = p[j * 3 + 2];
			}
			count++;
		}
	}
	imshow("afterTransformation", img_trans);
	//imwrite("src/afterTransform.png",img_trans);
	return transMat;
}

//-------------------------------------------------------------

void recordRect(int x1, int y1, int x2, int y2, int type) {
	//rectangle(frame, cvPoint(x1,y1), cvPoint(x2,y2), cvScalar(0, 0, 255, 0));
	players[0][totPlayers] = PlayerRec(y1, x1, y2, x2, type); // notice! swap x and y
	totPlayers++;
}

//----------------------------------------------------------------

void on_mouse(int event, int x, int y, int type, void *param) {

	int& otype = *(int*)param;
	switch (event) {
	case(EVENT_LBUTTONDOWN) : {
								  drawFlag = true;
								  rect = Rect(x, y, 0, 0);
								  printf("from (x:%d, y:%d) ", x, y);
								  break;
	}
	case(EVENT_MOUSEMOVE) : {
								if (drawFlag)
								{
									rect.width = x - rect.x;
									rect.height = y - rect.y;
								}
								break;
	}
	case(EVENT_LBUTTONUP) : {
								drawFlag = false;
								printf("to (x:%d, y:%d) type:%d\n", x, y, otype);
								recordRect(rect.tl().x, rect.tl().y,
									rect.br().x, rect.br().y, otype);
								break;
	}
	case(EVENT_RBUTTONDOWN) : { // ÓÒ¼üÍË³öÑ¡Ôñ
								  selectFlag = false;
								  printf("finish selection!\n");
								  break;
	}
	}
}

//----------------------------------------------------------------

void initialization(Mat &frame) {

	Mat tempimg;
	int type;

	selectFlag = true; type = 3;
	printf("Please select the position of ball\n");
	namedWindow("Ball");
	setMouseCallback("Ball", on_mouse, (void*)&type);
	while (selectFlag){
		frame.copyTo(tempimg);
		if (drawFlag)
			rectangle(tempimg, rect.tl(), rect.br(), cvScalar(0, 255, 255, 0));

		imshow("Ball", tempimg);
		if (waitKey(10) == 27) break; //keyvalue27:esc
	}
	destroyWindow("Ball");

	selectFlag = true; type = 0;
	printf("Please select the positions of goal keeper\n");
	namedWindow("Goal Keeper");
	setMouseCallback("Goal Keeper", on_mouse, (void*)&type);
	while (selectFlag) {
		frame.copyTo(tempimg);
		if (drawFlag)
			rectangle(tempimg, rect.tl(), rect.br(), cvScalar(0, 0, 0, 0));

		imshow("Goal Keeper", tempimg);
		if (waitKey(10) == 27) break;
	}
	destroyWindow("Goal Keeper");

	selectFlag = true; type = 1;
	printf("Please select the positions of team1 players\n");
	namedWindow("Team 1");
	setMouseCallback("Team 1", on_mouse, (void*)&type);
	while (selectFlag){
		frame.copyTo(tempimg);
		if (drawFlag)
			rectangle(tempimg, rect.tl(), rect.br(), cvScalar(0, 0, 255, 0));

		imshow("Team 1", tempimg);
		if (waitKey(10) == 27) break;
	}
	destroyWindow("Team 1");

	selectFlag = true; type = 2;
	printf("Please select the positions of team2 players\n");
	namedWindow("Team 2");
	setMouseCallback("Team 2", on_mouse, (void*)&type);
	while (selectFlag){
		frame.copyTo(tempimg);
		if (drawFlag)
			rectangle(tempimg, rect.tl(), rect.br(), cvScalar(255, 0, 0, 0));

		imshow("Team 2", tempimg);
		if (waitKey(10) == 27) break;
	}
	destroyWindow("Team 2");
	//imshow("src", frame);
	printf("total targets: %d", totPlayers);
}

//-------------------------------------------------------------

double kernelProfile(int x, int y, int centerx, int centery, double h) {
	double dist = (centerx - x)*(centerx - x) + (centery - y)*(centery - y);
	dist = dist / (h*h);
	//printf("%lf\n", exp(-0.5*dist));
	return exp(-0.5*dist);
}

//-------------------------------------------------------------

double gvalue(int x, int y, int centerx, int centery, double h) {
	//printf("enter\n");
	double dist = (centerx - x)*(centerx - x) + (centery - y)*(centery - y);
	dist = dist / (h*h);
	//printf("return\n");
	return -0.5 * exp(-0.5*dist);
}

//-------------------------------------------------------------

void showPlan(int imgIndex, Mat transMat, Mat &field) {
	vector<Point2f> pts, pts_trans;
	pts.clear();

	for (int p = startp; p < totPlayers; p++) {
		double footy = (players[imgIndex][p].y1 + players[imgIndex][p].y1) / 2;
		double footx = players[imgIndex][p].x2;
		pts.push_back(Point2f(footy, footx));
	}

	perspectiveTransform(pts, pts_trans, transMat);

	for (int p = startp; p < totPlayers; p++) {
		Point2f tpt = pts_trans[p];
		int type = players[0][p].type;
		CvScalar color;
		if (type == 1) color = Scalar(0, 0, 200); // red
		else if (type == 2) color = Scalar(0, 200, 200, 0); // yellow
		else if (type == 0) color = Scalar(0, 0, 0, 0); // black
		else color = Scalar(200, 0, 0, 0); // blue

		circle(field, tpt, 5, color, -1);
	}
	imshow("field", field);

}

//-------------------------------------------------------------

Mat ballCandidates(Mat img) {
	//-------------- Ground Map Construction --------------
	cv::Mat hsvimg;
	cv::cvtColor(img, hsvimg, CV_BGR2HSV);
	//HSVhist(hsvimg);
	cv::vector<cv::Mat> hsvplanes;
	cv::split(hsvimg, hsvplanes);
	//cv::imwrite("HSV.png",hsvimg);

	double meanSaturation;
	meanSaturation = cv::mean(hsvplanes[1])[0];
	double maxValue;
	cv::minMaxIdx(hsvplanes[2], NULL, &maxValue);

	//cv::Mat ground = img.clone();
	cv::Mat ground = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
	for (int i = 0; i<img.rows; i++){
		for (int j = 0; j<img.cols; j++) {
			if (img.at<cv::Vec3b>(i, j)[1] > 0.95*img.at<cv::Vec3b>(i, j)[2] && // g > 0.95r
				img.at<cv::Vec3b>(i, j)[2] > 0.95*img.at<cv::Vec3b>(i, j)[0] && // r > 0.95b
				hsvimg.at<cv::Vec3b>(i, j)[1] > 0.8*meanSaturation && // s > 0.8meanS
				hsvimg.at<cv::Vec3b>(i, j)[2] < 1.25*maxValue) { // v < 1.25maxV
				ground.at<cv::Vec3b>(i, j)[0] = 255;
				ground.at<cv::Vec3b>(i, j)[1] = 255;
				ground.at<cv::Vec3b>(i, j)[2] = 255;
			}
		}
	}
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	cv::morphologyEx(ground, ground, CV_MOP_CLOSE, element);

	cvtColor(ground, ground, CV_BGR2GRAY);
	ground = 255 - ground;

	//--------------- Contour Tracking -----------------
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat cont = ground.clone();
	cv::findContours(cont, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	cont = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
	for (int i = 0; i<contours.size(); i++){
		double area = cv::contourArea(contours[i]);
		if (area < 2 || area > 30) continue; // remove people and noise

		double peri = cv::arcLength(contours[i], true);
		double factor = (peri*peri) / (4 * 3.1415926*area);
		if (factor < 0.5 || factor > 2) continue; // circularity

		cv::drawContours(cont, contours, i, cv::Scalar(255, 255, 255), CV_FILLED, 8);
	}

	return cont;
}

//-------------------------------------------------------------

void ballCorrect_mouse(int event, int x, int y, int type, void *param) {

	int& index = *(int*)param;
	switch (event) {
	case(EVENT_LBUTTONDOWN) : {
								  drawFlag = true;
								  rect = Rect(x, y, 0, 0);
								  printf("New Ball Position -- from (x:%d, y:%d) ", x, y);
								  break;
	}
	case(EVENT_MOUSEMOVE) : {
								if (drawFlag)
								{
									rect.width = x - rect.x;
									rect.height = y - rect.y;
								}
								break;
	}
	case(EVENT_LBUTTONUP) : {
								drawFlag = false;
								printf("to (x:%d, y:%d)\n", x, y);
								players[index][0] = PlayerRec(rect.tl().x, rect.tl().y, rect.br().x, rect.br().y, 3);
								break;
	}
	case(EVENT_RBUTTONDOWN) : { // ÓÒ¼üÍË³öÑ¡Ôñ
								  selectFlag = false;
								  printf("finish selection!\n");
								  break;
	}
	}
}