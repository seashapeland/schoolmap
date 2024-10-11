#include <QApplication>
#include <QWidget>
#include <iostream>
#include "MainWindow.h"
#include "ShortPath.h"
#include "Location.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    collator.setLocale(QLocale::Chinese);  // 设置为中文区域
    collator.setCaseSensitivity(Qt::CaseInsensitive);  // 忽略大小写

    sortLocations(CAUlocations);
    initalizeMappointsAndConnections();
    initializeLocationHash();
    initializeAdjacencyMatrix();

    // 创建主窗口
    MainWindow window;
    window.setWindowTitle("CAU指南");
    window.show();

    return app.exec();
}
