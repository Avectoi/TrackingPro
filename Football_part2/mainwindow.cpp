//------------------------------------
// Created by Linghan 2017-01-06
//------------------------------------

#include "function.h"
#include "mainwindow.h"
#include <QString>
using namespace cv;

MainWindow::MainWindow()
{
    cenWidget = new QWidget();
    this->setCentralWidget(cenWidget);
    setWindowTitle("Tracking Demo");
    initWidgets();

    QGroupBox *controlBox = new QGroupBox(tr("Control Window"));
    QVBoxLayout *controlBoxLayout = new QVBoxLayout;
    controlBoxLayout->addWidget(textLabel);
    controlBoxLayout->addWidget(controlButton);
    controlBox->setLayout(controlBoxLayout);

    QGroupBox *showBox = new QGroupBox(tr("Tracking Result"));
    QGridLayout *showGridLayout = new QGridLayout;
    showGridLayout->addWidget(ballButton,0,0);
    showGridLayout->addWidget(ballLabel,0,1);

    {
    showGridLayout->addWidget(t1p1Button,1,0);
    showGridLayout->addWidget(t1p1Label,1,1);
    showGridLayout->addWidget(t2p1Button,1,2);
    showGridLayout->addWidget(t2p1Label,1,3);

    showGridLayout->addWidget(t1p2Button,2,0);
    showGridLayout->addWidget(t1p2Label,2,1);
    showGridLayout->addWidget(t2p2Button,2,2);
    showGridLayout->addWidget(t2p2Label,2,3);

    showGridLayout->addWidget(t1p3Button,3,0);
    showGridLayout->addWidget(t1p3Label,3,1);
    showGridLayout->addWidget(t2p3Button,3,2);
    showGridLayout->addWidget(t2p3Label,3,3);

    showGridLayout->addWidget(t1p4Button,4,0);
    showGridLayout->addWidget(t1p4Label,4,1);
    showGridLayout->addWidget(t2p4Button,4,2);
    showGridLayout->addWidget(t2p4Label,4,3);

    showGridLayout->addWidget(t1p5Button,5,0);
    showGridLayout->addWidget(t1p5Label,5,1);
    showGridLayout->addWidget(t2p5Button,5,2);
    showGridLayout->addWidget(t2p5Label,5,3);

    showGridLayout->addWidget(t1p6Button,6,0);
    showGridLayout->addWidget(t1p6Label,6,1);
    showGridLayout->addWidget(t2p6Button,6,2);
    showGridLayout->addWidget(t2p6Label,6,3);

    showGridLayout->addWidget(t1p7Button,7,0);
    showGridLayout->addWidget(t1p7Label,7,1);
    showGridLayout->addWidget(t2p7Button,7,2);
    showGridLayout->addWidget(t2p7Label,7,3);

    showGridLayout->addWidget(t1p8Button,8,0);
    showGridLayout->addWidget(t1p8Label,8,1);
    showGridLayout->addWidget(t2p8Button,8,2);
    showGridLayout->addWidget(t2p8Label,8,3);

    showGridLayout->addWidget(t1p9Button,9,0);
    showGridLayout->addWidget(t1p9Label,9,1);
    showGridLayout->addWidget(t2p9Button,9,2);
    showGridLayout->addWidget(t2p9Label,9,3);

    showGridLayout->addWidget(t1p10Button,10,0);
    showGridLayout->addWidget(t1p10Label,10,1);
    showGridLayout->addWidget(t2p10Button,10,2);
    showGridLayout->addWidget(t2p10Label,10,3);

    showGridLayout->addWidget(t1p11Button,11,0);
    showGridLayout->addWidget(t1p11Label,11,1);
    showGridLayout->addWidget(t2p11Button,11,2);
    showGridLayout->addWidget(t2p11Label,11,3);
    }
    showBox->setLayout(showGridLayout);

    QGroupBox *rightBox = new QGroupBox;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(controlBox);
    rightLayout->addWidget(showBox);
    rightBox->setLayout(rightLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(imgLabel);
    mainLayout->addWidget(rightBox);
    cenWidget->setLayout(mainLayout);

    //--- connect signals & slots ---
    QSignalMapper *signalMapper=new QSignalMapper(this);
    connect(controlButton,SIGNAL(clicked()),this,SLOT(controlButtonClicked()));
    connect(ballButton,SIGNAL(clicked()),this,SLOT(ballCorrect()));
    connect(t1p1Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p2Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p3Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p4Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p5Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p6Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p7Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p8Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p9Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p10Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t1p11Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p1Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p2Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p3Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p4Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p5Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p6Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p7Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p8Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p9Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p10Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    connect(t2p11Button,SIGNAL(clicked()),signalMapper,SLOT(map()));
    signalMapper->setMapping(t1p1Button,1);
    signalMapper->setMapping(t1p2Button,2);
    signalMapper->setMapping(t1p3Button,3);
    signalMapper->setMapping(t1p4Button,4);
    signalMapper->setMapping(t1p5Button,5);
    signalMapper->setMapping(t1p6Button,6);
    signalMapper->setMapping(t1p7Button,7);
    signalMapper->setMapping(t1p8Button,8);
    signalMapper->setMapping(t1p9Button,9);
    signalMapper->setMapping(t1p10Button,10);
    signalMapper->setMapping(t1p11Button,11);
    signalMapper->setMapping(t2p1Button,12);
    signalMapper->setMapping(t2p2Button,13);
    signalMapper->setMapping(t2p3Button,14);
    signalMapper->setMapping(t2p4Button,15);
    signalMapper->setMapping(t2p5Button,16);
    signalMapper->setMapping(t2p6Button,17);
    signalMapper->setMapping(t2p7Button,18);
    signalMapper->setMapping(t2p8Button,19);
    signalMapper->setMapping(t2p9Button,20);
    signalMapper->setMapping(t2p10Button,21);
    signalMapper->setMapping(t2p11Button,22);
    connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(playerCorrect(int)));
}

void MainWindow::initWidgets(){
    int frameStyle = QFrame::Sunken | QFrame::Panel;

    //--- text ---
    textLabel = new QLabel;
    textLabel->setFrameStyle(frameStyle);
    textLabel->setText("Running...\nYou can press 'Pause' to stop!");

    //--- start&pause ---
    controlButton = new QPushButton(tr("&Pause"));
    runningFlag = true;

    //--- ball ---
    ballButton = new QPushButton(tr("&Ball"));
    ballLabel = new QLabel;
    ballLabel->setFrameStyle(frameStyle);
    ballLabel->setText("x:0,y:0");

    //--- team-1 ---
    t1p1Button = new QPushButton(tr("&T1-P1"));
    t1p1Label = new QLabel;
    t1p1Label->setFrameStyle(frameStyle);
    t1p1Label->setText("x:0,y:0");

    t1p2Button = new QPushButton(tr("&T1-P2"));
    t1p2Label = new QLabel;
    t1p2Label->setFrameStyle(frameStyle);
    t1p2Label->setText("x:0,y:0");

    t1p3Button = new QPushButton(tr("&T1-P3"));
    t1p3Label = new QLabel;
    t1p3Label->setFrameStyle(frameStyle);
    t1p3Label->setText("x:0,y:0");

    t1p4Button = new QPushButton(tr("&T1-P4"));
    t1p4Label = new QLabel;
    t1p4Label->setFrameStyle(frameStyle);
    t1p4Label->setText("x:0,y:0");

    t1p5Button = new QPushButton(tr("&T1-P5"));
    t1p5Label = new QLabel;
    t1p5Label->setFrameStyle(frameStyle);
    t1p5Label->setText("x:0,y:0");

    t1p6Button = new QPushButton(tr("&T1-P6"));
    t1p6Label = new QLabel;
    t1p6Label->setFrameStyle(frameStyle);
    t1p6Label->setText("x:0,y:0");

    t1p7Button = new QPushButton(tr("&T1-P7"));
    t1p7Label = new QLabel;
    t1p7Label->setFrameStyle(frameStyle);
    t1p7Label->setText("x:0,y:0");

    t1p8Button = new QPushButton(tr("&T1-P8"));
    t1p8Label = new QLabel;
    t1p8Label->setFrameStyle(frameStyle);
    t1p8Label->setText("x:0,y:0");

    t1p9Button = new QPushButton(tr("&T1-P9"));
    t1p9Label = new QLabel;
    t1p9Label->setFrameStyle(frameStyle);
    t1p9Label->setText("x:0,y:0");

    t1p10Button = new QPushButton(tr("&T1-P10"));
    t1p10Label = new QLabel;
    t1p10Label->setFrameStyle(frameStyle);
    t1p10Label->setText("x:0,y:0");

    t1p11Button = new QPushButton(tr("&T1-P11"));
    t1p11Label = new QLabel;
    t1p11Label->setFrameStyle(frameStyle);
    t1p11Label->setText("x:0,y:0");

    //--- team-2 ---
    t2p1Button = new QPushButton(tr("&T2-P1"));
    t2p1Label = new QLabel;
    t2p1Label->setFrameStyle(frameStyle);
    t2p1Label->setText("x:0,y:0");

    t2p2Button = new QPushButton(tr("&T2-P2"));
    t2p2Label = new QLabel;
    t2p2Label->setFrameStyle(frameStyle);
    t2p2Label->setText("x:0,y:0");

    t2p3Button = new QPushButton(tr("&T2-P3"));
    t2p3Label = new QLabel;
    t2p3Label->setFrameStyle(frameStyle);
    t2p3Label->setText("x:0,y:0");

    t2p4Button = new QPushButton(tr("&T2-P4"));
    t2p4Label = new QLabel;
    t2p4Label->setFrameStyle(frameStyle);
    t2p4Label->setText("x:0,y:0");

    t2p5Button = new QPushButton(tr("&T2-P5"));
    t2p5Label = new QLabel;
    t2p5Label->setFrameStyle(frameStyle);
    t2p5Label->setText("x:0,y:0");

    t2p6Button = new QPushButton(tr("&T2-P6"));
    t2p6Label = new QLabel;
    t2p6Label->setFrameStyle(frameStyle);
    t2p6Label->setText("x:0,y:0");

    t2p7Button = new QPushButton(tr("&T2-P7"));
    t2p7Label = new QLabel;
    t2p7Label->setFrameStyle(frameStyle);
    t2p7Label->setText("x:0,y:0");

    t2p8Button = new QPushButton(tr("&T2-P8"));
    t2p8Label = new QLabel;
    t2p8Label->setFrameStyle(frameStyle);
    t2p8Label->setText("x:0,y:0");

    t2p9Button = new QPushButton(tr("&T2-P9"));
    t2p9Label = new QLabel;
    t2p9Label->setFrameStyle(frameStyle);
    t2p9Label->setText("x:0,y:0");

    t2p10Button = new QPushButton(tr("&T2-P10"));
    t2p10Label = new QLabel;
    t2p10Label->setFrameStyle(frameStyle);
    t2p10Label->setText("x:0,y:0");

    t2p11Button = new QPushButton(tr("&T2-P11"));
    t2p11Label = new QLabel;
    t2p11Label->setFrameStyle(frameStyle);
    t2p11Label->setText("x:0,y:0");

    //--- img ---
    imgLabel = new QLabel;
    imgLabel->resize(QSize(1017,648));
    imgLabel->setScaledContents(true);
}

void MainWindow::showImage(){
    //printf("press the button\n");
    cvtColor(cvImg,cvImg,CV_BGR2RGB);
    img = QImage((const unsigned char*)(cvImg.data),
                 cvImg.cols,cvImg.rows, cvImg.cols*cvImg.channels(),
                 QImage::Format_RGB888);
    imgLabel->clear();
    imgLabel->setPixmap(QPixmap::fromImage(img));
    img.scaled(imgLabel->x(),imgLabel->y());
}

void MainWindow::updatePosition(int *x,int *y){
    ballLabel->setText("x:"+QString::number(x[0],10)+",y:"+QString::number(y[0],10));

    t1p1Label->setText("x:"+QString::number(x[1],10)+",y:"+QString::number(y[1],10));
    t1p2Label->setText("x:"+QString::number(x[2],10)+",y:"+QString::number(y[2],10));
    t1p3Label->setText("x:"+QString::number(x[3],10)+",y:"+QString::number(y[3],10));
    t1p4Label->setText("x:"+QString::number(x[4],10)+",y:"+QString::number(y[4],10));
    t1p5Label->setText("x:"+QString::number(x[5],10)+",y:"+QString::number(y[5],10));
    t1p6Label->setText("x:"+QString::number(x[6],10)+",y:"+QString::number(y[6],10));
    t1p7Label->setText("x:"+QString::number(x[7],10)+",y:"+QString::number(y[7],10));
    t1p8Label->setText("x:"+QString::number(x[8],10)+",y:"+QString::number(y[8],10));
    t1p9Label->setText("x:"+QString::number(x[9],10)+",y:"+QString::number(y[9],10));
    t1p10Label->setText("x:"+QString::number(x[10],10)+",y:"+QString::number(y[10],10));
    t1p11Label->setText("x:"+QString::number(x[11],10)+",y:"+QString::number(y[11],10));

    t2p1Label->setText("x:"+QString::number(x[12],10)+",y:"+QString::number(y[12],10));
    t2p2Label->setText("x:"+QString::number(x[13],10)+",y:"+QString::number(y[13],10));
    t2p3Label->setText("x:"+QString::number(x[14],10)+",y:"+QString::number(y[14],10));
    t2p4Label->setText("x:"+QString::number(x[15],10)+",y:"+QString::number(y[15],10));
    t2p5Label->setText("x:"+QString::number(x[16],10)+",y:"+QString::number(y[16],10));
    t2p6Label->setText("x:"+QString::number(x[17],10)+",y:"+QString::number(y[17],10));
    t2p7Label->setText("x:"+QString::number(x[18],10)+",y:"+QString::number(y[18],10));
    t2p8Label->setText("x:"+QString::number(x[19],10)+",y:"+QString::number(y[19],10));
    t2p9Label->setText("x:"+QString::number(x[20],10)+",y:"+QString::number(y[20],10));
    t2p10Label->setText("x:"+QString::number(x[21],10)+",y:"+QString::number(y[21],10));
    t2p11Label->setText("x:"+QString::number(x[22],10)+",y:"+QString::number(y[22],10));
}

bool MainWindow::isRunning(){
    return this->runningFlag;
}

void MainWindow::controlButtonClicked(){
    if(runningFlag){
        textLabel->setText("Pause...\nYou can edit the tracking results\nor press 'Start' to continue!");
        controlButton->setText("Start");
        runningFlag = false;
    }
    else{
        textLabel->setText("Running...\nYou can press 'Pause' to stop!");
        controlButton->setText("Pause");
        runningFlag = true;
    }
}

void MainWindow::ballCorrect(){
    if(!runningFlag){
        int x,y;
        reSelect(cvImg,currentIndex,x,y,0);
        ballLabel->setText("x:"+QString::number(x,10)+",y:"+QString::number(y,10));
    }
}

QLabel *MainWindow::PlayerToLabel(int playerIndex)
{
    switch(playerIndex)
    {
    case 1:return t1p1Label;
    case 2:return t1p2Label;
    case 3:return t1p3Label;
    case 4:return t1p4Label;
    case 5:return t1p5Label;
    case 6:return t1p6Label;
    case 7:return t1p7Label;
    case 8:return t1p8Label;
    case 9:return t1p9Label;
    case 10:return t1p10Label;
    case 11:return t1p11Label;
    case 12:return t2p1Label;
    case 13:return t2p2Label;
    case 14:return t2p3Label;
    case 15:return t2p4Label;
    case 16:return t2p5Label;
    case 17:return t2p6Label;
    case 18:return t2p7Label;
    case 19:return t2p8Label;
    case 20:return t2p9Label;
    case 21:return t2p10Label;
    case 22:return t2p11Label;
    }
    return NULL;
}

void MainWindow::playerCorrect(int playerIndex)
{
    if(!runningFlag)
    {
        int x,y;
        reSelect(cvImg,currentIndex,x,y,playerIndex);
        QLabel *playerLabel=PlayerToLabel(playerIndex);
        playerLabel->setText("x:"+QString::number(x,10)+",y:"+QString::number(y,10));
    }
}
