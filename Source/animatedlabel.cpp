#include "animatedlabel.h"

AnimatedLabel::AnimatedLabel(QWidget *parent)
    : QLabel(parent)
{

}

AnimatedLabel::AnimatedLabel(const AnimatedLabel &label) : QLabel(label.parentWidget())
{

}

AnimatedLabel &AnimatedLabel::operator=(const AnimatedLabel &other)
{
    destroy();
    QLabel(other.parentWidget());
    return *this;
}

void AnimatedLabel::set(const QString &fileName, QSize screenSize, int speed)
{
    setCursor(Qt::PointingHandCursor);
    setScaledContents(true);
    setMovie(&movie);


    movie.setCacheMode(QMovie::CacheAll);
    movie.setFileName(fileName);
    movie.setSpeed(speed);
    movie.start();

    this->screenSize = screenSize;
    defaultSize = movie.frameRect().size();
    resize(defaultSize);
}

void AnimatedLabel::setSequentialAnimation(const QList<QRect> &rects,
                                           const QList<int> &durations)
{
     animationsGroup.clear();
     auto r = rects.cbegin();
     auto d = durations.cbegin();
     while(r + 1 != rects.end())
     {
         addAnimation("geometry", *d, *r, *(r + 1));
         d++; r++;
     }
}

void AnimatedLabel::startSequentialAnimation(int loopCount)
{
    animationsGroup.setLoopCount(loopCount);
    animationsGroup.start();
}

QRect AnimatedLabel::getRect()
{
    return QRect(pos(), size());
}

void AnimatedLabel::setSize(QSize size)
{
    animationsGroup.clear();
    if(size.height() < screenSize.height() * max &&
            size.width() < screenSize.width() * max &&
            size.height() > screenSize.height() * min &&
            size.width() > screenSize.width() * min) {
        QRect targetGeometry = QRect(QPoint(pos().x() + rect().center().x() - size.width() / 2,
                                            pos().y() + rect().center().y() - size.height() / 2),
                                     size);
        addAnimation("geometry", 20, geometry(), targetGeometry);
        animationsGroup.start();
    }

}

void AnimatedLabel::addAnimation(const QString &type, float duration,
                          const QVariant &startGeometry, const QVariant &targetGeometry)
{
    QPointer<QPropertyAnimation> animation = new QPropertyAnimation(this, type.toLatin1());
    animation->setTargetObject(this);
    animation->setDuration(duration);
    //animation->setEasingCurve(QEasingCurve::InCubic);
    animation->setStartValue(startGeometry);
    animation->setEndValue(targetGeometry);
    animationsGroup.addAnimation(animation);
}

void AnimatedLabel::startShifting(float duration)
{
    animationsGroup.clear();
    addAnimation("geometry", duration, geometry(), QRect(1920, 1080, 50, 50));
    addAnimation("geometry", duration, QRect(1920, 1080, 50, 50), this->geometry());
    animationsGroup.setLoopCount(1);
    //animationsGroup->start();

    rotationAnimation.setDuration(10000);
    rotationAnimation.setStartValue(0.0f);
    rotationAnimation.setEndValue(-360.0f);
    connect(&rotationAnimation, &QVariantAnimation::valueChanged, [=](const QVariant &value){
        QTransform t;
        t.rotate(value.toReal());
        this->setPixmap(movie.currentPixmap().transformed(t));
    });
    connect(&rotationAnimation, &QVariantAnimation::finished, [=](){
        this->setMovie(&movie);
    });
    parallelAnimationsGroup.addAnimation(&rotationAnimation);

    QPointer<QVariantAnimation> animationCircle = new QVariantAnimation(this);
    animationCircle->setDuration(10000);
    animationCircle->setStartValue(90.0);
    animationCircle->setEndValue(-270.0);
    connect(animationCircle, &QVariantAnimation::valueChanged, [=](const QVariant &value){
        QPoint newPos(this->width() / 2 - defaultSize.width() / 2 + 500 * qCos(qDegreesToRadians(value.toReal())),
                      this->height() / 2 - defaultSize.height() / 2 + 500 * qSin(qDegreesToRadians(value.toReal())));
        this->move(newPos);
    });
    parallelAnimationsGroup.addAnimation(animationCircle);
    parallelAnimationsGroup.setLoopCount(5);
    parallelAnimationsGroup.start();
}

void AnimatedLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(isShift) {
        QPoint newPos = this->pos() + event->globalPos() - mousePos;
        mousePos = event->globalPos();
        this->move(newPos);
    }
    event->accept();
}

void AnimatedLabel::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()){
    case Qt::LeftButton:{
        currentSize = size();
        mousePos = event->globalPos();

        setSize(size() * down);
        movie.setSpeed(movie.speed() * down);

        isShift = true;
        break;
    }
    /*case Qt::RightButton:{
        if(!isShift)
            startShifting(10000);
        break;
    }*/
    case Qt::RightButton:{
        this->close();
        break;
    }
    default:{
        qDebug() << event->button();
        break;
    }
    }
    event->accept();
}

void AnimatedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:{
        if(!isChangedSize)
            setSize(currentSize);
        movie.setSpeed(movie.speed() * up);
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

void AnimatedLabel::wheelEvent(QWheelEvent *event)
{
    if(isShift)
        isChangedSize = true;
    setSize(this->size() * (event->angleDelta().y() > 0 ? up : down));
    event->accept();
}
