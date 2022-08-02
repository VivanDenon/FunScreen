#ifndef WIDGET_H
#define WIDGET_H

#include "included.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void set(QString fileName);
    void setMovie(QString fileName);
private slots:

private:
    Ui::Widget *ui;
    QPointer<QLabel> label;
    QPointer<QMovie> movie;
    QPointer<QVariantAnimation> rotationAnimation;
    QPointer<QComboBox> comboBox;
    QPointer<QSequentialAnimationGroup> animationsGroup;
    QPointer<QParallelAnimationGroup> parallelAnimationsGroup;

    QSize defaultSize, currentSize;
    QPoint mousePos;

    const float up = 1.25, down = 0.8, min = 0.02, max = 1.5;
    bool isShift = false;
    bool isChangedSize = false;

    void setSize(QSize size);
    void setPos(QPoint pos);
    void addAnimation(QString type, float duration,
                      QRect startGeometry, QRect targetGeometry);
    void startShifting(float duration);

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    //void enterEvent(QEvent *event) override;
};
#endif // WIDGET_H
