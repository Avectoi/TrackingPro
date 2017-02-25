#-------------------------------------------------
#
# Project created by QtCreator 2017-02-20T21:10:05
#
#-------------------------------------------------

QT       += core gui
QT+=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TRY2
TEMPLATE = app


SOURCES += main.cpp\
    function.cpp \
    mainwindow.cpp \
    function.cpp \
    function.cpp

HEADERS  += \
    function.h \
    mainwindow.h \
    function.h

FORMS    += \
    ../TRY2/mainwindow.ui


INCLUDEPATH+= C:\opencv\build\include \
        C:\opencv\build\include\opencv \
        C:\opencv\build\include\opencv2


LIBS+=C:\opencv\build\x86\vc12\lib\opencv_ml249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_calib3d249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_contrib249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_core249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_features2d249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_flann249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_gpu249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_highgui249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_imgproc249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_legacy249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_objdetect249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_ts249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_video249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_nonfree249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_ocl249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_photo249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_stitching249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_superres249d.lib
LIBS+=C:\opencv\build\x86\vc12\lib\opencv_videostab249d.lib
