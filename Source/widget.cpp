#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::CoverWindow | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
     //this->setWindowFlags(Qt::X11BypassWindowManagerHint);
    this->move(0, 0);

    label = new QLabel(this);
    movie = new QMovie(label);
    animationsGroup = new QSequentialAnimationGroup(this);
    parallelAnimationsGroup = new QParallelAnimationGroup(this);

    label->setScaledContents(true);
    movie-> setCacheMode(QMovie::CacheAll);

    set(":/new/prefix1/7.gif");
}


Widget::~Widget()
{
    delete ui;
}

void Widget::set(QString fileName)
{
    setMovie(fileName);
}

void Widget::setSize(QSize size)
{
    animationsGroup->clear();
    if(size.height() < this->size().height() * max &&
            size.width() < this->size().width() * max &&
            size.height() > this->size().height() * min &&
            size.width() > this->size().width() * min) {
        QRect targetGeometry = QRect(QPoint(label->pos().x() + label->rect().center().x() - size.width() / 2,
                                            label->pos().y() + label->rect().center().y() - size.height() / 2),
                                     size);
        addAnimation("geometry", 20, label->geometry(), targetGeometry);
    }
    animationsGroup->start();
}

void Widget::setMovie(QString fileName)
{
    movie->setFileName(fileName);
    label->setMovie(movie);
    movie->start();
    movie->setSpeed(200);

    defaultSize = movie->frameRect().size();
    label->resize(defaultSize);
}

void Widget::setPos(QPoint pos)
{
    label->move(pos);
}

void Widget::addAnimation(QString type, float duration,
                          QRect startGeometry, QRect targetGeometry)
{
    QPointer<QPropertyAnimation> animation = new QPropertyAnimation(this, type.toLatin1());
    animation->setTargetObject(label);
    animation->setDuration(duration);
    animation->setEasingCurve(QEasingCurve::InCubic);
    animation->setStartValue(startGeometry);
    animation->setEndValue(targetGeometry);
    animationsGroup->addAnimation(animation);
}

void Widget::startShifting(float duration)
{
    animationsGroup->clear();
    addAnimation("geometry", duration, label->geometry(), QRect(1920, 1080, 50, 50));
    addAnimation("geometry", duration, QRect(1920, 1080, 50, 50), label->geometry());
    animationsGroup->setLoopCount(1);
    animationsGroup->start();

    rotationAnimation = new QVariantAnimation(this);
    rotationAnimation->setDuration(10000);
    rotationAnimation->setStartValue(0.0f);
    rotationAnimation->setEndValue(-360.0f);
    connect(rotationAnimation, &QVariantAnimation::valueChanged, [=](const QVariant &value){
        QTransform t;
        t.rotate(value.toReal());
        label->setPixmap(movie->currentPixmap().transformed(t));
    });
    connect(rotationAnimation, &QVariantAnimation::finished, [=](){
        label->setMovie(movie);
    });
    parallelAnimationsGroup->addAnimation(rotationAnimation);

    QPointer<QVariantAnimation> animationCircle = new QVariantAnimation(this);
    animationCircle->setDuration(10000);
    animationCircle->setStartValue(90.0);
    animationCircle->setEndValue(-270.0);
    connect(animationCircle, &QVariantAnimation::valueChanged, [=](const QVariant &value){
        QPoint newPos(this->width() / 2 - defaultSize.width() / 2 + 500 * qCos(qDegreesToRadians(value.toReal())),
                      this->height() / 2 - defaultSize.height() / 2 + 500 * qSin(qDegreesToRadians(value.toReal())));
        label->move(newPos);
    });
    parallelAnimationsGroup->addAnimation(animationCircle);
    parallelAnimationsGroup->setLoopCount(5);
    parallelAnimationsGroup->start();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(isShift) {
        QPoint newPos = label->pos() + event->globalPos() - mousePos;
        mousePos = event->globalPos();
        setPos(newPos);
    }
    event->accept();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()){
    case Qt::LeftButton:{
        currentSize = label->size();
        mousePos = event->globalPos();

        setSize(label->size() * down);
        movie->setSpeed(movie->speed() * down);

        isShift = true;
        break;
    }
    case Qt::RightButton:{
        if(!isShift)
            startShifting(10000);
        break;
    }
    case Qt::MidButton:{
        //movie->start();
        //this->close();
        break;
    }
    default:{
        qDebug() << event->button();
        break;
    }
    }
    event->accept();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:{
        if(!isChangedSize)
            setSize(currentSize);
        movie->setSpeed(movie->speed() * up);

        isChangedSize = false;
        isShift = false;
        break;
    }
    default:{
        break;
    }
    }
    event->accept();
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(isShift)
        isChangedSize = true;
    setSize(label->size() * (event->angleDelta().y() > 0 ? up : down));
    event->accept();
}

/*void Widget::enterEvent(QEvent *event)
{

}*/




