#ifndef PACKETSCAPTURE_H
#define PACKETSCAPTURE_H

#include <QObject>
#include <QDateTime>
#include <QThread>
#include <QList>

#include "tools.h"
#include "headers.h"

#define HAVE_REMOTE
#include <pcap.h>

QString ucharToHexString(const u_char data[]);
void packetHandler(u_char *user, const struct pcap_pkthdr *header, const u_char *pkt_data);

class PacketsCapture : public QObject
{
    Q_OBJECT

public:
    typedef struct{
        QString name;
        QString description;
        u_int netmask;
    }DeviceInfo;

    explicit PacketsCapture(QObject *parent = nullptr);
    ~PacketsCapture();
    bool findDevices();                     //发现网卡设备
    bool start(int index);                  //开始捕获数据包
    bool start(int index, char filter[]);   //开始捕获数据包
    void stop();                            //停止捕获数据包
    static void printHexString(u_int8_t data);
    static void printHexString(u_int16_t data);
    static void printHexString(u_int8_t data[], u_int8_t length);

private:
    pcap_t *handle;
    QList<DeviceInfo> devices;
signals:
    void devivicesFounded(QList<DeviceInfo> devices);     //发现设备
    void newPacket(QByteArray bytes);

public slots:

};



class CaptureThread : public QThread
{
    Q_OBJECT
private:
    pcap_t *handle;
public:
    CaptureThread(pcap_t *adhandle){
        handle = adhandle;
    }
protected:
    void run() override {
        QString result;
        pcap_loop(handle, 0, packetHandler, NULL);
        mDebug("Capture finished");
//        emit resultReady(result);
    }
signals:
    void resultReady(const QString &s);
};

#endif // PACKETSCAPTURE_H
