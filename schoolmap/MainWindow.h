//
// Created by DELL on 2024/8/28.
//

#ifndef SCHOOLMAP_MAINWINDOW_H
#define SCHOOLMAP_MAINWINDOW_H

#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QFrame>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <iostream>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLinkedList>
#include "MapView.h"
#include "Location.h"
#include "ShortPath.h"
#include "HoverLabel.h"
#include <QDebug>
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>

// 定义路障的模式
enum BlockageMode {
    BothDirections, // 双向
    OneToTwo,       // 从一到二
    TwoToOne        // 从二到一
};

// 定义路障的结构体
struct Blockage {
    int firstPoint;      // 一号点数据
    int secondPoint;     // 二号点数据
    BlockageMode mode;   // 障碍模式

    // 构造函数
    Blockage(int first, int second, BlockageMode m)
            : firstPoint(first), secondPoint(second), mode(m) {}
};

struct LocationNode {
    QString name;
    LocationNode* next;
    LocationNode* prev;

    LocationNode(const QString& name)
            : name(name), next(nullptr), prev(nullptr) {}
};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onQueryButtonClicked();
    void onClearButtonClicked();
    void onConfirmButtonClicked();
    void onShowPointsToggled(bool checked);
    void onShowEdgesToggled(bool checked);
    void onSetBlockageButtonClicked();
    void onRemoveBlockageButtonClicked(QHBoxLayout *blockageItemLayout, QGraphicsEllipseItem *marker, QGraphicsTextItem *blockageNameLabel);

    void onAddLocationButtonClicked();
    void moveUp(LocationNode* node);
    void moveDown(LocationNode* node);
    void updateDisplay();
    void removeLocationFromList(LocationNode* node);
    void addLocationToList(const QString& name);
    void clearLayout(QLayout* layout);
    void onPlanButtonClicked();
    void onMutiClearButtonClicked();

private:
    int blockageCount;
    QComboBox *currentLocationComboBox;
    QComboBox *startComboBox;
    QComboBox *endComboBox;

    QCheckBox *showPointsSwitch;
    QCheckBox *showEdgesSwitch;

    QComboBox *firstPointComboBox;
    QComboBox *secondPointComboBox;
    QCheckBox *fromOneToTwoCheckBox;
    QCheckBox *fromTwoToOneCheckBox;

    QGraphicsScene *scene;
    MapView *mapViewer;
    std::vector<QGraphicsLineItem*> currentPathItems;
    std::vector<QGraphicsItem*> previousMarkers;
    std::vector<QGraphicsItem*> pointItems;
    std::vector<QGraphicsLineItem *> edgeItems;
    QVBoxLayout *blockScrollLayout;
    std::vector<QHBoxLayout*> blockageLayouts;

    std::vector<Blockage> blockages;
    std::vector<QGraphicsItem*> blockageMarkers; // 用于存储路障标记的图形项

    QComboBox *locationComboBox;
    QButtonGroup *selectionGroup;
    QRadioButton *spotNameRadioButton;
    QRadioButton *sequenceRadioButton;

    QRadioButton *orderedRadioButton;
    QRadioButton *unorderedRadioButton;
    QButtonGroup *orderGroup;
    QCheckBox *detailsCheckBox;

    QVBoxLayout *multiObjectiveScrollLayout;

    LocationNode* head;
    int listSize;

    std::vector<QGraphicsItem*> mutiCurrentPathItems;


    // 初始化函数
    void initializeList() {
        head = nullptr;
        listSize = 0;
    }

    bool blockageExists(int firstPoint, int secondPoint, BlockageMode mode);
    std::vector<int> extractDestinations();
    std::vector<int> findOptimalPath(const std::vector<int>& destinations);
    double calculatePathDistance(const std::vector<int>& path);
    double calculatePathDistance2(const std::vector<int>& path, const std::vector<std::vector<double>>& subMatrix);
    double fitnessFunction(const std::vector<int>& path, const std::vector<std::vector<double>>& subMatrix);
    std::vector<std::vector<int>> generateInitialPopulation(int populationSize, int numCities);
    std::vector<int> selection(const std::vector<std::vector<int>>& population, const std::vector<double>& fitnesses);
    std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2);
    void mutate(std::vector<int>& individual);
    std::vector<int> geneticAlgorithmTSP(const std::vector<int>& destinations, int populationSize = 100, int generations = 1000, double mutationRate = 0.01);
};


#endif //SCHOOLMAP_MAINWINDOW_H
