//
// Created by DELL on 2024/8/28.
//

#ifndef SCHOOLMAP_MAPVIEW_H
#define SCHOOLMAP_MAPVIEW_H


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>

class MapView : public QGraphicsView {
public:
    MapView(QGraphicsScene *scene, QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};


#endif //SCHOOLMAP_MAPVIEW_H
