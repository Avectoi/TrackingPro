//------------------------------------
// Created by Linghan 2017-01-03
//------------------------------------

#include <QApplication>
#include <QTime>
#include <Windows.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include <QtSql>

#include "function.h"
#include "mainwindow.h"

#define getcolor(i,j,k) frame.at<Vec3b>(i,j)[k]
#define gettime() clock() / CLOCKS_PER_SEC

using namespace std;
using namespace cv;

const int tracking_avi_w = 1017;
const int tracking_avi_h = 648;
const int plan_avi_w = 312;
const int plan_avi_h = 500;
vector<PlayerRec> vector_null;
bool isInit[2] = { false, false };
vector<PlayerRec> last_frame_pos[2];
int imgIndex;

//----------SQL USAGE GLOBALS-----------
QSqlDatabase db;
int posses[25]={0};
int now_posses=-1,last_posses=-1;
int Distance;
int out_flag=-1;
int event=1;

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
void trackPro(string s,  int tag, MainWindow &win) {
    VideoCapture cap(s);
    if (!cap.isOpened()){
        printf("Can't open file!\n");
        return;
    }
    cout << "now begin processing: " << s << endl;
    long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    cout<<"total Frames:"<<totalFrameNumber<<"frames"<<endl;

    VideoWriter tracking("F:/Study/TrackingPro/TrackQt/TrackQt/result/tracking.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(tracking_avi_w, tracking_avi_h));
    VideoWriter plan("F:/Study/TrackingPro/TrackQt/TrackQt/result/plan.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, Size(plan_avi_w, plan_avi_h));

    SYSTEMTIME st, et, totTime;

    imgIndex = 0;
    double h = 3.0;
    Mat transMat;

    for (;;) { // deal with each frame

        Mat img;
        cap >> img;
        if (img.empty()) {
            cout << "over" << endl;
            break;
        }
        cout<<"Processing Image Index:"<<imgIndex<<endl;

        Mat field;
        field = imread("D:/model.png");

        double imgScale = 0.6;
        Size dsize = Size(img.cols*imgScale, img.rows*imgScale); // shrink image by 60%
        Mat frame = Mat(dsize, CV_32S);
        resize(img, frame, dsize);

        //char txt[5];
        //sprintf_s(txt, "%d", imgIndex);
        //putText(frame, txt, Point2f(10, 25), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));
        int centerx, centery, x1, y1, x2, y2, type, pointsNo, lastCenterx, lastCentery;
        int ured, ugreen, ublue, u;

        if (imgIndex == 0) { // 该视频段第1帧
            transMat = getTransformMat(frame, field, tag);
            if(!isInit[tag-1]){
                initialization(frame);
            }
            destroyAllWindows();
            startp = 0;
            GetSystemTime(&st);
            for (int p = startp; p < totPlayers; p++) {
                if(isInit[tag-1]){
                    players[0][p].x1 = last_frame_pos[tag-1][p-startp].x1;
                    players[0][p].x2 = last_frame_pos[tag-1][p-startp].x2;
                    players[0][p].y1 = last_frame_pos[tag-1][p-startp].y1;
                    players[0][p].y2 = last_frame_pos[tag-1][p-startp].y2;
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

            //imshow("src", frame);
            win.cvImg = frame;
            win.showImage();
            tracking << frame;
            showPlan(imgIndex, transMat, field);
            plan << field;
            isInit[tag-1] = true;
        }

        else { // 该视频段其它帧
            //printf("frame: %d\n", imgIndex);
            for (int p = startp; p < totPlayers; p++) {
                //if(TrackingStatus[p]!=REGULAR)continue;
                //SPEEDUP USE, NOT NECESSARY
                x1 = players[imgIndex - 1][p].x1;
                y1 = players[imgIndex - 1][p].y1;
                x2 = players[imgIndex - 1][p].x2;
                y2 = players[imgIndex - 1][p].y2;
                type=players[imgIndex - 1][p].type;

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
                                TrackingStatus[p]=OUT_OF_WINDOW;
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
                players[imgIndex][p].type=type;

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
                //putText(frame, num, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x2),
                  //  CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255));
                rectangle(frame, cvPoint(players[imgIndex][p].y1, players[imgIndex][p].x1),
                    cvPoint(players[imgIndex][p].y2, players[imgIndex][p].x2), Scalar(0, 0, 0, 0));
            }

            //imshow("src", frame);
            win.cvImg = frame;
            win.showImage();
            tracking << frame;
            showPlan(imgIndex, transMat, field);
            plan << field;
        } // end else


        //win.showImage(frame);
        //imshow("field",field);
        cv::waitKey(50);
        int xvec[25]={0};
        int yvec[25]={0};
        for(int p = startp; p < totPlayers; p++){ // 想改
            xvec[p] = players[imgIndex][p].x2;
            yvec[p] = players[imgIndex][p].y2;
        }
        win.updatePosition(xvec,yvec);
        win.currentIndex = imgIndex;
        while(!win.isRunning()){
                QCoreApplication::processEvents();
        }
        //for(int i=0;i<23;i++)std::cout<<players[imgIndex][i].type<<' ';
        //std::cout<<std::endl;
        imgIndex++;

    } // end for(;;)

    //if (imgIndex > totalFrameNumber - 2) {
    cout << "total frames: " << imgIndex << endl;
    totTime = et - st;
    printf("Running Time: %02d:%02d.%03d\n", totTime.wMinute, totTime.wSecond, totTime.wMilliseconds);
    //cv::waitKey(0);

    vector<PlayerRec>().swap(last_frame_pos[tag-1]);
    for (int i = startp; i < totPlayers; i++){
        last_frame_pos[tag-1].push_back(players[imgIndex-1][i]);
    }
    cv::destroyWindow("src");
    //}
}

//-----------------SQL FEATURES-------------------------
void SQL_init(string FileName)
{
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("mysql");
    db.setUserName("root");
    db.setPassword("TrackingPro");
    if(!db.open())
    {
        qDebug()<<"Error"<<db.lastError().text();
        exit(0);
    }
    QSqlQuery query(db);
    query=QSqlQuery::QSqlQuery(db);
    QString SQLString;
    QString FileNameString=QString::fromStdString(FileName);
    cout<<FileName<<endl;
    SQLString="DROP TABLE IF EXISTS "+FileNameString+"_position";

    if(query.exec(SQLString))
    {
        cout<<"delete table success"<<endl;
    }
    else
    {
        cout<<"delete table failed"<<endl;
        db.close();
        exit(0);
    }
    SQLString="CREATE TABLE IF NOT EXISTS "+FileNameString+"_position (FRAME INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT, X1 FLOAT, Y1 FLOAT, X2 FLOAT, Y2 FLOAT, X3 FLOAT, Y3 FLOAT, X4 FLOAT, \
            Y4 FLOAT, X5 FLOAT, Y5 FLOAT, X6 FLOAT, Y6 FLOAT, X7 FLOAT, Y7  FLOAT, X8 FLOAT, Y8 FLOAT, X9 FLOAT, Y9 FLOAT, X10 FLOAT, Y10 FLOAT, X11 FLOAT, Y11 FLOAT, X12 FLOAT, Y12 FLOAT,X13 FLOAT, Y13 FLOAT, X14 FLOAT,Y14 FLOAT,X15 FLOAT,Y15 FLOAT,X16 FLOAT,Y16 FLOAT,X17 FLOAT,Y17 FLOAT, \
                        X18 FLOAT, Y18 FLOAT,X19 FLOAT, Y19 FLOAT, X20 FLOAT,Y20 FLOAT,X21 FLOAT,Y21 FLOAT,X22 FLOAT,Y22 FLOAT,X23 FLOAT,Y23 FLOAT,X24 FLOAT,Y24 FLOAT,X25 FLOAT,Y25 FLOAT,X26 FLOAT,Y26 FLOAT,X27 FLOAT,Y27 FLOAT,X28 FLOAT,Y28 FLOAT,X29 FLOAT,Y29 FLOAT,X30 FLOAT,Y30 FLOAT);";
    if(query.exec(SQLString))
    {
        cout<<"create table 1 success"<<endl;
    }
    else
    {
        cout<<"create table 1 failed"<<endl;
        db.close();
        exit(0);
    }
    SQLString = "DROP TABLE IF EXISTS "+FileNameString+"_event";
    if (query.exec(SQLString))
    {
        cout << "delete table succeed" << endl;
    }
    else
    {
        cout << "delete table failed" << endl;
        db.close();
        exit(0);
    }
    SQLString = "CREATE TABLE IF NOT EXISTS "+FileNameString+"_event (ID INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,TYPE INTEGER,		\
                                    PLAYER1 INTEGER,X1 FLOAT,Y1 FLOAT,PLAYER2 INTEGER,X2 FLOAT,Y2 FLOAT,FRAME INTEGER);";
    if (query.exec(SQLString))
    {
        cout << "create table 2 succeed" << endl;
    }
    else
    {
        cout << "create table 2 failed" << endl;
        db.close();
        exit(0);
    }
    SQLString = "DROP TABLE IF EXISTS "+FileNameString+"_count";
    if (query.exec(SQLString))
    {
        cout << "delete table succeed" << endl;
    }
    else
    {
        cout << "delete table failed" << endl;
        db.close();
        exit(0);
    }
    SQLString = "CREATE TABLE IF NOT EXISTS "+FileNameString+"_count (TYPE INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,\
                                    P1 INTEGER,P2 INTEGER,P3 INTEGER,P4 INTEGER,P5 INTEGER,P6 INTEGER,P7 INTEGER,P8 INTEGER,P9 INTEGER,P10 INTEGER,\
                                                                            P11 INTEGER,P12 INTEGER,P13 INTEGER,P14 INTEGER,P15 INTEGER,P16 INTEGER,P17 INTEGER,P18 INTEGER,P19 INTEGER,P20 INTEGER,\
                                                                                                                                                        P21 INTEGER,P22 INTEGER,P23 INTEGER,P24 INTEGER,P25 INTEGER,P26 INTEGER,P27 INTEGER,P28 INTEGER,P29 INTEGER,P30 INTEGER);";
    if (query.exec(SQLString))
    {
        cout << "create table 3 succeed" << endl;
    }
    else
    {
        cout << "create table 3 failed" << endl;
        db.close();
        exit(0);
    }
    stringstream strStream;

    for (int i = 0; i < 61; i++){
        SQLString = "insert into "+FileNameString+"_count values(";
        strStream.str("");
        strStream << i + 1;
        string temp="";
        temp+=strStream.str();
        QString numString=QString::fromStdString(temp);
        SQLString += numString;
        SQLString+=",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);";
        if(query.exec(SQLString))
        {
            cout<<"table line "<<i<<" complete"<<endl;
        }
        else
        {
            cout<<"table line "<<i<<" failed"<<endl;
            db.close();
            exit(0);
        }
    }
}
void SQL_Process(string FileName)
{
    QSqlQuery query(db);
    query=QSqlQuery::QSqlQuery(db);
    QString SQLString;
    QString ParamString;
    stringstream strStream;
    QString FileNameString=QString::fromStdString(FileName);

    for(int i=0;i<imgIndex;i++)
    {
        last_posses=now_posses;
        int min=100000;
        SQLString="insert into "+FileNameString+"_position values(";
        strStream.str("");
        strStream<<i+1;
        //FULFILL
        for(int j=0;j<23;j++)
        {
            //x
            strStream<<",";
            strStream<<players[i][j].x2;
            //y
            strStream<<",";
            strStream<<players[i][j].y2;
        }
        for(int j=0;j<7;j++)
        {
            //x
            strStream<<",";
            strStream<<0;
            //y
            strStream<<",";
            strStream<<0;
        }
        strStream<<");";
        QString ParamStr=QString::fromStdString(strStream.str());
        SQLString+=ParamStr;
        if(query.exec(SQLString))
        {
            cout<<"Index "<<i<<" input complete"<<endl;
        }
        else
        {
            cout<<"Index "<<i<<" input failed"<<endl;
            db.close();
            exit(0);
        }
        //DISTANCE CHECK
        for(int j=1;j<23;j++)
        {
            if(TrackingStatus[j]!=REGULAR)continue;
            Distance=(players[i][j].x2-players[i][0].x2)*(players[i][j].x2-players[i][0].x2)+
                    (players[i][j].y2-players[i][0].y2)*(players[i][j].y2-players[i][0].y2);
            if(Distance<min)
            {
                min=Distance;
                now_posses=j;
            }
        }
        //POSSESSION CHECK
        if(now_posses==-1)continue;
        if(last_posses==-1)last_posses=now_posses;
        if(now_posses==last_posses)posses[now_posses]++;
        //Team1:1 2 Team2:3 4
        else if(abs(players[i][now_posses].type - players[i][last_posses].type)<=1)
        {
            //PASSING
            SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
            strStream.str("");
            strStream<<event++;
            strStream<<",";
            strStream<<1;
            strStream<<",";
            strStream<<last_posses;
            strStream<<",";
            strStream<<players[i][last_posses].x2;
            strStream<<",";
            strStream<<players[i][last_posses].y2;
            strStream<<",";
            strStream<<now_posses;
            strStream<<",";
            strStream<<players[i][now_posses].x2;
            strStream<<",";
            strStream<<players[i][now_posses].y2;
            strStream<<",";
            strStream<<i+1;
            strStream<<");";
            QString ParamStr=QString::fromStdString(strStream.str());
            SQLString+=ParamStr;
            if(query.exec(SQLString))
            {
                cout<<"PASSING INPUT "<<last_posses<<" "<<now_posses<<endl;
            }
            else
            {
                cout<<"PASSING INPUT FAILED"<<endl;
                db.close();
                exit(0);
            }
        }
        else if(abs(players[i][now_posses].type - players[i][last_posses].type)>1)
        {
            //STEAL
            SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
            strStream.str("");
            strStream<<event++;
            strStream<<",";
            strStream<<8;
            strStream<<",";
            strStream<<now_posses;
            strStream<<",";
            strStream<<players[i][now_posses].x2;
            strStream<<",";
            strStream<<players[i][now_posses].y2;
            strStream<<",";
            strStream<<last_posses;
            strStream<<",";
            strStream<<players[i][last_posses].x2;
            strStream<<",";
            strStream<<players[i][last_posses].y2;
            strStream<<",";
            strStream<<i+1;
            strStream<<");";
            QString ParamStr=QString::fromStdString(strStream.str());
            SQLString+=ParamStr;
            if(query.exec(SQLString))
            {
                cout<<"STEAL INPUT "<<now_posses<<" "<<last_posses<<endl;
            }
            else
            {
                cout<<"STEAL INPUT FAILED"<<endl;
                db.close();
                exit(0);
            }
        }

        int FieldLeft,FieldRight,FieldUp,FieldDown;
        for(int j=0;j<4;j++)
        {
            if(j==0)
            {
                FieldLeft=dstpt[j].x;
                FieldRight=dstpt[j].x;
                FieldUp=dstpt[j].y;
                FieldDown=dstpt[j].y;
            }
            else
            {
                if(dstpt[j].x<FieldLeft)FieldLeft=dstpt[j].x;
                if(dstpt[j].x>FieldRight)FieldRight=dstpt[j].x;
                if(dstpt[j].y<FieldUp)FieldUp=dstpt[j].y;
                if(dstpt[j].y>FieldDown)FieldDown=dstpt[j].y;
                //cout<<FieldLeft<<" "<<FieldRight<<" "<<FieldUp<<" "<<FieldDown<<endl;
            }
        }
        //cout<<FieldLeft<<" "<<FieldRight<<" "<<FieldUp<<" "<<FieldDown<<endl;

        int FieldWidth=FieldRight-FieldLeft;
        int FieldHeight=FieldDown-FieldUp;
        int PlayGroundLeft=FieldLeft+FieldWidth*35/800;
        int PlayGroundRight=FieldRight-FieldWidth*35/800;
        int PlayGroundUp=FieldUp+FieldHeight*15/500;
        int PlayGroundDown=FieldDown-FieldHeight*15/500;
        int GoalUp=FieldUp+FieldHeight*225/500;
        int GoalDown=FieldDown-FieldHeight*225/500;
        int AreaUp=FieldUp+FieldHeight*110/500;
        int AreaDown=FieldDown-FieldHeight*110/500;

        if(players[i][0].y2<PlayGroundUp||players[i][0].y2>PlayGroundDown)
        {
            if(out_flag==-1)
            {
                //Side Out
                SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
                strStream.str("");
                strStream<<event++;
                strStream<<",";
                strStream<<8;
                strStream<<",";
                strStream<<now_posses;
                strStream<<",";
                strStream<<players[i][now_posses].x2;
                strStream<<",";
                strStream<<players[i][now_posses].y2;
                strStream<<",";
                strStream<<-1;
                strStream<<",";
                strStream<<-1;
                strStream<<",";
                strStream<<-1;
                strStream<<",";
                strStream<<i+1;
                strStream<<");";
                QString ParamStr=QString::fromStdString(strStream.str());
                SQLString+=ParamStr;
                if(query.exec(SQLString))
                {
                    cout<<"SIDE OUT INPUT "<<now_posses<<endl;

                }
                else
                {
                    cout<<"SIDE OUT INPUT FAILED"<<endl;
                    db.close();
                    exit(0);
                }
                out_flag=1;
            }
        }
        if(players[i][0].x2<PlayGroundLeft||players[i][0].x2>PlayGroundRight)
        {
            if(out_flag==-1)
            {
                if(players[i][0].y2<GoalDown&&players[i][0].y2>GoalUp)
                {
                    //GOAL
                    SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
                    strStream.str("");
                    strStream<<event++;
                    strStream<<",";
                    strStream<<16;
                    strStream<<",";
                    strStream<<now_posses;
                    strStream<<",";
                    strStream<<players[i][now_posses].x2;
                    strStream<<",";
                    strStream<<players[i][now_posses].y2;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<i+1;
                    strStream<<");";
                    QString ParamStr=QString::fromStdString(strStream.str());
                    SQLString+=ParamStr;
                    if(query.exec(SQLString))
                    {
                        cout<<"GOAL INPUT "<<now_posses<<endl;
                    }
                    else
                    {
                        cout<<"GOAL INPUT FAILED"<<endl;
                        db.close();
                        exit(0);
                    }
                }
                if((players[i][0].y2<GoalUp&&players[i][0].y2>AreaUp)||(players[i][0].y2<AreaDown&&players[i][0].y2>GoalDown))
                {
                    //AREA OUT
                    SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
                    strStream.str("");
                    strStream<<event++;
                    strStream<<",";
                    strStream<<13;
                    strStream<<",";
                    strStream<<now_posses;
                    strStream<<",";
                    strStream<<players[i][now_posses].x2;
                    strStream<<",";
                    strStream<<players[i][now_posses].y2;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<i+1;
                    strStream<<");";
                    ParamStr=QString::fromStdString(strStream.str());
                    SQLString+=ParamStr;
                    if(query.exec(SQLString))
                    {
                        cout<<"AREA OUT INPUT "<<now_posses<<endl;
                    }
                    else
                    {
                        cout<<"AREA OUT INPUT FAILED"<<endl;
                        db.close();
                        exit(0);
                    }
                    //SIDE OUT AGAIN
                    SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
                    strStream.str("");
                    strStream<<event++;
                    strStream<<",";
                    strStream<<8;
                    strStream<<",";
                    strStream<<now_posses;
                    strStream<<",";
                    strStream<<players[i][now_posses].x2;
                    strStream<<",";
                    strStream<<players[i][now_posses].y2;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<i+1;
                    strStream<<");";
                    ParamStr=QString::fromStdString(strStream.str());
                    SQLString+=ParamStr;
                    if(query.exec(SQLString))
                    {
                        cout<<"AREA OUT INPUT2 "<<now_posses<<endl;
                    }
                    else
                    {
                        cout<<"AREA OUT INPUT2 FAILED"<<endl;
                        db.close();
                        exit(0);
                    }
                }
                if(players[i][0].y2<AreaUp&&players[i][0].y2>GoalDown)
                {
                    //NORMAL BASELINE OUT
                    SQLString="insert into "+FileNameString+"_event (ID,TYPE,PLAYER1,X1,Y1,PLAYER2,X2,Y2,FRAME)values(";
                    strStream.str("");
                    strStream<<event++;
                    strStream<<",";
                    strStream<<8;
                    strStream<<",";
                    strStream<<now_posses;
                    strStream<<",";
                    strStream<<players[i][now_posses].x2;
                    strStream<<",";
                    strStream<<players[i][now_posses].y2;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<-1;
                    strStream<<",";
                    strStream<<i+1;
                    strStream<<");";
                    QString ParamStr=QString::fromStdString(strStream.str());
                    SQLString+=ParamStr;
                    if(query.exec(SQLString))
                    {
                        cout<<"BASELINE OUT INPUT "<<now_posses<<endl;
                    }
                    else
                    {
                        cout<<"BASELINE OUT INPUT FAILED"<<endl;
                        db.close();
                        exit(0);
                    }
                }
                out_flag=1;
            }
        }
        if(players[i][0].x2>PlayGroundLeft&&players[i][0].x2<PlayGroundRight
                &&players[i][0].y2>PlayGroundUp&&players[i][0].y2<PlayGroundDown)
        {
            //Back into field
            out_flag=-1;
        }

    }
}

//--------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow win;
    win.show();
    QTextStream cout(stdout,  QIODevice::WriteOnly); // cout需要这一步,printf不需要

    string filePath = "D:/Video";
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
            trackPro(files[i].c_str(), tag, win);
            //SQL FUNCTION
            //if(processed_file_count==0)
            stringstream VideoTag;
            VideoTag.str("");
            for(int j=0;j<file_name[i].length()-1;j++)
            {
                char nameItem=file_name[i][j];
                if(nameItem>='0'&&nameItem<='9')VideoTag<<nameItem;
            }

            SQL_init(VideoTag.str());
            SQL_Process(VideoTag.str());
            //cout << files[i].c_str() << endl;
            processed_file_count++;
        }

        //如果视频处理完、则等待
        while (processed_file_count >= size_of_mp4_files){
            Sleep(5000);
            printf("wait\t");
            files.clear();
            getFiles(filePath, "mp4", files);
            size_of_mp4_files = files.size();
        }
    }

    return a.exec();
}
