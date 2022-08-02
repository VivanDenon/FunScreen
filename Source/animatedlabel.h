#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H

#include "included.h"

class AnimatedLabel : public QLabel
{
    Q_OBJECT
public:
    AnimatedLabel(QWidget *parent = nullptr);
    AnimatedLabel(const AnimatedLabel& other);
    AnimatedLabel &operator=(const AnimatedLabel& other);

    void set(const QString &fileName, QSize screenSize, int speed = 100);
    void setSequentialAnimation(const QList<QRect> &rects,
                                const QList<int> &durations);
    void startSequentialAnimation(int loopCount);
    QRect getRect();
private:
    QMovie movie;
    QSize defaultSize, currentSize, screenSize;
    QPoint mousePos;

    QVariantAnimation rotationAnimation;
    QSequentialAnimationGroup animationsGroup;
    QParallelAnimationGroup parallelAnimationsGroup;

    QList<QRect> rects;
    QList<int> durations;
    QList<int> speeds;

    const float up = 1.25, down = 0.8, min = 0.02, max = 1.5;
    bool isChangedSize = false, isShift = false;

    void setSize(QSize size);
    void addAnimation(const QString &type, float duration,
                      const QVariant &startGeometry, const QVariant &targetGeometry);
    void startShifting(float duration);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};
#endif // ANIMATEDLABEL_H
