//------------------------------------
// Created by Linghan 2017-01-06
//------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QLabel;
class QGroupBox;
QT_END_NAMESPACE

#include <opencv/cv.h>
using namespace  cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Mat cvImg;
    int currentIndex;

public:
    MainWindow();
    void showImage();
    void updatePosition(int *x,int *y);
    bool isRunning();

private:
    QWidget *cenWidget;
    QLabel *textLabel;
    QPushButton *controlButton;

    QPushButton *ballButton;
    QLabel *ballLabel;

    QPushButton *t1p1Button,*t1p2Button,*t1p3Button,
                *t1p4Button,*t1p5Button,*t1p6Button,
                *t1p7Button,*t1p8Button,*t1p9Button,
                *t1p10Button,*t1p11Button;
    QLabel *t1p1Label,*t1p2Label,*t1p3Label,*t1p4Label,
           *t1p5Label,*t1p6Label,*t1p7Label,*t1p8Label,
           *t1p9Label,*t1p10Label,*t1p11Label;

    QPushButton *t2p1Button,*t2p2Button,*t2p3Button,
                *t2p4Button,*t2p5Button,*t2p6Button,
                *t2p7Button,*t2p8Button,*t2p9Button,
                *t2p10Button,*t2p11Button;
    QLabel *t2p1Label,*t2p2Label,*t2p3Label,*t2p4Label,
           *t2p5Label,*t2p6Label,*t2p7Label,*t2p8Label,
           *t2p9Label,*t2p10Label,*t2p11Label;

    QLabel *imgLabel;
    QImage img;

    bool runningFlag;

private:
    void initWidgets();

/*public slots:
    void setPosition();*/

private slots:
    void controlButtonClicked();
    void ballCorrect();
    QLabel *PlayerToLabel(int);
    void playerCorrect(int);
};

#endif // MAINWINDOW_H
