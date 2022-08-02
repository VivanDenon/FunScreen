#ifndef BACKGROUNDWIDGET_H
#define BACKGROUNDWIDGET_H

#include "included.h"
#include "animatedlabel.h"

class BackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BackgroundWidget(QWidget *parent = nullptr);

    QRect getRectFromLabel(int index);

    void setAnimationForLabel(int index, const QList<QRect> &rects,
                              const QList<int> &durations);
    void startAnimationForLabel(int index, int loopCount);
    void addAnimatedLabel(QString fileName, int speed = 100);
private:
    QVector<QPointer<AnimatedLabel>> labels;
};

#endif // BACKGROUNDWIDGET_H
