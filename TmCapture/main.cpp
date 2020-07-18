#include "mainwindow.h"

#include <QApplication>

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
}

#ifdef Q_OS_WIN
#include <windows.h>
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    CoInitialize(nullptr);
#endif
    /* Init ffmpeg */
    avdevice_register_all();


    MainWindow w;
    w.show();

    int ret = a.exec();

#ifdef Q_OS_WIN
   CoUninitialize();
#endif
    return ret;
}
