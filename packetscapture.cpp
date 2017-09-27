#include "packetscapture.h"

#define printHex(x) (PacketsCapture::printHexString(x))
#define printHex16(x) (PacketsCapture::printHexString(x))
#define printHexArray(x,y) (PacketsCapture::printHexString(x,y))

void packetHandler(u_char *user, const pcap_pkthdr *header, const u_char *pkt_data)
{
//    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    mDebug(QString("Length:%1, CaptureLength:%2")
          .arg(header->len).arg(header->caplen));
    //复制数据到临时变量中
    u_char *data = new u_char[header->caplen];
    memcpy(data,pkt_data,sizeof(u_char)*header->caplen);
    //交换EthernetHeader中type字段的两字节
    u_char tmp;
    tmp = data[12];
    data[12] = data[13];
    data[13] = tmp;
    //交换EapolHeader中length字段的两字节
    tmp = data[16];
    data[16] = data[17];
    data[17] = tmp;
    //交换EapHeader中length字段的两字节
    tmp = data[20];
    data[20] = data[21];
    data[21] = tmp;
    //打印包的信息
    EapolPacket *p = (EapolPacket *)data;
    printHexArray(p->ethernetHeader.dst,6);
    printHexArray(p->ethernetHeader.src,6);
    printHex16(p->ethernetHeader.type);
    printHex(p->eapolHeader.version);
    printHex(p->eapolHeader.type);
    printHex16(p->eapolHeader.length);
    if(p->eapolHeader.type == Eap_Packet){
        printHex(p->eapHeader.code);
        printHex(p->eapHeader.identifier);
        printHex16(p->eapHeader.length);
    }
}

PacketsCapture::PacketsCapture(QObject *parent) : QObject(parent)
{
    handle = NULL;
}

PacketsCapture::~PacketsCapture()
{

}

bool PacketsCapture::findDevices()
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];

    /* 获取本机设备列表 */
    if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        mDebug("Error in pcap_findalldevs: %s"+QString(errbuf));
        return false;
    }

    if(alldevs == NULL)
    {
        mDebug("No interfaces found! Make sure WinPcap is installed.");
        return -1;
    }

    /* 打印列表 */
    devices.clear();
    DeviceInfo info;
    for(d=alldevs;d; d=d->next)
    {
//        mDebug(QString("Name:%1").arg(d->name));
//        if (d->description)
//            mDebug(QString(" (%1)").arg(d->description));
//        else
//            mDebug(QString(" (No description available)"));
        info.name = QString(d->name);
        info.description = QString(d->description);
        if(d->addresses != NULL)
                /* 获得接口第一个地址的掩码 */
                info.netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
            else
                /* 如果接口没有地址，那么我们假设一个C类的掩码 */
                info.netmask=0xffffff;
        devices.append(info);
//        mDebug(devices.last().description);
    }
    /* 释放设备列表 */
    pcap_freealldevs(alldevs);
    //将设备列表发送给主程序
    emit devivicesFounded(devices);
    return true;
}

bool PacketsCapture::start(int index)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *adhandle;
    if((adhandle= pcap_open(devices[index].name.toStdString().c_str(),          // 设备名
                              65535,            // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
                              PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
                              1000,             // 读取超时时间
                              NULL,             // 远程机器验证
                              errbuf            // 错误缓冲池
                             ) ) == NULL)
    {
        mDebug(QString("Unable to open the adapter. %1 is not supported by WinPcap").arg(devices[index].name));
        return false;
    }else{
        if(pcap_datalink(adhandle) != DLT_EN10MB){
            showD("警告",QString("This program works only on Ethernet networks."));
            pcap_close(adhandle);
            return false;
        }else{
            mDebug(QString("listening on %1...\n").arg(devices[index].description));
            /* 开始捕获 */
            (new CaptureThread(adhandle))->start();
            handle = adhandle;
            mDebug("Start to capture data");
            return true;
        }
    }
}

bool PacketsCapture::start(int index, char filter[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fcode;
    pcap_t *adhandle;
    if((adhandle= pcap_open(devices[index].name.toStdString().c_str(),          // 设备名
                              65535,            // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
                              PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
                              1000,             // 读取超时时间
                              NULL,             // 远程机器验证
                              errbuf            // 错误缓冲池
                             ) ) == NULL)
    {
        mDebug(QString("Unable to open the adapter. %1 is not supported by WinPcap").arg(devices[index].name));
        return false;
    }else{
        if(pcap_datalink(adhandle) != DLT_EN10MB){
            showD("警告",QString("This program works only on Ethernet networks."));
            pcap_close(adhandle);
            return false;
        }else{
            //编译过滤器
            if (pcap_compile(adhandle, &fcode, filter, 1, devices[index].netmask) <0 )
            {
                fprintf(stderr,"Unable to compile the packet filter. Check the syntax.");
                return false;
            }
            //设置过滤器
            if (pcap_setfilter(adhandle, &fcode)<0)
            {
                mDebug("Error setting the filter.");
                return false;
            }
            mDebug(QString("listening on %1...\n").arg(devices[index].description));
            /* 开始捕获 */
            (new CaptureThread(adhandle))->start();
            handle = adhandle;
            mDebug("Start to capture data");
            return true;
        }
    }
}

void PacketsCapture::stop()
{
    if(handle)
        pcap_breakloop(handle);
    else
        showD("Infomation","Capture is already stopped");
    handle = NULL;
}

void PacketsCapture::printHexString(u_int8_t data)
{
    mDebug(QString::asprintf("%x",data));
}

void PacketsCapture::printHexString(u_int16_t data)
{
    mDebug(QString::asprintf("%x",data));
}

void PacketsCapture::printHexString(u_int8_t data[],u_int8_t length)
{
    QString s = "";
    for(uint i=0;i<length;i++){
        s += QString::asprintf("%x ",data[i]);
    }
    mDebug(s);
}
