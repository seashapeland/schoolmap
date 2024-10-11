//
// Created by DELL on 2024/8/28.
//

#ifndef SCHOOLMAP_LOCATION_H
#define SCHOOLMAP_LOCATION_H

#include <QString>
#include <vector>
#include <algorithm>
#include <QTextCodec>
#include <QCollator>

// 定义地点的结构体
struct Location {
    QString name;       // 地点名字
    QString info;       // 地点信息介绍
    QString imagePath;  // 地点图片链接
};

// 声明全局变量
extern std::vector<Location> CAUlocations;
extern QCollator collator;

char getFirstPinyinLetter(const QString& name);
int partition(std::vector<Location>& locations, int low, int high);
void quickSort(std::vector<Location>& locations, int low, int high);
void sortLocations(std::vector<Location>& locations);

#endif //SCHOOLMAP_LOCATION_H
