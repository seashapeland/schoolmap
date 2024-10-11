//
// Created by DELL on 2024/8/30.
//

#include "HoverLabel.h"

HoverLabel::HoverLabel(const QString &imagePath, QWidget *parent)
        : QLabel(parent), imagePath(imagePath), popupLabel(nullptr) {
    setMouseTracking(true);
}

void HoverLabel::enterEvent(QEvent *event) {
    QLabel::enterEvent(event);
    showLargeImage();
}

void HoverLabel::leaveEvent(QEvent *event) {
    QLabel::leaveEvent(event);
    hideLargeImage();
}

void HoverLabel::showLargeImage() {
    if (!popupLabel) {
        QPixmap pixmap(imagePath);
        QPixmap scaledPixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio);

        popupLabel = new QLabel;
        popupLabel->setPixmap(scaledPixmap);
        popupLabel->setWindowFlags(Qt::ToolTip); // 使 QLabel 显示为浮动窗口
        popupLabel->move(QCursor::pos());
        popupLabel->show();
    }
}

void HoverLabel::hideLargeImage() {
    if (popupLabel) {
        popupLabel->hide();
        popupLabel->deleteLater();
        popupLabel = nullptr;
    }
}
