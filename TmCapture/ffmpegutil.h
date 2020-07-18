#ifndef FFMPEGUTIL_H
#define FFMPEGUTIL_H

#include <QObject>
#include <QProcess>

class FFmpegUtil : public QObject
{
    Q_OBJECT
public:
    explicit FFmpegUtil(QObject *parent = nullptr);
public:
    Q_PROPERTY(QString ffmpegVersion MEMBER ffmpegVersionReadOnly READ getFFmpegVersion)
    Q_PROPERTY(QStringList ffmpegCaptureDeviceList MEMBER ffmpegCaptureDeviceListReadOnly READ getFFmpegCaptureDeviceList NOTIFY ffmpegCaptureDeviceListChanged)
public:
    const QString getFFmpegVersion();
    const QStringList getFFmpegCaptureDeviceList();
public slots:
    void updateVideoCaptureDevice();

    void startPreview(QString deviceName);
    //void startRecord(QStringList args);

protected:
    QString ffmpegVersionReadOnly;
    QStringList ffmpegCaptureDeviceListReadOnly;
signals:
    void ffmpegCaptureDeviceListChanged(QStringList deviceList);
protected:
    static void _ffmpegutil_hook_log(void *avcl, int level, const char * fmt, va_list vl);
};

#endif // FFMPEGUTIL_H
