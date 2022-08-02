#include "backgroundwidget.h"

BackgroundWidget::BackgroundWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowState(Qt::WindowFullScreen);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::CoverWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->move(0, 0);

    /*label.setParent(this);
    label.setGeometry(100, 100, 50, 50);
    label.setStyleSheet("background-color:white;border-style: solid; border-width: 1.5px; border-radius: 10px;");
    label.setText("1");
    label.setAlignment(Qt::AlignCenter);
    label.setFont(QFont("Segoe UI", 20));
    label.show();*/
}

void BackgroundWidget::addAnimatedLabel(QString fileName, int speed)
{
    QPointer<AnimatedLabel> newLabel = new AnimatedLabel(this);
    newLabel->setAttribute(Qt::WA_DeleteOnClose);
    newLabel->set(fileName, size(), speed);
    newLabel->show();
    labels.append(newLabel);
}

QRect BackgroundWidget::getRectFromLabel(int index)
{
    return labels[index]->getRect();
}

void BackgroundWidget::setAnimationForLabel(int index, const QList<QRect> &rects, const QList<int> &durations)
{
    labels[index]->setSequentialAnimation(rects, durations);
}

void BackgroundWidget::startAnimationForLabel(int index, int loopCount)
{
    labels[index]->startSequentialAnimation(loopCount);
}
