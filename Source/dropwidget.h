#ifndef DROPWIDGET_H
#define DROPWIDGET_H

#include "included.h"

class DropWidget : public QLabel
{
    Q_OBJECT
public:
    explicit DropWidget(QWidget *parent = nullptr);
    void setSpeedMovie(int value);
signals:
    void clickedToCreate(const QString &fileName, int speed);
protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    QMovie movie;
    const QString defaultStyleSheet = R"(background-color: white;
                  border-style: dashed;
                  border-width: 1.5px;
                  border-radius: 10px;)",
    alternativeStyleSheet = R"(background-color: blue;
                  border-style: dashed;
                  border-width: 1.5px;
                  border-radius: 10px;)";
    QString fileName;
    bool isExistFile = false;
};

#endif // DROPWIDGET_H
