//
// Created by DELL on 2024/8/28.
//

#include <iostream>
#include "MapView.h"


MapView::MapView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent) {
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  // 设置缩放锚点在鼠标下
}


void MapView::wheelEvent(QWheelEvent *event) {
    // 缩放因子
    const double scaleFactor = 1.15;

    if (event->angleDelta().y() > 0) {
        // 放大
        scale(scaleFactor, scaleFactor);
    } else {
        // 缩小
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MapView::mousePressEvent(QMouseEvent *event) {
//    if (event->button() == Qt::LeftButton) {
//        QPointF scenePos = mapToScene(event->pos());
//        auto *marker = new QGraphicsEllipseItem(scenePos.x() - 2.5, scenePos.y() - 2.5, 5, 5);
//        std::cout << "(" << scenePos.x() << "," << scenePos.y() << ")" << std::endl;
//        marker->setBrush(Qt::red);
//        scene()->addItem(marker);
//    }
      QGraphicsView::mousePressEvent(event);
}
