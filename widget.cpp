#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    packetCapture = new PacketsCapture();
    connect(packetCapture,SIGNAL(devivicesFounded(QList<DeviceInfo>)),
            this,SLOT(refreshDeivices(QList<DeviceInfo>)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pbStart_clicked()
{
    packetCapture->start(ui->cbDevices->currentIndex());
}

void Widget::on_pbStop_clicked()
{
    packetCapture->stop();
}

void Widget::on_pbRefresh_clicked()
{
    packetCapture->findDevices();
}

void Widget::refreshDeivices(QList<DeviceInfo> devices)
{
    mDebug("");
    ui->cbDevices->clear();
    foreach (DeviceInfo d, devices) {
        ui->cbDevices->addItem(QString(d.description));
    }
}
