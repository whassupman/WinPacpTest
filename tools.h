#ifndef TOOLS_H
#define TOOLS_H

#define DEBUG

#include <QObject>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>

//显示调试信息的宏
#ifdef DEBUG
#define mDebug(x)     (qDebug()    << "Debug   " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" << x)
#define mWarning(x)   (qWarning()  << "Warning " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" << x)
#define mError(x)     (qCritical() << "Error   " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" << x)
#define mInfo(x)      (qInfo()     << "Infor   " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" << x)
#else
#define mDebug(x)     (Tools::log("Debug   " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x))
#define mWarning(x)   (Tools::log("Warning " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x))
#define mError(x)     (Tools::log("Error   " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x))
#define mInfo(x)      (Tools::log("Infor   " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz ") + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x))
#endif

#define showD(x,y) (Tools::showInfo(x,y))
#define showW(x,y) (Tools::showWarning(x,y))
#define showE(x,y) (Tools::showError(x,y))

class Tools
{

public:
    explicit Tools();

    //实用函数
    enum ButtonClicked{ButtonYes,ButtonNo,ButtonCancel};
    static void showInfo(QString title, QString tip);           //显示信息
    static void showWarning(QString title, QString tip);        //显示警告
    static void showError(QString title, QString tip);          //显示错误
    static bool question(QString title, QString content);       //显示提问窗口
    static ButtonClicked question(QString buttonYesText, QString buttonNoText,
                                  QString buttonCancelText, QString title, QString content); //显示提问窗口

    //日志记录
    static void log(QString msg);

signals:

public slots:
};

#endif // TOOLS_H
