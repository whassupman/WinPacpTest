#include "base.h"
#include "ui_base.h"

Base::Base(QWidget *parent) :
    QWidget(parent),ui(new Ui::Base)
{
    ui->setupUi(this);
}

Base::~Base()
{
    delete ui;
    delete config;
}

/**
 * @brief Base::configWindow 对窗口进行配置
 */
void Base::configWindow()
{
    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
//    QPixmap icon  = style()->standardPixmap(QStyle::SP_DriveNetIcon);
    this->setWindowIcon(QIcon(":/Icon/48.ico"));
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle("网络共享");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(this->size());

    //获取界面的宽度和高度
    int width = this->width();
    int height = this->height();
    //qDebug()<<width<<height;

    //固定窗口大小
    this->setFixedSize(width,height);

    //设置背景
    QString picpath = QDir::currentPath()+"/bg.jpg";
    QString picpath2 = QDir::currentPath()+"/bg.jpeg";
    QPixmap pixma(picpath);
    if(pixma.isNull())
        pixma.load(picpath2);
    if(!pixma.isNull())
    {
        QPixmap pixmap=pixma.scaled(width,height);
        //qDebug()<<pixmap;
        QPalette   palette;
        palette.setBrush(QPalette::Window,QBrush(pixmap));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }

    //构建最小化、最大化、关闭按钮
    minButton = new QToolButton(this);
    closeButton= new QToolButton(this);
    //获取最小化、关闭按钮图标
    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    //设置最小化、关闭按钮图标
    minButton->setIcon(minPix);
    closeButton->setIcon(closePix);
    //设置最小化、关闭按钮在界面的位置
    minButton->setGeometry(width-46,5,20,20);
    closeButton->setGeometry(width-25,5,20,20);
    //设置鼠标移至按钮上的提示信息
    minButton->setToolTip(tr("最小化"));
    closeButton->setToolTip(tr("关闭"));
    //设置最小化、关闭按钮的样式
#ifdef Q_OS_WIN32
    minButton->setStyleSheet("background-color:transparent;");
    closeButton->setStyleSheet("background-color:transparent;");
#endif
    //关联最小化、关闭按钮的槽函数
    connect(minButton, SIGNAL(clicked()), this, SLOT(minWindow()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

}
/**
 * @brief Base::initialize
 */
void Base::initialize(){
#ifdef Q_OS_WIN32
    confPath = "libsob.dll";
#elif defined(Q_OS_LINUX)
    QString ppath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
    confPath = ppath+"/NetworkClient/libsob.so";
#endif
    config = new QSettings(confPath, QSettings::IniFormat);
}

//鼠标事件处理，用来移动窗口
void Base::mousePressEvent(QMouseEvent *e){
    last = e->globalPos();
}

void Base::mouseMoveEvent(QMouseEvent *e){
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    if(dx>50)
        dx=0;
    if(dy>50)
        dy=0;
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void Base::mouseReleaseEvent(QMouseEvent *e){
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    if(dx>50)
        dx=9;
    if(dy>50)
        dy=9;
    move(x()+dx, y()+dy);
}

/**
 * @brief Base::minWindow 最小化
 */
void Base::minWindow()
{
    this->showMinimized();
}
