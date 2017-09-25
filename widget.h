#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "packetscapture.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    typedef PacketsCapture::DeviceInfo DeviceInfo;

public:    
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_pbRefresh_clicked();

    void refreshDeivices(QList<DeviceInfo> devices);

private:
    Ui::Widget *ui;
    PacketsCapture *packetCapture;
};

#endif // WIDGET_H
