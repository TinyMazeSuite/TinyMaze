#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ffUtil(new FFmpegUtil(this))
{
    ui->setupUi(this);

    connect(ffUtil,&FFmpegUtil::ffmpegCaptureDeviceListChanged, this, &MainWindow::onCaptureDeviceChanged);

    setWindowTitle(QString("TinyMaze - Capture [ Run with FFmpeg @ %1 ]").arg(ffUtil->getFFmpegVersion()));

    ui->listWidgetDevices->addItems(ffUtil->getFFmpegCaptureDeviceList());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onCaptureDeviceChanged(QStringList deviceLists)
{
    ui->listWidgetDevices->clear();
    ui->listWidgetDevices->addItems(deviceLists);
}

void MainWindow::on_pushButtonUpdateCamera_clicked()
{
    ffUtil->updateVideoCaptureDevice();
}

void MainWindow::on_pushButtonPreviewOnly_clicked()
{
    if(ui->listWidgetDevices->selectedItems().count()==1)
    {
        ffUtil->startPreview(ui->listWidgetDevices->selectedItems().front()->text());
    }
}

void MainWindow::on_pushButtonRecord_clicked()
{

}






