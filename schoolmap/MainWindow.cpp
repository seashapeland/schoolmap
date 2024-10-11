//
// Created by DELL on 2024/8/28.
//



#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : blockageCount(0) {
    initializeList();
    // 主布局：水平布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 左侧：关键地点信息展示区
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setSpacing(10); // 设置各个小矩形之间的间距

    QScrollArea *scrollArea = new QScrollArea;
    QWidget *scrollWidget = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout;
    scrollLayout->setContentsMargins(10, 10, 0, 10); // 设置内部边距为0
    scrollLayout->setSpacing(10); // 设置小矩形之间的间距

    for (int i = 0; i < CAUlocations.size(); ++i) {
        // 创建小矩形的容器，使用 QFrame 代替 QWidget
        QFrame *pointFrame = new QFrame;
        QHBoxLayout *pointLayout = new QHBoxLayout;
        pointLayout->setContentsMargins(5, 5, 5, 5); // 设置图片和文字之间的间距

        // 使用 HoverLabel 显示图片
        HoverLabel *imageLabel = new HoverLabel(CAUlocations[i].imagePath);
        QPixmap pixmap(CAUlocations[i].imagePath); // 这里替换为你的图片路径
        imageLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio)); // 调整图片大小
        imageLabel->setFixedSize(50, 50);

        // 地点名字和信息布局
        QVBoxLayout *infoLayout = new QVBoxLayout;
        QLabel *nameLabel = new QLabel(QString(CAUlocations[i].name));
        nameLabel->setStyleSheet("font-weight: bold; text-decoration: underline; color: rgb(100, 100, 100);");
        QLabel *infoLabel = new QLabel(CAUlocations[i].info);
        infoLabel->setStyleSheet("color: rgb(150, 158, 185);");

        // 设置地点信息的大小，使其比名字部分更大
        infoLabel->setFixedHeight(40);
        nameLabel->setFixedHeight(20);

        // 将名字和信息添加到 infoLayout 中
        infoLayout->addWidget(nameLabel);
        infoLayout->addWidget(infoLabel);

        // 将图片和信息布局添加到 pointLayout 中
        pointLayout->addWidget(imageLabel);
        pointLayout->addLayout(infoLayout);

        // 将布局添加到 QFrame 中
        pointFrame->setLayout(pointLayout);
        pointFrame->setFrameStyle(QFrame::Box | QFrame::Plain); // 设置边框样式
        pointFrame->setFixedHeight(70); // 设置每个小矩形的固定高度
        pointFrame->setFixedWidth(250); // 设置每个小矩形的宽度，适应左侧框的宽度

        // 将小矩形添加到滚动区域的布局中
        scrollLayout->addWidget(pointFrame);
    }

    scrollWidget->setLayout(scrollLayout);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedWidth(300); // 设置滚动区域的宽度

    leftLayout->addWidget(scrollArea);

    // 中间上部：地图展示区
    QVBoxLayout *centerLayout = new QVBoxLayout;

    // 创建场景和视图
    scene = new QGraphicsScene(this);
    QPixmap mapPixmap(":/picture/CAUmap.png");
    QGraphicsPixmapItem *mapItem = new QGraphicsPixmapItem(mapPixmap);
    scene->addItem(mapItem);

    mapViewer = new MapView(scene);
    mapViewer->setFixedSize(800, 600); // 设置地图视图的大小

    centerLayout->addWidget(mapViewer);

    // 中间下部：路径查询区
    QFrame *routeFrame = new QFrame;
    routeFrame->setFrameStyle(QFrame::Box | QFrame::Plain); // 设置边框样式
    routeFrame->setLineWidth(1);


    // 路径查询区的布局
    QVBoxLayout *routeLayout = new QVBoxLayout(routeFrame);

    // 第一行：当前位置 + 下拉框 + 确定按钮
    QHBoxLayout *currentLocationLayout = new QHBoxLayout;
    QLabel *currentLocationLabel = new QLabel("当前位置：");
    currentLocationComboBox = new QComboBox;
    for (const auto& location : CAUlocations) {
        currentLocationComboBox->addItem(location.name);
    }

    startComboBox = new QComboBox; // 创建第二行的第一个下拉框
    for (const auto& location : CAUlocations) {
        startComboBox->addItem(location.name);
    }

    currentLocationComboBox->setFixedWidth(150);
    startComboBox->setFixedWidth(150);


    QPushButton *confirmButton = new QPushButton("确定");
    confirmButton->setFixedWidth(80); // 设置按钮宽度
    confirmButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    // 将按钮的点击信号连接到槽函数
    connect(confirmButton, &QPushButton::clicked, this, &MainWindow::onConfirmButtonClicked);


    currentLocationLayout->addWidget(currentLocationLabel);
    currentLocationLayout->addWidget(currentLocationComboBox);
    currentLocationLayout->addStretch();  // 弹性空间，保持对齐
    currentLocationLayout->addWidget(confirmButton);

    // 第二行：起点 + 下拉框 + 终点 + 下拉框
    QHBoxLayout *startEndLayout = new QHBoxLayout;
    QLabel *startLabel = new QLabel("起点位置：");
    QLabel *endLabel = new QLabel("终点位置：");
    endComboBox = new QComboBox;
    for (const auto& location : CAUlocations) {
        endComboBox->addItem(location.name);
    }

    endComboBox->setFixedWidth(150); // 为第二行的第二个下拉框设置宽度

    startEndLayout->addWidget(startLabel);
    startEndLayout->addWidget(startComboBox);
    startEndLayout->addStretch(8); // 弹性空间增加起点和终点之间的距离
    startEndLayout->addWidget(endLabel);
    startEndLayout->addWidget(endComboBox);

    // 第三行：查询按钮
    QHBoxLayout *queryButtonLayout = new QHBoxLayout;
    QPushButton *queryButton = new QPushButton("查询");
    queryButton->setFixedWidth(80); // 设置按钮宽度
    queryButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    // 创建清除按钮
    QPushButton *clearButton = new QPushButton("清除");
    clearButton->setFixedWidth(80); // 设置按钮宽度
    clearButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    // 将清除按钮的点击信号连接到槽函数
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);

    // 将按钮的点击信号连接到槽函数
    connect(queryButton, &QPushButton::clicked, this, &MainWindow::onQueryButtonClicked);

    queryButtonLayout->addStretch();  // 添加弹性空间，使按钮靠右对齐
    queryButtonLayout->addWidget(clearButton);  // 先添加清除按钮
    queryButtonLayout->addWidget(queryButton);

    // 将所有行添加到路径查询区的主布局中
    routeLayout->addLayout(currentLocationLayout);
    routeLayout->addLayout(startEndLayout);
    routeLayout->addLayout(queryButtonLayout);

    // 将路径查询区的框架添加到中间布局
    centerLayout->addWidget(routeFrame);



    // 右侧：功能区
    QVBoxLayout *rightLayout = new QVBoxLayout;

    QFrame *rightOpenFrame = new QFrame;
    rightOpenFrame->setFrameStyle(QFrame::Box | QFrame::Plain); // 设置边框样式
    rightOpenFrame->setLineWidth(1);
    rightOpenFrame->setFixedHeight(60); // 设置默认高度

// 最上面的框：点边显示区
    QHBoxLayout *displayLayout = new QHBoxLayout(rightOpenFrame);


    // 创建“显示点”标签和开关按钮
    QLabel *showPointsLabel = new QLabel("显示点");
    showPointsSwitch = new QCheckBox();
    showPointsSwitch->setFixedSize(40, 20); // 设置开关按钮的大小

    // 将“显示点”标签和开关按钮添加到布局
    displayLayout->addWidget(showPointsLabel);
    displayLayout->addWidget(showPointsSwitch);

    // 创建“显示边”标签和开关按钮
    QLabel *showEdgesLabel = new QLabel("显示边");
    showEdgesSwitch = new QCheckBox();
    showEdgesSwitch->setFixedSize(40, 20); // 设置开关按钮的大小

    // 将“显示边”标签和开关按钮添加到布局
    displayLayout->addWidget(showEdgesLabel);
    displayLayout->addWidget(showEdgesSwitch);

    rightLayout->addWidget(rightOpenFrame);

    connect(showPointsSwitch, &QCheckBox::toggled, this, &MainWindow::onShowPointsToggled);
    connect(showEdgesSwitch, &QCheckBox::toggled, this, &MainWindow::onShowEdgesToggled);

    // 添加路障设置区
    QFrame *blockageFrame = new QFrame;
    blockageFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    blockageFrame->setLineWidth(1);
    blockageFrame->setFixedHeight(120);

// 创建布局并添加到blockageFrame中
    QVBoxLayout *blockageLayout = new QVBoxLayout(blockageFrame);

// 第一行：设置路障标签
    QLabel *blockageLabel = new QLabel("设置路障");
    blockageLabel->setStyleSheet("font-weight: bold; text-decoration: underline; color: rgb(100, 100, 100);");
    blockageLabel->setAlignment(Qt::AlignCenter); // 居中对齐
    blockageLayout->addWidget(blockageLabel);

// 第二行：一号点、二号点和下拉框
    QHBoxLayout *pointsLayout = new QHBoxLayout;
    QLabel *firstPointLabel = new QLabel("一号点：");
    firstPointComboBox = new QComboBox;
    for (int i = 1; i <= mappoints.size(); ++i) {
        firstPointComboBox->addItem(QString::number(i));
    }

    QLabel *secondPointLabel = new QLabel("二号点：");
    secondPointComboBox = new QComboBox;
    for (int i = 1; i <= mappoints.size(); ++i) {
        secondPointComboBox->addItem(QString::number(i));
    }

    pointsLayout->addWidget(firstPointLabel);
    pointsLayout->addWidget(firstPointComboBox);
    pointsLayout->addWidget(secondPointLabel);
    pointsLayout->addWidget(secondPointComboBox);
    blockageLayout->addLayout(pointsLayout);
    blockageLayout->addSpacing(10);

// 第三行：从一到二、从二到一的QCheckBox和设置按钮
    QHBoxLayout *directionLayout = new QHBoxLayout;

    QLabel *fromOneToTwoLabel = new QLabel("从一到二");
    fromOneToTwoCheckBox = new QCheckBox();

    QLabel *fromTwoToOneLabel = new QLabel("从二到一");
    fromTwoToOneCheckBox = new QCheckBox();

    QPushButton *setBlockageButton = new QPushButton("设置");
    setBlockageButton->setFixedWidth(80);
    setBlockageButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    directionLayout->addWidget(fromOneToTwoLabel);
    directionLayout->addWidget(fromOneToTwoCheckBox);
    directionLayout->addStretch();
    directionLayout->addWidget(fromTwoToOneLabel);
    directionLayout->addWidget(fromTwoToOneCheckBox);
    directionLayout->addStretch(); // 添加弹性空间，将设置按钮靠右
    directionLayout->addWidget(setBlockageButton);

    blockageLayout->addLayout(directionLayout);

    connect(setBlockageButton, &QPushButton::clicked, this, &MainWindow::onSetBlockageButtonClicked);

    rightLayout->addWidget(blockageFrame);


    // 添加路障展示区
    QFrame *blockageDisplayFrame = new QFrame;
    blockageDisplayFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    blockageDisplayFrame->setLineWidth(1);
    blockageDisplayFrame->setFixedHeight(200);

// 使用 QScrollArea 来实现可滚动区域
    QScrollArea *blockScrollArea = new QScrollArea();
    blockScrollArea->setWidgetResizable(true); // 自动调整内容大小
    blockScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *blockScrollWidget = new QWidget;
    blockScrollLayout = new QVBoxLayout(blockScrollWidget);
    // 设置 scrollLayout 的对齐方式为顶部对齐
    blockScrollLayout->setAlignment(Qt::AlignTop);



//    for (int i = 1; i <= 15; ++i) {
//        QHBoxLayout *blockageItemLayout = new QHBoxLayout;
//
//        QLabel *blockageNameLabel = new QLabel(QString("路障 %1").arg(i));
//        QPushButton *removeButton = new QPushButton("消除");
//        removeButton->setFixedWidth(80);
//        removeButton->setStyleSheet(
//                "QPushButton {"
//                "background-color: gray;"
//                "color: white;"
//                "border-radius: 5px;"
//                "padding: 5px;"
//                "border: none;"
//                "}"
//                "QPushButton:hover {"
//                "background-color: lightgray;"
//                "color: black;"
//                "}"
//        );
//
//        blockageItemLayout->addWidget(blockageNameLabel);
//        blockageItemLayout->addStretch(); // 添加弹性空间，将按钮靠右
//        blockageItemLayout->addWidget(removeButton);
//
//        blockScrollLayout->addLayout(blockageItemLayout);
//    }
    // 设置滚动区域的内容
    blockScrollArea->setWidget(blockScrollWidget);

    QVBoxLayout *blockageDisplayLayout = new QVBoxLayout(blockageDisplayFrame);
    blockageDisplayLayout->addWidget(blockScrollArea);

    rightLayout->addWidget(blockageDisplayFrame);

    // 添加多目的规划区
    QFrame *multiObjectiveFrame = new QFrame;
    multiObjectiveFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    multiObjectiveFrame->setLineWidth(1);
    multiObjectiveFrame->setFixedHeight(150); // 可以根据需要调整高度

// 创建布局并添加到 multiObjectiveFrame 中
    QVBoxLayout *multiObjectiveLayout = new QVBoxLayout(multiObjectiveFrame);

// 第一行：多目的规划标签
    QLabel *multiObjectiveLabel = new QLabel("多目的规划");
    multiObjectiveLabel->setStyleSheet("font-weight: bold; text-decoration: underline; color: rgb(100, 100, 100);");
    multiObjectiveLabel->setAlignment(Qt::AlignCenter); // 居中对齐
    multiObjectiveLayout->addWidget(multiObjectiveLabel);

// 第二行：添加位置标签、下拉框和添加按钮
    QHBoxLayout *addLocationLayout = new QHBoxLayout;
    QLabel *addLocationLabel = new QLabel("添加位置：");
    locationComboBox = new QComboBox;
    for (const auto &location : CAUlocations) {
        locationComboBox->addItem(location.name);
    }

    QPushButton *addLocationButton = new QPushButton("添加");
    addLocationButton->setFixedWidth(80);
    addLocationButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    // 在 MainWindow 构造函数或初始化代码中
    connect(addLocationButton, &QPushButton::clicked, this, &MainWindow::onAddLocationButtonClicked);


    addLocationLayout->addWidget(addLocationLabel);
    addLocationLayout->addWidget(locationComboBox);
    addLocationLayout->addStretch(); // 添加弹性空间，将按钮推到右边
    addLocationLayout->addWidget(addLocationButton);

    multiObjectiveLayout->addLayout(addLocationLayout);

// 第三行：景点名、序号单选按钮和规划按钮
    QHBoxLayout *selectionLayout = new QHBoxLayout;

    QLabel *spotNameLabel = new QLabel("景点名");
    spotNameRadioButton = new QRadioButton;
    spotNameRadioButton->setChecked(true); // 默认选中景点名

    QLabel *sequenceLabel = new QLabel("序号");
    sequenceRadioButton = new QRadioButton;

    // 创建 QRadioButton 选择事件的槽函数
    connect(spotNameRadioButton, &QRadioButton::clicked, this, [this]() {
        // 清空 ComboBox 的现有选项
        locationComboBox->clear();
        // 添加 CAUlocations 的地点名
        for (const auto &location : CAUlocations) {
            locationComboBox->addItem(location.name);
        }
    });

    connect(sequenceRadioButton, &QRadioButton::clicked, this, [this]() {
        // 清空 ComboBox 的现有选项
        locationComboBox->clear();
        // 添加 mappoints 中的序号
        for (int i = 1; i <= mappoints.size(); ++i) {
            locationComboBox->addItem(QString::number(i));
        }
    });

// 添加互斥行为：二选一
    selectionGroup = new QButtonGroup;
    selectionGroup->addButton(spotNameRadioButton);
    selectionGroup->addButton(sequenceRadioButton);

    QPushButton *mutiClearButton = new QPushButton("清除");
    mutiClearButton->setFixedWidth(80);
    mutiClearButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    QPushButton *planButton = new QPushButton("规划");
    planButton->setFixedWidth(80);
    planButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    connect(planButton, &QPushButton::clicked, this, &MainWindow::onPlanButtonClicked);
    connect(mutiClearButton, &QPushButton::clicked, this, &MainWindow::onMutiClearButtonClicked);

    selectionLayout->addWidget(spotNameLabel);
    selectionLayout->addWidget(spotNameRadioButton);
    selectionLayout->addStretch();
    selectionLayout->addWidget(sequenceLabel);
    selectionLayout->addWidget(sequenceRadioButton);
    selectionLayout->addStretch(); // 添加弹性空间，将按钮推到右边
    selectionLayout->addWidget(mutiClearButton);
    selectionLayout->addWidget(planButton);

    multiObjectiveLayout->addSpacing(10);
    multiObjectiveLayout->addLayout(selectionLayout);

    // 添加第四行：有序、无序单选按钮
    QHBoxLayout *orderLayout = new QHBoxLayout;

    QLabel *orderedLabel = new QLabel("有序");
    orderedRadioButton = new QRadioButton;
    orderedRadioButton->setChecked(true); // 默认选中有序

    QLabel *unorderedLabel = new QLabel("无序");
    unorderedRadioButton = new QRadioButton;

// 添加互斥行为：二选一
    orderGroup = new QButtonGroup;
    orderGroup->addButton(orderedRadioButton);
    orderGroup->addButton(unorderedRadioButton);


    orderLayout->addWidget(orderedLabel);
    orderLayout->addWidget(orderedRadioButton);
    orderLayout->addStretch();
    orderLayout->addWidget(unorderedLabel);
    orderLayout->addWidget(unorderedRadioButton);
    orderLayout->addStretch(); // 添加弹性空间

    // 添加细节标签和 QCheckBox
    QLabel *detailsLabel = new QLabel("细节");
    detailsCheckBox = new QCheckBox;

    orderLayout->addStretch(); // 添加弹性空间，将细节部分靠右
    orderLayout->addWidget(detailsLabel);
    orderLayout->addWidget(detailsCheckBox);

    multiObjectiveLayout->addSpacing(10);
    multiObjectiveLayout->addLayout(orderLayout);


// 添加多目的规划区到右侧布局
    rightLayout->addWidget(multiObjectiveFrame);


    // 添加多目的展示区
    QFrame *multiObjectiveDisplayFrame = new QFrame;
    multiObjectiveDisplayFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    multiObjectiveDisplayFrame->setLineWidth(1);

// 使用 QScrollArea 来实现可滚动区域
    QScrollArea *multiObjectiveScrollArea = new QScrollArea();
    multiObjectiveScrollArea->setWidgetResizable(true); // 自动调整内容大小
    multiObjectiveScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget * multiObjectiveScrollWidget = new QWidget;
    multiObjectiveScrollLayout = new QVBoxLayout(multiObjectiveScrollWidget);
    multiObjectiveScrollLayout->setAlignment(Qt::AlignTop);
// 设置滚动区域的内容
    multiObjectiveScrollArea->setWidget(multiObjectiveScrollWidget);

    QVBoxLayout *multiObjectiveDisplayLayout = new QVBoxLayout(multiObjectiveDisplayFrame);
    multiObjectiveDisplayLayout->addWidget(multiObjectiveScrollArea);

    rightLayout->addWidget(multiObjectiveDisplayFrame);


    // 将左、中、右布局添加到主布局
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(centerLayout, 2);
    mainLayout->addLayout(rightLayout, 1);

    // 设置窗口的主布局
    setLayout(mainLayout);
    setFixedSize(1500, 800); // 设置主窗口的大小
}

void MainWindow::onQueryButtonClicked() {
    // 获取起点和终点
    QString startLocation = startComboBox->currentText();
    QString endLocation = endComboBox->currentText();

    std::cout << locationHash[startLocation] << " " << locationHash[endLocation] << std::endl;

    int start = locationHash[startLocation];
    int end = locationHash[endLocation];
    std::vector<int> shortestPath = aStar(adjacencyMatrix, mappoints, start, end);

    // 输出最短路径
    std::cout << "Shortest path from " << start << " to " << end << ": ";
    for (int node : shortestPath) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    // 清除上一条路径
    for (QGraphicsLineItem* item : currentPathItems) {
        scene->removeItem(item);
        delete item;  // 释放内存
    }
    currentPathItems.clear();  // 清空当前路径项列表

    for (int i = 0; i < shortestPath.size() - 1; i++) {
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(QLineF(mappoints[shortestPath[i]].position, mappoints[shortestPath[i + 1]].position));
        lineItem->setPen(QPen(Qt::blue, 2));  // 设置线条颜色和宽度
        scene->addItem(lineItem);
        currentPathItems.push_back(lineItem);  // 将新的线条项存入列表
    }
}

void MainWindow::onClearButtonClicked() {
    // 清除当前路径
    for (QGraphicsLineItem* item : currentPathItems) {
        scene->removeItem(item);
        delete item;  // 释放内存
    }
    currentPathItems.clear();  // 清空当前路径项列表
}

void MainWindow::onConfirmButtonClicked() {
    // 清除之前的标志和文字说明
    for (QGraphicsItem* item : previousMarkers) {
        scene->removeItem(item);
        delete item;  // 释放内存
    }
    previousMarkers.clear();  // 清空存储标志的列表

    // 获取用户在下拉框中选择的地点
    QString selectedLocation = currentLocationComboBox->currentText();
    int locationIndex = locationHash[selectedLocation];
    QPointF selectedPosition = mappoints[locationIndex].position;

    // 在地图上标记当前位置
    QGraphicsEllipseItem *marker = new QGraphicsEllipseItem(selectedPosition.x() - 5, selectedPosition.y() - 5, 10, 10);
    marker->setBrush(Qt::red);
    scene->addItem(marker);
    previousMarkers.push_back(marker);  // 将标志存储到列表中

    // 添加文字说明“当前位置”
    QGraphicsTextItem *text = new QGraphicsTextItem("当前位置");
    text->setPos(selectedPosition.x() + 10, selectedPosition.y() - 10);  // 文字位置
    text->setDefaultTextColor(Qt::red);
    scene->addItem(text);
    previousMarkers.push_back(text);  // 将文字说明存储到列表中

    // 获取当前视图的中心位置
    QPointF viewCenter = mapViewer->mapToScene(mapViewer->viewport()->rect().center());

    // 计算要移动的偏移量
    QPointF offset = selectedPosition - viewCenter;

    // 通过动画将地图视图移到当前位置的中心
    QPropertyAnimation *hAnimation = new QPropertyAnimation(mapViewer->horizontalScrollBar(), "value", this);
    hAnimation->setDuration(500);  // 设置动画持续时间
    hAnimation->setStartValue(mapViewer->horizontalScrollBar()->value());
    hAnimation->setEndValue(mapViewer->horizontalScrollBar()->value() + offset.x());
    hAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation *vAnimation = new QPropertyAnimation(mapViewer->verticalScrollBar(), "value", this);
    vAnimation->setDuration(500);
    vAnimation->setStartValue(mapViewer->verticalScrollBar()->value());
    vAnimation->setEndValue(mapViewer->verticalScrollBar()->value() + offset.y());
    vAnimation->start(QAbstractAnimation::DeleteWhenStopped);

}

void MainWindow::onShowPointsToggled(bool checked) {
    if (checked) {
        // 添加显示点的逻辑
        for (int i = 0; i < mappoints.size(); ++i) {
            // 绘制圆形标记
            QGraphicsEllipseItem *pointItem = new QGraphicsEllipseItem(mappoints[i].position.x() - 5, mappoints[i].position.y() - 5, 10, 10);
            pointItem->setBrush(Qt::red); // 设置为红色
            scene->addItem(pointItem);
            pointItems.push_back(pointItem);

            // 绘制文本标记
            QGraphicsTextItem *textItem = new QGraphicsTextItem(QString::number(i + 1));
            textItem->setPos(mappoints[i].position.x() + 5, mappoints[i].position.y() - 5); // 文本在点的右侧
            textItem->setDefaultTextColor(Qt::black);
            scene->addItem(textItem);
            pointItems.push_back(textItem);
        }
    } else {
        // 添加隐藏点的逻辑
        for (QGraphicsItem *item : pointItems) {
            scene->removeItem(item);
            delete item;
        }
        pointItems.clear(); // 清空列表
    }
}

void MainWindow::onShowEdgesToggled(bool checked) {
    if (checked) {
        // 添加显示边的逻辑

        // dfs
        // 初始化边标记矩阵
        std::vector<std::vector<bool>> visitedEdges(mappoints.size(), std::vector<bool>(mappoints.size(), false));

        // DFS 辅助函数
        std::function<void(int)> dfs = [&](int u) {
            for (int v = 0; v < mappoints.size(); ++v) {
                if (adjacencyMatrix[u][v] < std::numeric_limits<double>::infinity() && !visitedEdges[u][v]) {
                    // 标记边 u-v 作为访问过
                    visitedEdges[u][v] = true;
                    visitedEdges[v][u] = true; // 对无向图而言，u-v 和 v-u 是相同的边

                    // 绘制从 u 到 v 的边
                    QGraphicsLineItem *edgeItem = new QGraphicsLineItem(QLineF(mappoints[u].position, mappoints[v].position));
                    edgeItem->setPen(QPen(QColor(100, 149, 237), 2)); // 使用更深的蓝色绘制边
                    scene->addItem(edgeItem);
                    edgeItems.push_back(edgeItem);

                    // 继续递归访问
                    dfs(v);
                }
            }
        };

        // 对所有节点执行 DFS
        for (int i = 0; i < mappoints.size(); ++i) {
            dfs(i);
        }

        // bfs
        // BFS 队列
//        std::queue<int> bfsQueue;
//
//        // 对每个节点执行 BFS
//        for (int i = 0; i < mappoints.size(); ++i) {
//            if (bfsQueue.empty()) {
//                bfsQueue.push(i); // 从节点 i 开始
//            }
//
//            while (!bfsQueue.empty()) {
//                int u = bfsQueue.front();
//                bfsQueue.pop();
//
//                for (int v = 0; v < mappoints.size(); ++v) {
//                    if (adjacencyMatrix[u][v] < std::numeric_limits<double>::infinity() && !visitedEdges[u][v]) {
//                        // 标记边 u-v 作为访问过
//                        visitedEdges[u][v] = true;
//                        visitedEdges[v][u] = true; // 对无向图而言，u-v 和 v-u 是相同的边
//
//                        // 绘制从 u 到 v 的边
//                        QGraphicsLineItem *edgeItem = new QGraphicsLineItem(QLineF(mappoints[u].position, mappoints[v].position));
//                        edgeItem->setPen(QPen(QColor(100, 149, 237), 2)); // 使用更深的蓝色绘制边
//                        scene->addItem(edgeItem);
//                        edgeItems.push_back(edgeItem);
//
//                        // 将节点 v 加入 BFS 队列
//                        bfsQueue.push(v);
//                    }
//                }
//            }
//        }

    } else {
        // 添加隐藏边的逻辑
        for (QGraphicsLineItem *item : edgeItems) {
            scene->removeItem(item);
            delete item;
        }
        edgeItems.clear(); // 清空列表
    }
}

bool MainWindow::blockageExists(int firstPoint, int secondPoint, BlockageMode mode) {
    for (const auto& blockage : blockages) {
        if ((blockage.firstPoint == firstPoint && blockage.secondPoint == secondPoint) ||
            (blockage.firstPoint == secondPoint && blockage.secondPoint == firstPoint)) {

            // 如果已有双向路障，任何单向路障都不能再设置
            if (blockage.mode == BothDirections) {
                return true;
            }

            // 如果已有单向路障，再设置双向路障或者相同方向的单向路障时返回true
            if (blockage.mode == mode || mode == BothDirections) {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::onSetBlockageButtonClicked() {
    int firstPoint = firstPointComboBox->currentIndex() + 1;
    int secondPoint = secondPointComboBox->currentIndex() + 1;
    BlockageMode mode;

    QString modeString;
    if (fromOneToTwoCheckBox->isChecked() && fromTwoToOneCheckBox->isChecked()) {
        mode = BothDirections;
        modeString = QString("%1 号点 ↔ %2 号点").arg(firstPoint).arg(secondPoint);
    } else if (fromOneToTwoCheckBox->isChecked()) {
        mode = OneToTwo;
        modeString = QString("%1 号点 → %2 号点").arg(firstPoint).arg(secondPoint);
    } else if (fromTwoToOneCheckBox->isChecked()) {
        mode = TwoToOne;
        modeString = QString("%1 号点 ← %2 号点").arg(firstPoint).arg(secondPoint);
    } else {
        QMessageBox::warning(this, "错误", "请至少选择一个模式！");
        return;
    }

    // 检查路障是否已经存在
    if (blockageExists(firstPoint, secondPoint, mode)) {
        QMessageBox::warning(this, "错误", "路障已存在！");
        return;
    }

    // 检查邻接矩阵中是否存在直接路径
    if (adjacencyMatrix[firstPoint - 1][secondPoint - 1] == std::numeric_limits<double>::infinity()) {
        QMessageBox::warning(this, "错误", "两点之间没有直接路径！");
        return;
    }
    blockageCount++;
    // 创建路障结构体并存储
    Blockage newBlockage(firstPoint, secondPoint, mode);
    blockages.push_back(newBlockage);

    // 修改邻接矩阵，设置路障
    if (mode == OneToTwo || mode == BothDirections) {
        adjacencyMatrix[firstPoint - 1][secondPoint - 1] = std::numeric_limits<double>::infinity();
    }
    if (mode == TwoToOne || mode == BothDirections) {
        adjacencyMatrix[secondPoint - 1][firstPoint - 1] = std::numeric_limits<double>::infinity();
    }

    // 在地图上绘制路障标记
    QPointF midpoint = (mappoints[firstPoint - 1].position + mappoints[secondPoint - 1].position) / 2.0;
    QGraphicsEllipseItem *marker = new QGraphicsEllipseItem(midpoint.x() - 3, midpoint.y() - 3, 6, 6);
    marker->setBrush(Qt::black);
    scene->addItem(marker);
    blockageMarkers.push_back(marker);

    // 绘制文本标记
    QGraphicsTextItem *blockageNameLabel = new QGraphicsTextItem(QString("路障 %1").arg(blockageCount));
    blockageNameLabel->setPos(midpoint.x() + 10, midpoint.y() + 10);
    blockageNameLabel->setDefaultTextColor(Qt::black);
    scene->addItem(blockageNameLabel);
    blockageMarkers.push_back(blockageNameLabel);


    // 将路障展示项添加到 UI
    QHBoxLayout *blockageItemLayout = new QHBoxLayout;
    QLabel *blockageNameLabelUI = new QLabel(QString("路障 %1").arg(blockageCount));
    QLabel *blockageModeLabelUI = new QLabel(modeString);
    QPushButton *removeButton = new QPushButton("消除");
    removeButton->setFixedWidth(80);
    removeButton->setStyleSheet(
            "QPushButton {"
            "background-color: gray;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 5px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: lightgray;"
            "color: black;"
            "}"
    );

    blockageItemLayout->addWidget(blockageNameLabelUI);
    blockageItemLayout->addStretch();
    blockageItemLayout->addWidget(blockageModeLabelUI);
    blockageItemLayout->addStretch();
    blockageItemLayout->addWidget(removeButton);

    blockScrollLayout->addLayout(blockageItemLayout);
    blockageLayouts.push_back(blockageItemLayout);

    // 连接“消除”按钮的点击事件
    connect(removeButton, &QPushButton::clicked, this, [=]() {
        onRemoveBlockageButtonClicked(blockageItemLayout, marker, blockageNameLabel);
    });
}

void MainWindow::onRemoveBlockageButtonClicked(QHBoxLayout *blockageItemLayout, QGraphicsEllipseItem *marker, QGraphicsTextItem *blockageNameLabel) {
    // 找到要删除的路障索引
    int indexToRemove = -1;
    for (int i = 0; i < blockageLayouts.size(); ++i) {
        if (blockageLayouts[i] == blockageItemLayout) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove != -1) {
        // 获取要删除的路障信息
        Blockage blockageToRemove = blockages[indexToRemove];
        int firstPoint = blockageToRemove.firstPoint;
        int secondPoint = blockageToRemove.secondPoint;
        BlockageMode mode = blockageToRemove.mode;

        // 恢复邻接矩阵中的权值，使用点的坐标计算两点之间的距离平方
        double originalWeight = std::pow(mappoints[firstPoint - 1].position.x() - mappoints[secondPoint - 1].position.x(), 2) +
                                std::pow(mappoints[firstPoint - 1].position.y() - mappoints[secondPoint - 1].position.y(), 2);

        if (mode == OneToTwo || mode == BothDirections) {
            adjacencyMatrix[firstPoint - 1][secondPoint - 1] = originalWeight;
        }
        if (mode == TwoToOne || mode == BothDirections) {
            adjacencyMatrix[secondPoint - 1][firstPoint - 1] = originalWeight;
        }

        // 从 blockages 向量中移除对应的路障
        blockages.erase(blockages.begin() + indexToRemove);

        // 从 blockageLayouts 向量中移除该布局
        blockageLayouts.erase(blockageLayouts.begin() + indexToRemove);
    }

    // 从图形场景中移除路障标记
    scene->removeItem(marker);
    delete marker;

    // 从图形场景中移除文本标记
    scene->removeItem(blockageNameLabel);
    delete blockageNameLabel;

    while (QLayoutItem* item = blockageItemLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater(); // 删除子部件
        }
        delete item; // 删除布局项
    }

    // 从父布局中移除该布局
    blockageItemLayout->parentWidget()->layout()->removeItem(blockageItemLayout);

    // 删除该布局
    delete blockageItemLayout;

    // 从 blockageLayouts 向量中移除该布局
    blockageLayouts.erase(std::remove(blockageLayouts.begin(), blockageLayouts.end(), blockageItemLayout), blockageLayouts.end());
}

void MainWindow::clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater(); // 延迟删除控件
        }
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout); // 递归删除子布局
        }
        delete item;
    }
}

void MainWindow::updateDisplay() {
    // 清除当前显示的内容
    clearLayout(multiObjectiveScrollLayout);

    LocationNode* current = head;
    if (head == nullptr) return;
    int index = 1;
    do {
        qDebug() << current->name;
        QHBoxLayout* itemLayout = new QHBoxLayout;
        QLabel* sequenceLabel = new QLabel(QString::number(index++));
        QLabel* spotNameLabel = new QLabel(current->name);

        QPushButton* upButton = new QPushButton("↑");
        QPushButton* downButton = new QPushButton("↓");
        QPushButton* cutButton = new QPushButton("cut");

        // 设置按钮样式
        upButton->setFixedWidth(40);
        downButton->setFixedWidth(40);
        cutButton->setFixedWidth(60);

        itemLayout->addWidget(sequenceLabel);
        itemLayout->addStretch();
        itemLayout->addWidget(spotNameLabel);
        itemLayout->addStretch();
        itemLayout->addWidget(upButton);
        itemLayout->addWidget(downButton);
        itemLayout->addWidget(cutButton);

        multiObjectiveScrollLayout->addLayout(itemLayout);

        connect(upButton, &QPushButton::clicked, this, [=]() {
            moveUp(current);
        });
        connect(downButton, &QPushButton::clicked, this, [=]() {
            moveDown(current);
        });
        connect(cutButton, &QPushButton::clicked, this, [=]() {
            removeLocationFromList(current);
        });



        current = current->next;
    } while (current != head);
    qDebug() << endl;
}

void MainWindow::addLocationToList(const QString& name) {
    LocationNode* newNode = new LocationNode(name);

    if (!head) {
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        LocationNode* tail = head->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = head;
        head->prev = newNode;
    }
    listSize++;
    updateDisplay();  // 更新 UI 显示
}

void MainWindow::onAddLocationButtonClicked() {
    QString selectedLocation;
    if (spotNameRadioButton->isChecked()) {
        selectedLocation = locationComboBox->currentText();
    } else if (sequenceRadioButton->isChecked()) {
        int selectedIndex = locationComboBox->currentIndex() + 1;
        selectedLocation = QString::number(selectedIndex);
    }

    addLocationToList(selectedLocation);

}

void MainWindow::moveUp(LocationNode* node) {
    if (!node || listSize <= 1) return;

    LocationNode* prevNode = node->prev;

    // 如果 node 是头节点，将头更新为前一个节点
    if (node == head) {
        head = node->next;
    } else {
        // 交换节点位置
        prevNode->next = node->next;
        node->next->prev = prevNode;

        node->next = prevNode;
        node->prev = prevNode->prev;

        prevNode->prev->next = node;
        prevNode->prev = node;
        if (prevNode == head) {
            head = node;
        }
    }



    updateDisplay();
}

void MainWindow::moveDown(LocationNode* node) {
    if (!node || listSize <= 1) return;

    LocationNode* nextNode = node->next;

    // 如果 node 的下一个节点是头节点，更新头节点为当前节点

    if (nextNode == head) {
        head = node;
    } else {
        // 交换节点位置
        node->prev->next = nextNode;
        nextNode->next->prev = node;

        nextNode->prev = node->prev;
        node->next = nextNode->next;

        nextNode->next = node;
        node->prev = nextNode;
        if (node == head) {
            head = nextNode;
        }
    }



    updateDisplay();
}

void MainWindow::removeLocationFromList(LocationNode* node) {
    if (!node) return;  // 如果节点为空，直接返回

    if (listSize == 1) {
        // 链表中只有一个节点
        delete node;
        head = nullptr;
        listSize = 0;
    } else {
        // 链表中有多个节点
        if (node == head) {
            head = head->next;
        }

        node->prev->next = node->next;
        node->next->prev = node->prev;

        delete node;
        listSize--;

        // 确保删除后链表中的节点不指向自己，避免形成循环
        if (listSize == 1) {
            head->next = head;
            head->prev = head;
        }
    }

    updateDisplay();
}

std::vector<int> MainWindow::extractDestinations() {
    std::vector<int> destinations;
    LocationNode* current = head;
    if (!current) return destinations;  // 空链表

    do {
        QString locationName = current->name;
        int index = -1;

        // 使用 LocationHash 查找对应的序号
        if (locationHash.find(locationName) != locationHash.end()) {
            index = locationHash[locationName];
        } else {
            // 如果地点名是序号模式，则直接转换为索引
            bool isNumber = false;
            int number = locationName.toInt(&isNumber);
            if (isNumber) {
                index = number - 1; // 序号是从1开始，而索引从0开始
            }
        }

        if (index != -1) {
            destinations.push_back(index);
        }

        current = current->next;
    } while (current != head);

    return destinations;
}

double MainWindow::calculatePathDistance(const std::vector<int>& path) {
    double totalDistance = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalDistance += adjacencyMatrix[path[i]][path[i + 1]];
    }
    return totalDistance;
}

double MainWindow::calculatePathDistance2(const std::vector<int>& path, const std::vector<std::vector<double>>& subMatrix) {
    double totalDistance = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        totalDistance += subMatrix[path[i]][path[i + 1]];
    }
    return totalDistance;
}

std::vector<int> MainWindow::findOptimalPath(const std::vector<int>& destinations) {
    int n = destinations.size(); // 目的地的数量
    std::vector<std::vector<double>> dp(1 << n, std::vector<double>(n, std::numeric_limits<double>::infinity()));
    std::vector<std::vector<int>> path(1 << n, std::vector<int>(n, -1));

    dp[1][0] = 0; // 从起点出发，初始路径长度为0

    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) continue; // 如果 u 不在集合中，跳过

            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue; // 如果 v 在集合中，跳过

                int newMask = mask | (1 << v);
                std::vector<int> shortestPath = dijkstra(adjacencyMatrix, destinations[u], destinations[v]);
                double newDist = dp[mask][u] + calculatePathDistance(shortestPath);

                if (newDist < dp[newMask][v]) {
                    dp[newMask][v] = newDist;
                    path[newMask][v] = u;
                }
            }
        }
    }

    // 回溯找到最短路径
    double minLength = std::numeric_limits<double>::infinity();
    int lastNode = -1;
    for (int u = 1; u < n; ++u) {
        std::vector<int> shortestPath = dijkstra(adjacencyMatrix, destinations[u], destinations[0]);
        double curLength = dp[(1 << n) - 1][u] + calculatePathDistance(shortestPath);
        if (curLength < minLength) {
            minLength = curLength;
            lastNode = u;
        }
    }

    std::vector<int> optimalPath;
    int mask = (1 << n) - 1;
    while (lastNode != -1) {
        optimalPath.push_back(lastNode);
        int temp = lastNode;
        lastNode = path[mask][lastNode];
        mask ^= (1 << temp);
    }
    optimalPath.push_back(0);
    std::reverse(optimalPath.begin(), optimalPath.end());

    // 映射回 mappoints 的索引
    for (int& index : optimalPath) {
        index = destinations[index];
    }

    return optimalPath;
}

// 定义适应度函数，根据路径的总距离来计算
double MainWindow::fitnessFunction(const std::vector<int>& path, const std::vector<std::vector<double>>& subMatrix) {
    double totalDistance = calculatePathDistance2(path, subMatrix);
    return 1.0 / totalDistance;  // 适应度值为路径距离的倒数
}

// 随机生成初始种群
std::vector<std::vector<int>> MainWindow::generateInitialPopulation(int populationSize, int numCities) {
    std::vector<std::vector<int>> population;
    std::vector<int> basePath(numCities);
    std::iota(basePath.begin(), basePath.end(), 0); // 生成从0到numCities-1的序列

    for (int i = 0; i < populationSize; ++i) {
        std::vector<int> individual(basePath);
        std::random_shuffle(individual.begin() + 1, individual.end());  // 保证起点不变，随机排列其余部分
        population.push_back(individual);
    }
    return population;
}

// 选择最优个体
std::vector<int> MainWindow::selection(const std::vector<std::vector<int>>& population, const std::vector<double>& fitnesses) {
    std::discrete_distribution<int> distribution(fitnesses.begin(), fitnesses.end());
    std::random_device rd;
    std::mt19937 gen(rd());
    return population[distribution(gen)];
}

// 交叉操作 (部分映射交叉)
std::vector<int> MainWindow::crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int size = parent1.size();
    std::vector<int> offspring(size, -1);

    int start = std::rand() % size;
    int end = std::rand() % size;

    if (start > end) std::swap(start, end);

    std::copy(parent1.begin() + start, parent1.begin() + end, offspring.begin() + start);

    for (int i = 0; i < size; ++i) {
        if (offspring[i] == -1) {
            int gene = parent2[i];
            while (std::find(offspring.begin(), offspring.end(), gene) != offspring.end()) {
                gene = parent2[std::distance(parent2.begin(), std::find(parent2.begin(), parent2.end(), gene))];
            }
            offspring[i] = gene;
        }
    }
    return offspring;
}

// 变异操作
void MainWindow::mutate(std::vector<int>& individual) {
    int size = individual.size();
    int index1 = std::rand() % size;
    int index2 = std::rand() % size;
    std::swap(individual[index1], individual[index2]);
}

// 遗传算法求解TSP
std::vector<int> MainWindow::geneticAlgorithmTSP(const std::vector<int>& destinations, int populationSize, int generations, double mutationRate) {
    int numCities = destinations.size();

    // 提取出只包含目的地的最短路径子矩阵
    std::vector<std::vector<double>> subMatrix(numCities, std::vector<double>(numCities));
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (i != j) {
                std::vector<int> shortestPath = dijkstra(adjacencyMatrix, destinations[i], destinations[j]);
                subMatrix[i][j] = calculatePathDistance(shortestPath);
            } else {
                subMatrix[i][j] = 0; // 同一点的距离为0
            }
        }
    }

    std::vector<std::vector<int>> population = generateInitialPopulation(populationSize, numCities);
    std::vector<double> fitnesses(populationSize);

    for (int generation = 0; generation < generations; ++generation) {
        // 计算每个个体的适应度
        for (int i = 0; i < populationSize; ++i) {
            fitnesses[i] = fitnessFunction(population[i], subMatrix);
        }

        std::vector<std::vector<int>> newPopulation;

        // 生成新一代
        for (int i = 0; i < populationSize; ++i) {
            std::vector<int> parent1 = selection(population, fitnesses);
            std::vector<int> parent2 = selection(population, fitnesses);
            std::vector<int> offspring = crossover(parent1, parent2);

            if ((std::rand() / double(RAND_MAX)) < mutationRate) {
                mutate(offspring);
            }

            newPopulation.push_back(offspring);
        }

        population = newPopulation;
    }

    // 找到最优解
    int bestIndex = std::distance(fitnesses.begin(), std::max_element(fitnesses.begin(), fitnesses.end()));
    std::vector<int> bestPath = population[bestIndex];

    // 将结果映射回原来的地点索引
    std::vector<int> optimalPath;
    for (int cityIndex : bestPath) {
        optimalPath.push_back(destinations[cityIndex]);
    }

    return optimalPath;
}

void MainWindow::onPlanButtonClicked() {
    std::vector<int> destinations = extractDestinations();
    if (destinations.empty()) {
        QMessageBox::warning(this, "错误", "没有选择有效的目的地！");
        return;
    }

    // 删除所有路径和标记
    for (QGraphicsItem* item : mutiCurrentPathItems) {
        scene->removeItem(item);
        delete item;
    }
    mutiCurrentPathItems.clear();

    std::vector<int> optimalPath;

    if (orderedRadioButton->isChecked()) {
        // 有序的情况，逐对计算相邻地点之间的最短路径
        for (size_t i = 0; i < destinations.size() - 1; ++i) {
            std::vector<int> segment = dijkstra(adjacencyMatrix, destinations[i], destinations[i + 1]);
            if (i == 0) {
                optimalPath.insert(optimalPath.end(), segment.begin(), segment.end());
            } else {
                optimalPath.insert(optimalPath.end(), segment.begin() + 1, segment.end()); // 避免重复第一个点
            }
        }
    } else {
        // 无序的情况，使用TSP算法计算最优路径
        optimalPath = findOptimalPath(destinations);

        // 如果选中了“细节”，则为optimalPath添加中间点
        if (detailsCheckBox->isChecked()) {
            std::vector<int> detailedOptimalPath;
            for (size_t i = 0; i < optimalPath.size() - 1; ++i) {
                std::vector<int> segment = dijkstra(adjacencyMatrix, optimalPath[i], optimalPath[i + 1]);
                if (i == 0) {
                    detailedOptimalPath.insert(detailedOptimalPath.end(), segment.begin(), segment.end());
                } else {
                    detailedOptimalPath.insert(detailedOptimalPath.end(), segment.begin() + 1, segment.end()); // 避免重复第一个点
                }
            }
            optimalPath = detailedOptimalPath;
        }
    }

    // 输出路径的索引序列
    for(auto op : optimalPath) {
        qDebug() << op;
    }

    // 在地图上绘制路径
    for (size_t i = 0; i < optimalPath.size() - 1; ++i) {
        int u = optimalPath[i];
        int v = optimalPath[i + 1];

        QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(mappoints[u].position, mappoints[v].position));
        lineItem->setPen(QPen(Qt::blue, 2));
        scene->addItem(lineItem);
        mutiCurrentPathItems.push_back(lineItem);

        // 标记位置
        QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(mappoints[u].position.x() - 5, mappoints[u].position.y() - 5, 10, 10);
        pointItem->setBrush(Qt::red);
        scene->addItem(pointItem);
        mutiCurrentPathItems.push_back(pointItem);

        QGraphicsTextItem* textItem = new QGraphicsTextItem(QString::number(u + 1));
        textItem->setPos(mappoints[u].position.x() + 5, mappoints[u].position.y() - 5);
        textItem->setDefaultTextColor(Qt::black);
        scene->addItem(textItem);
        mutiCurrentPathItems.push_back(textItem);
    }
}

void MainWindow::onMutiClearButtonClicked() {
    // 删除所有路径和标记
    for (QGraphicsItem* item : mutiCurrentPathItems) {
        scene->removeItem(item);
        delete item;
    }
    mutiCurrentPathItems.clear();
}





