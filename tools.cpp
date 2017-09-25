#include "tools.h"

Tools::Tools()
{

}

/**
 * @brief Base::showTip 显示提示信息
 * @param title 窗口标题
 * @param tip 信息
 */
void Tools::showInfo(QString title,QString tip){
    QMessageBox message(QMessageBox::Information, title, tip,QMessageBox::Yes);
    message.setButtonText(QMessageBox::Yes,QString("确 定"));
    message.exec();
}

/**
 * @brief Base::showWarning 显示警告信息
 * @param title 窗口标题
 * @param tip 信息
 */
void Tools::showWarning(QString title,QString tip){
    QMessageBox message(QMessageBox::Warning, title, tip,QMessageBox::Yes);
    message.setButtonText(QMessageBox::Yes,QString("确 定"));
    message.exec();
}

/**
 * @brief Base::question 根据被点击的按钮，返回true或者false
 * @param title 窗口标题
 * @param content 提问内容
 * @return 同意返回true，不同意返回false
 */
bool Tools::question(QString title, QString content){
    QMessageBox msg(QMessageBox::Question,title,content,QMessageBox::Yes | QMessageBox::No);
    msg.setEscapeButton(msg.button(QMessageBox::Cancel));
    msg.setButtonText(QMessageBox::Yes,"确 定");
    msg.setButtonText(QMessageBox::No,"取 消");
    msg.exec();
    if (msg.clickedButton() == msg.button(QMessageBox::Yes)) {
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief Base::question 根据被点击的按钮，返回true或者false
 * @param buttonYesText 按钮Yes的文本
 * @param buttonNoText 按钮No的文本
 * @param title 窗口标题
 * @param content 提问内容
 * @return 当buttonYesText对应按钮被点击时，返回ButtonYes，
 * 当buttonNoText对应按钮被点击时，返回ButtonNo,否则返回ButtonCancel
 */
Tools::ButtonClicked Tools::question(QString buttonYesText, QString buttonNoText, QString buttonCancelText,QString title, QString content){
    QMessageBox msg(QMessageBox::Question,title,content,QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msg.setEscapeButton(msg.button(QMessageBox::Cancel));
    msg.setButtonText(QMessageBox::Yes,buttonYesText);
    msg.setButtonText(QMessageBox::No,buttonNoText);
    msg.setButtonText(QMessageBox::Cancel,buttonCancelText);
    msg.exec();
    if (msg.clickedButton() == msg.button(QMessageBox::Yes)) {
        return ButtonYes; //返回确认
    }
    else if(msg.clickedButton() == msg.button(QMessageBox::No)){
        return ButtonNo;
    }
    else
    {
        return ButtonCancel;
    }
}

/**
 * @brief Base::showError 显示错误信息
 * @param title 标题
 * @param tip 信息
 */
void Tools::showError(QString title, QString tip){
    QMessageBox message(QMessageBox::Critical, title, tip,QMessageBox::Yes);
    message.setButtonText(QMessageBox::Yes,QString("确 定"));
    message.exec();
}

void Tools::log(QString msg){

#ifdef Q_OS_WIN32
    QString logPath = "run.log";
#elif defined(Q_OS_LINUX)
    QString ppath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    logPath = ppath+"/NetworkClient/run.log";
#endif
    QFile logFile(logPath);
    if(!logFile.open(QIODevice::Append)){
        showError("提示","打开日志文件失败");
        return;
    }else{
        QTextStream logTextStream(&logFile);
        logTextStream<<msg;
        logTextStream.flush();
    }
    logFile.close();
}
