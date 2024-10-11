//
// Created by DELL on 2024/8/28.
//

#include "ShortPath.h"


std::unordered_map<QString, int, QStringHash> locationHash;
std::vector<MapPoint> mappoints;
int len;
std::vector<std::vector<double>> adjacencyMatrix;
std::vector<std::pair<int, int>> connections;

double calculateWeight(const MapPoint& a, const MapPoint& b) {
    return std::pow(a.position.x() - b.position.x(), 2) + std::pow(a.position.y() - b.position.y(), 2);
}

std::vector<int> dijkstra(const std::vector<std::vector<double>>& adjMatrix, int start, int end) {
    int n = adjMatrix.size();
    std::vector<double> dist(n, std::numeric_limits<double>::infinity());
    std::vector<int> prev(n, -1);
    std::vector<bool> visited(n, false);

    dist[start] = 0;

    auto cmp = [&](int left, int right) { return dist[left] > dist[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> pq(cmp);

    pq.push(start);

    while (!pq.empty()) {
        int u = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && adjMatrix[u][v] < std::numeric_limits<double>::infinity()) {
                double alt = dist[u] + adjMatrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    pq.push(v);
                }
            }
        }
    }

    // 构建最短路径数组
    std::vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());

    return path;
}

void floydWarshall(const std::vector<std::vector<double>>& adjMatrix, std::vector<std::vector<double>>& dist, std::vector<std::vector<int>>& next) {
    int n = adjMatrix.size();
    dist = adjMatrix;
    next.resize(n, std::vector<int>(n, -1));

    // 初始化next矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j && dist[i][j] < std::numeric_limits<double>::infinity()) {
                next[i][j] = j;
            }
        }
    }

    // Floyd-Warshall算法
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] < std::numeric_limits<double>::infinity() && dist[k][j] < std::numeric_limits<double>::infinity()) {
                    double newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j]) {
                        dist[i][j] = newDist;
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }
}

std::vector<int> constructPath(int start, int end, const std::vector<std::vector<int>>& next) {
    if (next[start][end] == -1) {
        return {};  // 无路径
    }
    std::vector<int> path = {start};
    while (start != end) {
        start = next[start][end];
        path.push_back(start);
    }
    return path;
}

double heuristic(const MapPoint& a, const MapPoint& b) {
    // 使用欧几里得距离的平方作为启发式函数
    return calculateWeight(a, b);
}

std::vector<int> aStar(const std::vector<std::vector<double>>& adjMatrix, const std::vector<MapPoint>& points, int start, int end) {
    int n = adjMatrix.size();
    std::vector<double> gScore(n, std::numeric_limits<double>::infinity());  // 从起点到当前节点的实际代价
    std::vector<double> fScore(n, std::numeric_limits<double>::infinity());  // 估计的从起点到终点的代价
    std::vector<int> cameFrom(n, -1);  // 记录路径
    std::vector<bool> visited(n, false);

    auto cmp = [&](int left, int right) { return fScore[left] > fScore[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> openSet(cmp);

    gScore[start] = 0;
    fScore[start] = heuristic(points[start], points[end]);
    openSet.push(start);

    while (!openSet.empty()) {
        int current = openSet.top();
        openSet.pop();

        if (current == end) {
            // 重建路径
            std::vector<int> path;
            while (current != -1) {
                path.push_back(current);
                current = cameFrom[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        visited[current] = true;

        for (int neighbor = 0; neighbor < n; ++neighbor) {
            if (adjMatrix[current][neighbor] < std::numeric_limits<double>::infinity()) {
                if (visited[neighbor]) continue;

                double tentative_gScore = gScore[current] + adjMatrix[current][neighbor];

                if (tentative_gScore < gScore[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = gScore[neighbor] + heuristic(points[neighbor], points[end]);

                    openSet.push(neighbor);
                }
            }
        }
    }

    return {};  // 如果没有路径
}

void initalizeMappointsAndConnections() {
    mappoints.push_back({QPointF(1102.74, 543.414), "V0"});
    mappoints.push_back({QPointF(1046.56, 543.414), "V1"});
    mappoints.push_back({QPointF(1046.06, 530.488), "V2"});
    mappoints.push_back({QPointF(1047.05, 605.561), "V3"});
    mappoints.push_back({QPointF(930.218, 535.459), "V4"});
    mappoints.push_back({QPointF(933.698, 609.539), "V5"});
    mappoints.push_back({QPointF(927.732, 698.533), "V6"});
    mappoints.push_back({QPointF(887.958, 472.318), "V7"});
    mappoints.push_back({QPointF(890.444, 535.459), "V8"});
    mappoints.push_back({QPointF(844.206, 531.482), "V9"});
    mappoints.push_back({QPointF(835.754, 368.905), "V10"});
    mappoints.push_back({QPointF(834.263, 283.888), "V11"});
    mappoints.push_back({QPointF(831.28, 227.707), "V12"});
    mappoints.push_back({QPointF(763.166, 40.7685), "V13"});
    mappoints.push_back({QPointF(847.189, 610.036), "V14"});
    mappoints.push_back({QPointF(847.189, 702.511), "V15"});
    mappoints.push_back({QPointF(865.585, 939.664), "V16"});
    mappoints.push_back({QPointF(870.059, 1027.66), "V17"});
    mappoints.push_back({QPointF(880.5, 1238.96), "V18"});
    mappoints.push_back({QPointF(658.262, 290.35), "V19"});
    mappoints.push_back({QPointF(689.584, 389.787), "20"});
    mappoints.push_back({QPointF(659.754, 390.781), "V21"});
    mappoints.push_back({QPointF(691.573, 533.471), "V22"});
    mappoints.push_back({QPointF(709.968, 612.025), "V23"});
    mappoints.push_back({QPointF(689.584, 1030.65), "V24"});
    mappoints.push_back({QPointF(557.335, 398.239), "V25"});
    mappoints.push_back({QPointF(561.313, 541.923), "V26"});
    mappoints.push_back({QPointF(565.29, 616.996), "V27"});
    mappoints.push_back({QPointF(568.77, 720.906), "V28"});
    mappoints.push_back({QPointF(577.222, 955.574), "V29"});
    mappoints.push_back({QPointF(582.691, 1031.14), "V30"});
    mappoints.push_back({QPointF(476.295, 1040.59), "V31"});
    mappoints.push_back({QPointF(367.911, 731.347), "V32"});
    mappoints.push_back({QPointF(370.397, 879.008), "V33"});
    mappoints.push_back({QPointF(372.385, 963.529), "V34"});
    mappoints.push_back({QPointF(379.346, 1045.56), "V35"});
    mappoints.push_back({QPointF(297.809, 880.003), "V36"});
    mappoints.push_back({QPointF(106.396, 749.743), "V37"});
    mappoints.push_back({QPointF(939.167, 938.172), "V38"});
    mappoints.push_back({QPointF(951.596, 1089.31), "V39"});
    mappoints.push_back({QPointF(1053.52, 932.704), "V40"});
    mappoints.push_back({QPointF(1054.01, 1079.87), "V41"});
    mappoints.push_back({QPointF(1111.19, 928.229), "V42"});
    mappoints.push_back({QPointF(1204.16, 926.737), "V43"});
    mappoints.push_back({QPointF(1210.63, 1073.9), "V44"});
    mappoints.push_back({QPointF(1272.77, 1077.38), "V45"});
    mappoints.push_back({QPointF(1305.59, 919.28), "V46"});
    mappoints.push_back({QPointF(1300.61, 824.816), "V47"});
    mappoints.push_back({QPointF(1425.9, 817.359), "V48"});
    mappoints.push_back({QPointF(1289.18, 720.409), "V49"});
    mappoints.push_back({QPointF(1259.85, 600.092), "V50"});
    mappoints.push_back({QPointF(1096.27, 605.561), "V51"});
    mappoints.push_back({QPointF(1102.24, 753.72), "V52"});
    mappoints.push_back({QPointF(1430.1, 911.317), "V53"});

    len = mappoints.size();

    // 添加连接关系
    connections.push_back({1, 2});
    connections.push_back({2, 3});
    connections.push_back({2, 4});
    connections.push_back({3, 5});
    connections.push_back({4, 6});
    connections.push_back({4, 52});
    connections.push_back({5, 9});
    connections.push_back({5, 6});
    connections.push_back({6, 7});
    connections.push_back({6, 15});
    connections.push_back({7, 16});
    connections.push_back({8, 9});
    connections.push_back({9, 10});
    connections.push_back({10, 11});
    connections.push_back({10, 15});
    connections.push_back({10, 23});
    connections.push_back({11, 12});
    connections.push_back({12, 13});
    connections.push_back({12, 20});
    connections.push_back({13, 14});
    connections.push_back({15, 16});
    connections.push_back({15, 24});
    connections.push_back({16, 17});
    connections.push_back({16, 29});
    connections.push_back({17, 30});
    connections.push_back({17, 39});
    connections.push_back({17, 18});
    connections.push_back({18, 19});
    connections.push_back({18, 25});
    connections.push_back({20, 22});
    connections.push_back({21, 22});
    connections.push_back({21, 23});
    connections.push_back({22, 26});
    connections.push_back({23, 27});
    connections.push_back({24, 28});
    connections.push_back({25, 31});
    connections.push_back({26, 27});
    connections.push_back({27, 28});
    connections.push_back({28, 29});
    connections.push_back({29, 33});
    connections.push_back({29, 30});
    connections.push_back({30, 31});
    connections.push_back({30, 35});
    connections.push_back({31, 32});
    connections.push_back({32, 36});
    connections.push_back({33, 34});
    connections.push_back({33, 38});
    connections.push_back({34, 35});
    connections.push_back({34, 37});
    connections.push_back({35, 36});
    connections.push_back({39, 40});
    connections.push_back({39, 41});
    connections.push_back({40, 42});
    connections.push_back({41, 43});
    connections.push_back({41, 42});
    connections.push_back({42, 45});
    connections.push_back({43, 44});
    connections.push_back({43, 53});
    connections.push_back({44, 45});
    connections.push_back({44, 47});
    connections.push_back({45, 46});
    connections.push_back({47, 48});
    connections.push_back({48, 50});
    connections.push_back({48, 49});
    connections.push_back({49, 54});
    connections.push_back({50, 51});
    connections.push_back({51, 52});
    connections.push_back({52, 53});
}

void initializeLocationHash() {
    locationHash["王子楼"] = 0;
    locationHash["CAU图书馆"] = 7;
    locationHash["CAU三教"] = 23;
    locationHash["CAU二教"] = 31;
    locationHash["工学院"] = 10;
    locationHash["水利与土木工程学院"] = 12;
    locationHash["信息与电气工程学院"] = 19;
    locationHash["CAU一教"] = 24;
    locationHash["CAU体育馆"] = 52;
    locationHash["CAU运动场"] = 51;
    locationHash["CAU公共澡堂"] = 53;
    locationHash["东门外卖柜"] = 50;
    locationHash["东门近邻宝"] = 49;
    locationHash["国际学院"] = 46;
    locationHash["篮球场"] = 42;
    locationHash["公主楼"] = 41;
    locationHash["人文与发展学院"] = 39;
    locationHash["商业街"] = 45;
    locationHash["研究生楼"] = 46;
    locationHash["理学院"] = 32;
    locationHash["经济管理学院"] = 35;
    locationHash["CAU校医院"] = 36;
    locationHash["计网中心"] = 33;
    locationHash["马锦明大楼"] = 30;
    locationHash["东南门"] = 18;
    locationHash["北门"] = 13;
    locationHash["西门"] = 37;
}

void initializeAdjacencyMatrix() {
    std::cout << len << std::endl;
    adjacencyMatrix.resize(len); // 调整外部向量的大小

    for (int i = 0; i < len; ++i) {
        // 调整每个内部向量的大小并初始化为正无穷大
        adjacencyMatrix[i].resize(len, std::numeric_limits<double>::infinity());
    }
    // 填充邻接矩阵
    for (const auto& conn : connections) {
        int i = conn.first - 1;
        int j = conn.second - 1;
        double weight = calculateWeight(mappoints[i], mappoints[j]);
        adjacencyMatrix[i][j] = weight;
        adjacencyMatrix[j][i] = weight;  // 因为是无向图，所以对称
    }
}