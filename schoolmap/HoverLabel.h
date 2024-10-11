//
// Created by DELL on 2024/8/30.
//

#ifndef SCHOOLMAP_HOVERLABEL_H
#define SCHOOLMAP_HOVERLABEL_H
#include <QLabel>
#include <QHoverEvent>
#include <QToolTip>

class HoverLabel : public QLabel{
    Q_OBJECT

public:
    HoverLabel(const QString &imagePath, QWidget *parent = nullptr);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString imagePath;
    QLabel *popupLabel;

    void showLargeImage();
    void hideLargeImage();
};


#endif //SCHOOLMAP_HOVERLABEL_H
