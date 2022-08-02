#include "dropwidget.h"

DropWidget::DropWidget(QWidget *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);
    setCursor(Qt::PointingHandCursor);
    setStyleSheet(defaultStyleSheet);
    setMovie(&movie);

    movie.setCacheMode(QMovie::CacheAll);
    movie.setFileName(R"(C:\Users\mothe\Documents\QtProject\FunScreen\Resources\Upload.gif)");
    movie.start();
}

void DropWidget::setSpeedMovie(int value)
{
    movie.setSpeed(value);
}

void DropWidget::dropEvent(QDropEvent *event)
{
    fileName = event->mimeData()->text();
    fileName = fileName.right(fileName.length() - 8);
    isExistFile = true;

    setStyleSheet(defaultStyleSheet);
    setScaledContents(true);

    movie.stop();
    movie.setSpeed(movie.speed() * 2);
    movie.setFileName(fileName);
    movie.start();    

    event->acceptProposedAction();
}

void DropWidget::dragEnterEvent(QDragEnterEvent *event)
{

    QString fileName = event->mimeData()->text();
    if (fileName.right(4) == ".gif")
    {
        movie.setSpeed(movie.speed() / 2);
        setStyleSheet(alternativeStyleSheet);
        event->acceptProposedAction();
    }
}

void DropWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    setStyleSheet(defaultStyleSheet);
    movie.setSpeed(movie.speed() * 2);
    event->accept();
}

void DropWidget::mousePressEvent(QMouseEvent *event)
{
    if(isExistFile)
        emit clickedToCreate(fileName, movie.speed());
    event->accept();
}

