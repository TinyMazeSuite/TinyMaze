#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QRegularExpression>

#include "ffmpegutil.h"

extern "C"
{
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void on_pushButtonUpdateCamera_clicked();
    void on_pushButtonPreviewOnly_clicked();
    void on_pushButtonRecord_clicked();
protected slots:
    void onCaptureDeviceChanged(QStringList deviceLists);
private:
    Ui::MainWindow *ui;
    FFmpegUtil* ffUtil;
};
#endif // MAINWINDOW_H
