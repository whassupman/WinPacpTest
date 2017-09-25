#ifndef BASE_H
#define BASE_H

#include <QWidget>
#include <QToolButton>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QSettings>
#include <QException>
#include <QDesktopWidget>
#include <QDir>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QStyle>
#include <QApplication>
#include <QEventLoop>

namespace Ui {
class Base; //y一个应用程序的基类，包含许多有用的函数
}

class Base : public QWidget
{
    Q_OBJECT

public:

    explicit Base(QWidget *parent = 0);
    ~Base();

    //配置文件
    QSettings *config;              //配置文件读写指针
    QString confPath;               //configration file path

    //UI配置
    QPoint last;                    //窗口最后的坐标
    QToolButton *minButton;         //最小化按钮
    QToolButton *closeButton;       //关闭按钮
    virtual void configWindow();
    virtual void initialize();      //初始化函数

private:
    Ui::Base *ui;

public slots:
    void minWindow(); //最小化窗口

protected:
    //与窗口移动相关
    void mousePressEvent (QMouseEvent *e);
    void mouseMoveEvent (QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // BASE_H
