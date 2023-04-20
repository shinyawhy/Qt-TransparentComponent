#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transparentcomponent.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* w = new QWidget(this);
    this->setCentralWidget(w);

    auto vb = new QVBoxLayout(w);
    vb->setSpacing(0);
    auto aw1 = new AccordionZoomWidget;
    auto aw2 = new AccordionZoomWidget;
    aw1->setTitle("标题1");
    aw1->setText("黄河之水天上来，奔流到海不复回。高堂明镜悲白发，朝如青丝暮成雪。古来圣贤皆寂寞，惟有饮者留其名。");
    aw2->setTitle("标题2");
    aw2->setText("黄河之水天上来，奔流到海不复回。高堂明镜悲白发，朝如青丝暮成雪。古来圣贤皆寂寞，惟有饮者留其名。陈王昔时宴平乐，斗酒十千恣欢谑。");
    vb->addWidget(aw1);
    vb->addWidget(aw2);
    vb->addStretch();
    w->resize(500,400);
    auto palette = w->palette();
    palette.setBrush(QPalette::Window,QGradient(QGradient::OldHat));
    w->setPalette(palette);
    w->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

