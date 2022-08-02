#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bw = new BackgroundWidget;
    dw = new DropWidget(this);
    connect(dw, &DropWidget::clickedToCreate,
            this, &MainWindow::onClickedToCreate);

    dw->setGeometry(10,10, 350, 350);
    dw->show();
    bw->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClickedToCreate(const QString &fileName, int speed)
{
    bw->addAnimatedLabel(fileName, speed);
    ui->comboBox->addItem(fileName);
    ui->comboBox->setCurrentIndex(ui->comboBox->count() - 1);
    rects.clear();
    durations.clear();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    dw->setSpeedMovie(50 + value * 1.5);
}


void MainWindow::on_addAnimationButton_released()
{
    qDebug() << bw->getRectFromLabel(ui->comboBox->currentIndex());
    rects.append(bw->getRectFromLabel(ui->comboBox->currentIndex()));
    durations.append(5000);
}


void MainWindow::on_startAnimationButton_released()
{
    bw->setAnimationForLabel(0,rects, durations);
    bw->startAnimationForLabel(0, 1);
}


void MainWindow::on_stopAnimationButton_released()
{

}

