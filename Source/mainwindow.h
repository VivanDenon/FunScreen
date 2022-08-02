#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "included.h"
#include "dropwidget.h"
#include "backgroundwidget.h"
#include "animatedlabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onClickedToCreate(const QString &fileName, int speed);
    void on_horizontalSlider_valueChanged(int value);

    void on_addAnimationButton_released();

    void on_startAnimationButton_released();

    void on_stopAnimationButton_released();

private:
    Ui::MainWindow *ui;
    QPointer<DropWidget> dw;
    QPointer<BackgroundWidget> bw;
    QList<QRect> rects;
    QList<int> durations;
};

#endif // MAINWINDOW_H
