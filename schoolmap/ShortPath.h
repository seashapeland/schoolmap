//
// Created by DELL on 2024/8/28.
//

#ifndef SCHOOLMAP_SHORTPATH_H
#define SCHOOLMAP_SHORTPATH_H
#include <QApplication>
#include <QWidget>
#include <iostream>
#include <cmath>
#include <queue>
#include <stack>
#include <unordered_map>

struct MapPoint {
    QPointF position;
    QString label;
};

struct QStringHash {
    std::size_t operator()(const QString& key) const {
        return qHash(key);
    }
};

extern std::unordered_map<QString, int, QStringHash> locationHash;
extern std::vector<MapPoint> mappoints;
extern int len;
extern std::vector<std::vector<double>> adjacencyMatrix;
extern std::vector<std::pair<int, int>> connections;

double calculateWeight(const MapPoint& a, const MapPoint& b);
std::vector<int> dijkstra(const std::vector<std::vector<double>>& adjMatrix, int start, int end);
void floydWarshall(const std::vector<std::vector<double>>& adjMatrix, std::vector<std::vector<double>>& dist, std::vector<std::vector<int>>& next);
std::vector<int> constructPath(int start, int end, const std::vector<std::vector<int>>& next);
double heuristic(const MapPoint& a, const MapPoint& b);
std::vector<int> aStar(const std::vector<std::vector<double>>& adjMatrix, const std::vector<MapPoint>& points, int start, int end);
void initalizeMappointsAndConnections();
void initializeLocationHash();
void initializeAdjacencyMatrix();


#endif //SCHOOLMAP_SHORTPATH_H
