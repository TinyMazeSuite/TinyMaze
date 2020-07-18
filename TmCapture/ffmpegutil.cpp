#include "ffmpegutil.h"
#include <QFile>
#include <QDir>
#include <QRegularExpression>

extern "C"
{
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
#include "libavutil/samplefmt.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"

}

static FFmpegUtil* currentFFmpegUtil = nullptr;
static bool currentEnumVideo = false;

static QStringList currentRawVideoDevices;

void FFmpegUtil::_ffmpegutil_hook_log(void *avcl, int level, const char * fmt, va_list vl)
{
    AVClass* avc = avcl ? *(AVClass **) avcl : NULL;
    QString class_name(avc->item_name(avcl));

    if(class_name=="dshow" && level == AV_LOG_INFO)
    {
        QString qfmt(fmt);

        if(qfmt == "DirectShow video devices (some may be both video and audio devices)\n")
        {
            currentEnumVideo = true;
            return;
        }

        if(qfmt == "DirectShow audio devices\n")
        {
            currentEnumVideo = false;
            return;
        }

        if(currentEnumVideo)
        {
            if(qfmt==" \"%s\"\n")
            {
                QString name = QString::vasprintf(fmt, vl);
                currentRawVideoDevices.push_back(name.trimmed());
            }
        }
    }

}

FFmpegUtil::FFmpegUtil(QObject *parent)
    : QObject(parent)
{
    avdevice_register_all();

    updateVideoCaptureDevice();
}

const QString FFmpegUtil::getFFmpegVersion()
{
    return ffmpegVersionReadOnly;
}

const QStringList FFmpegUtil::getFFmpegCaptureDeviceList()
{
    return ffmpegCaptureDeviceListReadOnly;
}

void FFmpegUtil::updateVideoCaptureDevice()
{
    int result = 0;

    AVFormatContext* pFmtCtx = avformat_alloc_context();
    AVInputFormat* inFmt = av_find_input_format("dshow");

    AVDictionary* options = NULL;
    av_dict_set(&options,"list_devices","true",0);

    /* hook log */
    av_log_set_callback(_ffmpegutil_hook_log);

    currentFFmpegUtil = this;

    currentRawVideoDevices.clear();
    ffmpegCaptureDeviceListReadOnly.clear();

    result = avformat_open_input(&pFmtCtx, "video=dummy", inFmt, &options);

    {
        QMap<QString, int> deviceNum;
        Q_FOREACH(const QString& d , currentRawVideoDevices)
        {
            if(deviceNum.count(d))
            {
                deviceNum[d] = (deviceNum[d] +1);
            }else{
                deviceNum[d] = 0;
            }

            ffmpegCaptureDeviceListReadOnly.push_back(QString("%1 #%2").arg(d).arg(deviceNum[d]));
        }
    }

    emit this->ffmpegCaptureDeviceListChanged(ffmpegCaptureDeviceListReadOnly);

    currentFFmpegUtil = nullptr;

    /* set it back */
    av_log_set_callback(av_log_default_callback);

    /* clean */
    avformat_close_input(&pFmtCtx);
    avformat_free_context(pFmtCtx);
    av_dict_free(&options);
}

#include <QInputDialog>

void FFmpegUtil::startPreview(QString deviceName)
{
    QStringList args;

    QRegularExpression nameRx("^(.+) #(\\d+)$");

    auto m = nameRx.match(deviceName);

    if(m.hasMatch())
    {
        args << "-y"; /*over write file*/
        args << "-rtbufsize" << "8M";
        args << "-f" << "dshow";
        args << "-s" << "800x600";
        args << "-video_device_number" << m.captured(2);
        args << "-i" << QString("video=\"%1\"").arg(m.captured(1));
        args << "-filter_complex" << "\"[0:v]realtime,format=yuv420p\"";
        args << "-f" << "sdl" << QString("\"%1 Preview\"").arg(deviceName);

        //ffProcess->close();
        //ffProcess->reset();

    }

}




