#include "MainWindow.h"
#include "../include/Utils.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QDateTime>
#include <QAbstractItemView>
#include <QApplication>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), database(new PlateDatabase())
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    refreshTable();
    updateStatusBar("系统就绪");
    updateActionStates();
}

MainWindow::~MainWindow()
{
    delete database;
}

void MainWindow::setupUI()
{
    setWindowTitle("辽宁省汽车牌照快速查询系统 v2.0");
    setMinimumSize(1200, 800);
    resize(1400, 900);

    // 全局样式美化
    setStyleSheet(R"(
        QWidget {
            font-family: "Microsoft YaHei", "Source Han Sans", "PingFang SC", sans-serif;
            font-size: 15px;
            color: #1f2328;
        }
        QMainWindow {
            background: #eef1f6;
        }
        QGroupBox {
            border: 1px solid #c8d2e3;
            border-radius: 10px;
            margin-top: 12px;
            padding: 14px 12px 10px 12px;
            background: #ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 4px;
            background: #eef1f6;
            color: #1f4b99;
            font-weight: 700;
            font-size: 14px;
        }
        QLabel {
            color: #2d3748;
        }
        QLineEdit, QComboBox, QTextEdit {
            border: 1px solid #b8c3d4;
            border-radius: 8px;
            padding: 8px;
            background: #ffffff;
        }
        QLineEdit:focus, QComboBox:focus, QTextEdit:focus {
            border: 1px solid #2b7de9;
            box-shadow: 0 0 0 3px rgba(43, 125, 233, 0.18);
        }
        QPushButton {
            border-radius: 10px;
            padding: 10px 18px;
            font-weight: 700;
            min-width: 120px;
            min-height: 36px;
        }
        QPushButton[btnRole="primary"] {
            background: #2b7de9;
            color: #ffffff;
            border: 1px solid #1f6fdb;
            box-shadow: 0 6px 14px rgba(43, 125, 233, 0.22);
        }
        QPushButton[btnRole="primary"]:hover {
            background: #1f6fdb;
            border-color: #1a63c5;
            box-shadow: 0 6px 14px rgba(31, 111, 219, 0.28);
        }
        QPushButton[btnRole="primary"]:pressed {
            background: #1a63c5;
            border-color: #1555ad;
            box-shadow: none;
        }
        QPushButton[btnRole="secondary"] {
            background: #eef2f7;
            color: #1f2328;
            border: 1px solid #c8d2e3;
        }
        QPushButton[btnRole="secondary"]:hover {
            background: #dfe6f0;
            border-color: #b8c3d4;
        }
        QPushButton[btnRole="secondary"]:pressed {
            background: #d2dbe7;
        }
        QPushButton[btnRole="neutral"] {
            background: #ffffff;
            color: #1f2328;
            border: 1px solid #d0d7de;
        }
        QPushButton[btnRole="neutral"]:hover {
            background: #f4f6f9;
        }
        QPushButton[btnRole="neutral"]:pressed {
            background: #e9edf3;
        }
        QPushButton[btnRole="danger"] {
            background: #f15151;
            color: #ffffff;
            border: 1px solid #d94141;
        }
        QPushButton[btnRole="danger"]:hover {
            background: #e04444;
            border-color: #c83a3a;
        }
        QPushButton[btnRole="danger"]:pressed {
            background: #c83a3a;
            border-color: #a83030;
        }
        QPushButton:disabled {
            background: #e4e7ed;
            color: #7a869a;
            border: 1px solid #cfd6e1;
            box-shadow: none;
        }
        QTableWidget {
            background: #ffffff;
            alternate-background-color: #f6f8fb;
            gridline-color: #e1e6ee;
            border: 1px solid #c8d2e3;
            border-radius: 10px;
        }
        QHeaderView::section {
            background: #eef2f7;
            padding: 8px;
            border: none;
            border-right: 1px solid #d0d7de;
            font-weight: 800;
            font-size: 14px;
            color: #1f2328;
        }
        QStatusBar {
            background: #f6f8fa;
            border-top: 1px solid #d0d7de;
        }
    )");
    
    // 配置全局占位符颜色
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::PlaceholderText, QColor("#bfc5cc"));
    qApp->setPalette(pal);
    
    // 保存基础字体用于缩放（基于当前应用字体）
    baseFont = QApplication::font();
    fontScale = 1.0;
    
    // 中央部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    
    // 左侧：数据录入和管理面板
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(12);
    
    // 数据录入组
    QGroupBox* inputGroup = new QGroupBox("数据维护", this);
    QGridLayout* inputLayout = new QGridLayout();
    
    QLabel* plateLabel = new QLabel("车牌号:", this);
    plateLabel->setFixedWidth(70);
    inputLayout->addWidget(plateLabel, 0, 0);
    plateEdit = new QLineEdit(this);
    plateEdit->setMinimumHeight(32);
    plateEdit->setPlaceholderText("例如: 燃油车 辽B7238A 或 新能源 辽BDF12345");
    plateEdit->setMaxLength(12); // 兼容新能源车更长车牌
    inputLayout->addWidget(plateEdit, 0, 1);
    
    QLabel* cityLabelLeft = new QLabel("城市:", this);
    cityLabelLeft->setFixedWidth(70);
    inputLayout->addWidget(cityLabelLeft, 1, 0);
    cityEdit = new QLineEdit(this);
    cityEdit->setMinimumHeight(32);
    cityEdit->setPlaceholderText("例如: 沈阳");
    inputLayout->addWidget(cityEdit, 1, 1);
    
    QLabel* ownerLabel = new QLabel("车主:", this);
    ownerLabel->setFixedWidth(70);
    inputLayout->addWidget(ownerLabel, 2, 0);
    ownerEdit = new QLineEdit(this);
    ownerEdit->setMinimumHeight(32);
    ownerEdit->setPlaceholderText("例如: 张三");
    inputLayout->addWidget(ownerEdit, 2, 1);
    
    addBtn = new QPushButton("添加记录", this);
    addBtn->setProperty("btnRole", "primary");
    modifyBtn = new QPushButton("修改记录", this);
    modifyBtn->setProperty("btnRole", "secondary");
    deleteBtn = new QPushButton("删除记录", this);
    deleteBtn->setProperty("btnRole", "danger");
    
    QHBoxLayout* btnLayout1 = new QHBoxLayout();
    btnLayout1->addWidget(addBtn);
    btnLayout1->addWidget(modifyBtn);
    btnLayout1->addWidget(deleteBtn);
    inputLayout->addLayout(btnLayout1, 3, 0, 1, 2);
    
    loadFileBtn = new QPushButton("从文件导入", this);
    loadFileBtn->setProperty("btnRole", "primary");
    randomBtn = new QPushButton("随机生成", this);
    randomBtn->setProperty("btnRole", "secondary");
    QHBoxLayout* btnLayout2 = new QHBoxLayout();
    btnLayout2->addWidget(loadFileBtn);
    btnLayout2->addWidget(randomBtn);
    inputLayout->addLayout(btnLayout2, 4, 0, 1, 2);
    
    inputGroup->setLayout(inputLayout);
    leftLayout->addWidget(inputGroup);
    
    // 查找组
    QGroupBox* searchGroup = new QGroupBox("查询与定位", this);
    QVBoxLayout* searchLayout = new QVBoxLayout();
    searchLayout->setSpacing(10);
    
    QLabel* exactLabel = new QLabel("精确查找（折半）：", this);
    searchLayout->addWidget(exactLabel);
    searchEdit = new QLineEdit(this);
    searchEdit->setMinimumHeight(30);
    searchEdit->setPlaceholderText("输入完整车牌号 (需先排序)");
    searchEdit->setToolTip("折半查找（二分查找）：\n"
                          "1. 先点击\"链式基数排序\"对车牌排序\n"
                          "2. 然后输入完整车牌号进行精确查找\n"
                          "3. 查找速度快，时间复杂度O(log n)");
    QHBoxLayout* exactRow = new QHBoxLayout();
    searchBtn = new QPushButton("折半查找", this);
    searchBtn->setProperty("btnRole", "secondary");
    exactRow->addWidget(searchEdit);
    exactRow->addWidget(searchBtn);
    searchLayout->addLayout(exactRow);
    
    QLabel* prefixLabel = new QLabel("前缀查找：", this);
    searchLayout->addWidget(prefixLabel);
    prefixEdit = new QLineEdit(this);
    prefixEdit->setMinimumHeight(30);
    prefixEdit->setPlaceholderText("例如: 辽B72 或 辽BDF1 (输入车牌前几位)");
    prefixEdit->setToolTip("前缀查找：输入车牌的前几个字符进行模糊匹配\n"
                          "示例：\n"
                          "• 输入\"辽B\"查找所有辽B开头的车牌\n"
                          "• 输入\"辽B72\"查找所有辽B72开头的车牌\n"
                          "• 输入\"辽BDF1\"查找新能源车牌前缀");
    QHBoxLayout* prefixRow = new QHBoxLayout();
    prefixSearchBtn = new QPushButton("前缀查找", this);
    prefixSearchBtn->setProperty("btnRole", "secondary");
    prefixRow->addWidget(prefixEdit);
    prefixRow->addWidget(prefixSearchBtn);
    searchLayout->addLayout(prefixRow);
    
    QLabel* cityLabel = new QLabel("城市查找：", this);
    searchLayout->addWidget(cityLabel);
    QHBoxLayout* cityRow = new QHBoxLayout();
    cityCombo = new QComboBox(this);
    cityCombo->setMinimumHeight(30);
    cityCombo->setEditable(true);
    cityCombo->addItems({"沈阳", "大连", "鞍山", "抚顺", "本溪", "丹东", 
                         "锦州", "营口", "阜新", "辽阳", "盘锦", "铁岭", "朝阳", "葫芦岛"});
    citySearchBtn = new QPushButton("城市查找", this);
    citySearchBtn->setProperty("btnRole", "secondary");
    cityRow->addWidget(cityCombo);
    cityRow->addWidget(citySearchBtn);
    searchLayout->addLayout(cityRow);
    
    searchGroup->setLayout(searchLayout);
    leftLayout->addWidget(searchGroup);
    
    // 操作组
    QGroupBox* operationGroup = new QGroupBox("数据处理", this);
    QVBoxLayout* operationLayout = new QVBoxLayout();
    
    sortBtn = new QPushButton("链式基数排序", this);
    sortBtn->setProperty("btnRole", "neutral");
    statsBtn = new QPushButton("统计信息", this);
    statsBtn->setProperty("btnRole", "neutral");
    cityStatsBtn = new QPushButton("城市统计排序", this);
    cityStatsBtn->setProperty("btnRole", "neutral");
    perfBtn = new QPushButton("性能统计", this);
    perfBtn->setProperty("btnRole", "neutral");
    validateBtn = new QPushButton("数据验证", this);
    validateBtn->setProperty("btnRole", "neutral");
    
    operationLayout->addWidget(sortBtn);
    operationLayout->addWidget(statsBtn);
    operationLayout->addWidget(cityStatsBtn);
    operationLayout->addWidget(perfBtn);
    operationLayout->addWidget(validateBtn);
    
    operationGroup->setLayout(operationLayout);
    leftLayout->addWidget(operationGroup);
    
    // 文件操作组
    QGroupBox* fileGroup = new QGroupBox("数据文件", this);
    QVBoxLayout* fileLayout = new QVBoxLayout();
    
    saveFileBtn = new QPushButton("保存到文件", this);
    saveFileBtn->setProperty("btnRole", "secondary");
    clearBtn = new QPushButton("清空数据", this);
    clearBtn->setProperty("btnRole", "danger");
    
    fileLayout->addWidget(saveFileBtn);
    fileLayout->addWidget(clearBtn);
    
    fileGroup->setLayout(fileLayout);
    leftLayout->addWidget(fileGroup);
    
    leftLayout->addStretch();
    
    // 中间：数据表格
    QVBoxLayout* centerLayout = new QVBoxLayout();
    centerLayout->setContentsMargins(0,0,0,0);
    
    QLabel* tableLabel = new QLabel("车牌记录列表", this);
    tableLabel->setStyleSheet("font-size: 15px; font-weight: bold; color:#1f4b99;");
    centerLayout->addWidget(tableLabel);
    
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "序号" << "车牌号" << "城市" << "车主" << "类别");
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setDefaultSectionSize(130);
    tableWidget->setColumnWidth(0, 50); // 序号列宽度较小
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:#F2F4F7;font-weight:800;font-size:13.5px;color:#1f2328;}");
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setDefaultSectionSize(34);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setShowGrid(false);
    tableWidget->setFrameShape(QFrame::NoFrame);
    
    emptyStateLabel = new QLabel("暂无车辆数据\n请在左侧添加记录或导入文件", this);
    emptyStateLabel->setAlignment(Qt::AlignCenter);
    emptyStateLabel->setStyleSheet("color:#8a94a6; font-size:14px;");
    emptyStateLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    
    // 使用叠加布局让空状态覆盖表格
    stackedLayout = new QStackedLayout();
    QWidget* tableContainer = new QWidget(this);
    QVBoxLayout* tableContainerLayout = new QVBoxLayout(tableContainer);
    tableContainerLayout->setContentsMargins(0,0,0,0);
    tableContainerLayout->addWidget(tableWidget);
    QWidget* emptyWrapper = new QWidget(this);
    QVBoxLayout* emptyLayout = new QVBoxLayout(emptyWrapper);
    emptyLayout->setContentsMargins(0,0,0,0);
    emptyLayout->addWidget(emptyStateLabel);
    stackedLayout->addWidget(tableContainer);
    stackedLayout->addWidget(emptyWrapper);
    stackedLayout->setCurrentIndex(1); // 默认空状态
    centerLayout->addLayout(stackedLayout);
    
    // 右侧：信息显示
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    QLabel* infoLabel = new QLabel("系统日志", this);
    infoLabel->setStyleSheet("font-size: 14px; font-weight: bold; color:#4a5568;");
    rightLayout->addWidget(infoLabel);
    
    infoText = new QTextEdit(this);
    infoText->setReadOnly(true);
    infoText->setMaximumWidth(240);
    infoText->setMinimumWidth(200);
    infoText->setStyleSheet("background:#f8fafc; border:1px solid #e2e8f0; border-radius:10px; font-size:12px; color:#4a5568;");
    rightLayout->addWidget(infoText);
    
    // 连接信号和槽
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddRecord);
    connect(modifyBtn, &QPushButton::clicked, this, &MainWindow::onModifyRecord);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteRecord);
    connect(loadFileBtn, &QPushButton::clicked, this, &MainWindow::onLoadFromFile);
    connect(randomBtn, &QPushButton::clicked, this, &MainWindow::onGenerateRandom);
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onBinarySearch);
    connect(prefixSearchBtn, &QPushButton::clicked, this, &MainWindow::onPrefixSearch);
    connect(citySearchBtn, &QPushButton::clicked, this, &MainWindow::onCitySearch);
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onRadixSort);
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::onStatistics);
    connect(cityStatsBtn, &QPushButton::clicked, this, &MainWindow::onCityStatistics);
    connect(perfBtn, &QPushButton::clicked, this, &MainWindow::onPerformanceStats);
    connect(validateBtn, &QPushButton::clicked, this, &MainWindow::onValidateData);
    connect(saveFileBtn, &QPushButton::clicked, this, &MainWindow::onSaveToFile);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearAll);
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClick);
    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);
    connect(plateEdit, &QLineEdit::textChanged, this, &MainWindow::onPlateTextChanged);
    connect(cityEdit, &QLineEdit::textChanged, this, &MainWindow::onCityTextChanged);
    
    // 布局
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(centerLayout, 3);
    mainLayout->addLayout(rightLayout, 1);
}

void MainWindow::setupMenuBar()
{
    QMenuBar* menuBar = this->menuBar();
    
    QMenu* fileMenu = menuBar->addMenu("文件(&F)");
    QAction* loadAction = fileMenu->addAction("导入文件(&I)");
    QAction* saveAction = fileMenu->addAction("保存文件(&S)");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("退出(&X)");
    
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadFromFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveToFile);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    QMenu* editMenu = menuBar->addMenu("编辑(&E)");
    QAction* addAction = editMenu->addAction("添加记录(&A)");
    QAction* modifyAction = editMenu->addAction("修改记录(&M)");
    QAction* deleteAction = editMenu->addAction("删除记录(&D)");
    editMenu->addSeparator();
    QAction* clearAction = editMenu->addAction("清空数据(&C)");
    
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddRecord);
    connect(modifyAction, &QAction::triggered, this, &MainWindow::onModifyRecord);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteRecord);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearAll);
    
    QMenu* searchMenu = menuBar->addMenu("查找(&S)");
    QAction* binaryAction = searchMenu->addAction("折半查找(&B)");
    QAction* cityAction = searchMenu->addAction("城市查找(&C)");
    QAction* prefixAction = searchMenu->addAction("前缀查找(&P)");
    
    connect(binaryAction, &QAction::triggered, this, &MainWindow::onBinarySearch);
    connect(cityAction, &QAction::triggered, this, &MainWindow::onCitySearch);
    connect(prefixAction, &QAction::triggered, this, &MainWindow::onPrefixSearch);
    
    QMenu* toolsMenu = menuBar->addMenu("工具(&T)");
    QAction* sortAction = toolsMenu->addAction("基数排序(&R)");
    QAction* statsAction = toolsMenu->addAction("统计信息(&S)");
    QAction* perfAction = toolsMenu->addAction("性能统计(&P)");
    
    connect(sortAction, &QAction::triggered, this, &MainWindow::onRadixSort);
    connect(statsAction, &QAction::triggered, this, &MainWindow::onStatistics);
    connect(perfAction, &QAction::triggered, this, &MainWindow::onPerformanceStats);
    
    QMenu* viewMenu = menuBar->addMenu("视图(&V)");
    QAction* fontUpAction = viewMenu->addAction("字体放大 (A+)");
    QAction* fontDownAction = viewMenu->addAction("字体缩小 (A-)");
    QAction* fontResetAction = viewMenu->addAction("恢复默认字体");
    
    connect(fontUpAction, &QAction::triggered, this, &MainWindow::onFontIncrease);
    connect(fontDownAction, &QAction::triggered, this, &MainWindow::onFontDecrease);
    connect(fontResetAction, &QAction::triggered, this, &MainWindow::onFontReset);
    
    QMenu* helpMenu = menuBar->addMenu("帮助(&H)");
    QAction* helpAction = helpMenu->addAction("使用帮助(&H)");
    QAction* aboutAction = helpMenu->addAction("关于(&A)");
    
    connect(helpAction, &QAction::triggered, this, &MainWindow::onHelp);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupStatusBar()
{
    statusLabel = new QLabel("就绪", this);
    statusBar()->addWidget(statusLabel);
    
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    statusBar()->addPermanentWidget(progressBar);
    
    // 字体调节按钮
    QWidget* fontWidget = new QWidget(this);
    QHBoxLayout* fontLayout = new QHBoxLayout(fontWidget);
    fontLayout->setContentsMargins(0,0,0,0);
    QPushButton* fontDownBtn = new QPushButton("A-", this);
    fontDownBtn->setProperty("btnRole", "neutral");
    fontDownBtn->setFixedWidth(44);
    QPushButton* fontResetBtn = new QPushButton("A", this);
    fontResetBtn->setProperty("btnRole", "neutral");
    fontResetBtn->setFixedWidth(36);
    QPushButton* fontUpBtn = new QPushButton("A+", this);
    fontUpBtn->setProperty("btnRole", "neutral");
    fontUpBtn->setFixedWidth(44);
    fontLayout->addWidget(fontDownBtn);
    fontLayout->addWidget(fontResetBtn);
    fontLayout->addWidget(fontUpBtn);
    statusBar()->addPermanentWidget(fontWidget);
    
    connect(fontDownBtn, &QPushButton::clicked, this, &MainWindow::onFontDecrease);
    connect(fontResetBtn, &QPushButton::clicked, this, &MainWindow::onFontReset);
    connect(fontUpBtn, &QPushButton::clicked, this, &MainWindow::onFontIncrease);
}

void MainWindow::refreshTable()
{
    auto records = database->getAllRecords();
    showRecordInTable(records);
}

void MainWindow::showRecordInTable(const std::vector<PlateRecord>& records)
{
    tableWidget->setRowCount(0);
    tableWidget->setRowCount(static_cast<int>(records.size()));
    
    for (size_t i = 0; i < records.size(); ++i) {
        // 序号列（从1开始）
        tableWidget->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::number(i + 1)));
        // 数据列
        tableWidget->setItem(static_cast<int>(i), 1, new QTableWidgetItem(QString::fromStdString(records[i].plate)));
        tableWidget->setItem(static_cast<int>(i), 2, new QTableWidgetItem(QString::fromStdString(records[i].city)));
        tableWidget->setItem(static_cast<int>(i), 3, new QTableWidgetItem(QString::fromStdString(records[i].owner)));
        tableWidget->setItem(static_cast<int>(i), 4, new QTableWidgetItem(QString::fromStdString(records[i].category)));
    }
    
    bool hasData = !records.empty();
    emptyStateLabel->setVisible(!hasData);
    if (stackedLayout) {
        stackedLayout->setCurrentIndex(hasData ? 0 : 1);
    }
    updateActionStates();
    updateStatusBar(QString("显示 %1 条记录").arg(records.size()));
}

void MainWindow::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "错误", message);
    } else {
        QMessageBox::information(this, "信息", message);
    }
}

void MainWindow::updateStatusBar(const QString& message)
{
    statusLabel->setText(message);
    infoText->append(QString("[%1] %2").arg(QDateTime::currentDateTime().toString("hh:mm:ss")).arg(message));
}

void MainWindow::updateActionStates()
{
    bool hasSelection = tableWidget->selectionModel() && tableWidget->selectionModel()->hasSelection();
    bool hasData = database->getRecordCount() > 0;
    
    modifyBtn->setEnabled(hasSelection);
    deleteBtn->setEnabled(hasSelection);
    statsBtn->setEnabled(hasData);        // 只需要有数据，不需要选中
    cityStatsBtn->setEnabled(hasData);    // 只需要有数据，不需要选中
}

void MainWindow::applyFontScale(double scale)
{
    // 限制缩放范围
    if (scale < 0.8) scale = 0.8;
    if (scale > 1.4) scale = 1.4;
    fontScale = scale;
    
    QFont f = baseFont;
    f.setPointSizeF(baseFont.pointSizeF() * fontScale);
    QApplication::setFont(f);
    
    // 调整表格行高
    tableWidget->verticalHeader()->setDefaultSectionSize(static_cast<int>(34 * fontScale));
    tableWidget->horizontalHeader()->setDefaultSectionSize(static_cast<int>(130 * fontScale));
}

// 槽函数实现
void MainWindow::onAddRecord()
{
    QString plate = plateEdit->text().trimmed().toUpper();
    QString city = cityEdit->text().trimmed();
    QString owner = ownerEdit->text().trimmed();
    
    if (plate.isEmpty() || city.isEmpty() || owner.isEmpty()) {
        showMessage("请填写完整信息！", true);
        return;
    }
    
    // 验证车牌格式（支持燃油车与新能源车）
    if (!Utils::isValidPlate(plate.toStdString())) {
        showMessage("车牌号格式错误！\n"
                    "燃油车示例：辽A12345（辽+地市字母+5位字母/数字）\n"
                    "新能源示例：辽BDF12345（辽+地市字母+D/F+5位字母/数字）", true);
        plateEdit->setFocus();
        return;
    }
    
    // 验证车牌字母和城市是否匹配
    if (!Utils::validatePlateCityMatch(plate.toStdString(), city.toStdString())) {
        // 从车牌中提取字母（跳过"辽"字）
        char plateLetter = Utils::extractPlateLetter(plate.toStdString());
        
        QString expectedCity = QString::fromStdString(Utils::getCityByPlateLetter(plateLetter));
        QString message;
        if (expectedCity.isEmpty()) {
            message = QString("车牌字母 '%1' 无效！有效的车牌字母为: A,B,C,D,E,F,G,H,J,K,L,M,N,P").arg(plateLetter);
            showMessage(message, true);
            return;
        } else {
            message = QString("车牌字母 '%1' 对应城市应为 '%2'，但您输入的是 '%3'。\n是否仍要添加？").arg(plateLetter).arg(expectedCity).arg(city);
            int ret = QMessageBox::question(this, "城市不匹配", message, 
                                           QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::No) {
                return;
            }
        }
    }
    
    if (database->addRecord(plate.toStdString(), city.toStdString(), owner.toStdString())) {
        showMessage("添加成功！");
        plateEdit->clear();
        cityEdit->clear();
        ownerEdit->clear();
        refreshTable();
        updateStatusBar(QString("当前记录数: %1").arg(database->getRecordCount()));
    } else {
        showMessage("添加失败！车牌号已存在。", true);
    }
}

void MainWindow::onLoadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "选择文件",
        ".",
        "数据文件 (*.txt *.csv);;文本文件 (*.txt);;CSV 文件 (*.csv);;所有文件 (*)");
    if (!filename.isEmpty()) {
        if (database->loadFromFile(filename.toStdString())) {
            showMessage("导入成功！");
            refreshTable();
        } else {
            showMessage("导入失败！", true);
        }
    }
}

void MainWindow::onGenerateRandom()
{
    bool ok;
    int count = QInputDialog::getInt(this, "随机生成", "请输入要生成的记录数:", 100, 1, 10000, 1, &ok);
    if (ok) {
        database->generateRandomData(count);
        showMessage(QString("成功生成 %1 条记录！").arg(count));
        refreshTable();
    }
}

void MainWindow::onShowAllRecords()
{
    auto records = database->getAllRecords();
    showRecordInTable(records);
}

void MainWindow::onModifyRecord()
{
    // 检查是否有选中的行
    QList<QTableWidgetItem*> items = tableWidget->selectedItems();
    if (items.isEmpty()) {
        showMessage("请先在表格中选中要修改的记录！", true);
        return;
    }
    
    int row = items.first()->row();
    QString originalPlate = tableWidget->item(row, 1)->text();  // 原始车牌号
    
    QString plate = plateEdit->text().trimmed();
    if (plate.isEmpty()) {
        showMessage("请输入新的车牌号！", true);
        return;
    }
    
    // 验证车牌格式（支持燃油车与新能源车）
    if (!Utils::isValidPlate(plate.toStdString())) {
        showMessage("车牌号格式错误！\n"
                    "燃油车示例：辽A12345（辽+地市字母+5位字母/数字）\n"
                    "新能源示例：辽BDF12345（辽+地市字母+D/F+5位字母/数字）", true);
        return;
    }
    
    QString city = cityEdit->text().trimmed();
    QString owner = ownerEdit->text().trimmed();
    
    if (city.isEmpty() || owner.isEmpty()) {
        showMessage("请输入新的城市和车主信息！", true);
        return;
    }
    
    // 验证车牌字母和城市是否匹配
    if (!Utils::validatePlateCityMatch(plate.toStdString(), city.toStdString())) {
        char plateLetter = Utils::extractPlateLetter(plate.toStdString());
        
        QString expectedCity = QString::fromStdString(Utils::getCityByPlateLetter(plateLetter));
        if (!expectedCity.isEmpty()) {
            QString message = QString("车牌字母 '%1' 对应城市应为 '%2'，但您输入的是 '%3'。\n是否仍要修改？").arg(plateLetter).arg(expectedCity).arg(city);
            int ret = QMessageBox::question(this, "城市不匹配", message, 
                                           QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::No) {
                return;
            }
        } else {
            showMessage(QString("车牌字母 '%1' 无效！有效的车牌字母为: A,B,C,D,E,F,G,H,J,K,L,M,N,P").arg(plateLetter), true);
            return;
        }
    }
    
    // 先删除原记录，再添加新记录
    if (database->deleteRecord(originalPlate.toStdString()) && 
        database->addRecord(plate.toStdString(), city.toStdString(), owner.toStdString())) {
        showMessage("修改成功！");
        plateEdit->clear();
        cityEdit->clear();
        ownerEdit->clear();
        refreshTable();
    } else {
        showMessage("修改失败！", true);
    }
}

void MainWindow::onDeleteRecord()
{
    QString plate = plateEdit->text().trimmed().toUpper();
    if (plate.isEmpty()) {
        showMessage("请输入要删除的车牌号！", true);
        return;
    }
    
    int ret = QMessageBox::warning(this, "确认删除", "确定要删除选中的记录吗？", 
                                     QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (database->deleteRecord(plate.toStdString())) {
            showMessage("删除成功！");
            plateEdit->clear();
            cityEdit->clear();
            ownerEdit->clear();
            refreshTable();
        } else {
            showMessage("删除失败！未找到该车牌号。", true);
        }
    }
}

void MainWindow::onRadixSort()
{
    if (database->getRecordCount() == 0) {
        showMessage("当前无记录，无法排序！", true);
        return;
    }
    
    progressBar->setVisible(true);
    progressBar->setRange(0, 0); // 不确定进度
    
    database->radixSortByPlate();
    
    progressBar->setVisible(false);
    showMessage("排序完成！");
    refreshTable();
}

void MainWindow::onBinarySearch()
{
    QString plate = searchEdit->text().trimmed().toUpper();
    if (plate.isEmpty()) {
        showMessage("请输入要查找的车牌号！", true);
        return;
    }
    
    int idx = database->binarySearchPlate(plate.toStdString());
    if (idx != -1) {
        // 高亮显示找到的记录
        refreshTable();
        tableWidget->selectRow(idx);
        tableWidget->scrollToItem(tableWidget->item(idx, 0));
        showMessage(QString("找到记录，位于第 %1 行").arg(idx + 1));
    } else {
        showMessage("未找到该车牌！", true);
    }
}

void MainWindow::onCitySearch()
{
    QString city = cityCombo->currentText().trimmed();
    if (city.isEmpty()) {
        showMessage("请选择或输入城市名！", true);
        return;
    }
    
    auto results = database->searchByCity(city.toStdString());
    if (results.empty()) {
        showMessage("未找到该城市的记录！", true);
    } else {
        showRecordInTable(results);
        showMessage(QString("找到 %1 条记录").arg(results.size()));
    }
}

void MainWindow::onPrefixSearch()
{
    QString prefix = prefixEdit->text().trimmed();
    
    // 如果前缀输入框为空，尝试从查找输入框获取
    if (prefix.isEmpty()) {
        prefix = searchEdit->text().trimmed();
    }
    
    // 如果还是为空，提示用户
    if (prefix.isEmpty()) {
        showMessage("请输入车牌前缀！\n\n前缀查找说明：\n"
                   "前缀查找可以根据车牌的前几个字符进行模糊匹配。\n\n"
                   "输入格式：必须以\"辽\"开头\n"
                   "• 输入\"辽B\"：查找所有以\"辽B\"开头的车牌\n"
                   "• 输入\"辽B72\"：查找所有以\"辽B72\"开头的车牌\n"
                   "• 输入\"辽B7238\"：查找所有以\"辽B7238\"开头的车牌\n\n"
                   "用途：当您只记得车牌的前几位时，可以使用前缀查找快速找到相关记录。", true);
        return;
    }
    
    prefix = prefix.toUpper();
    
    // 必须以“辽”开头
    if (!prefix.startsWith(QStringLiteral("辽"))) {
        showMessage("前缀格式错误！\n\n"
                    "前缀必须以\"辽\"开头，后面可以跟字母(A-Z,排除I和O)和数字。\n\n"
                    "正确示例：\n"
                    "• 辽B\n"
                    "• 辽B72\n"
                    "• 辽B7238\n\n"
                    "错误示例：\n"
                    "• B72（缺少\"辽\"）\n"
                    "• 01B（格式不正确）", true);
        return;
    }
    
    QString suffix = prefix.mid(1); // 取除“辽”外的部分
    suffix = suffix.toUpper();
    
    // 最多允许 7 个字符（支持燃油“字母+5位”与新能源“字母+D/F+5位”）
    if (suffix.length() > 7) {
        suffix = suffix.left(7);
    }
    
    // 验证后续字符
    for (int i = 0; i < suffix.length(); ++i) {
        QChar c = suffix[i];
        if (!c.isDigit() && !(c >= 'A' && c <= 'Z')) {
            showMessage(QString("前缀包含非法字符：%1\n\n"
                                "允许的字符：数字0-9，字母A-Z（排除I和O）").arg(c), true);
            return;
        }
        if (c == 'I' || c == 'O') {
            showMessage(QString("前缀不能包含字母 %1（避免与数字1和0混淆）").arg(c), true);
            return;
        }
    }
    
    // 重新组合前缀（确保格式为“辽+后缀”）
    prefix = QStringLiteral("辽") + suffix;
    
    auto results = database->prefixSearch(prefix.toStdString());
    if (results.empty()) {
        showMessage(QString("未找到以 %1 为前缀的车牌\n\n提示：\n"
                          "请检查输入的前缀是否正确。\n"
                          "前缀必须以\"辽\"开头，后面可以跟字母(A-Z,排除I和O)和数字。\n\n"
                          "示例：\n"
                          "• 辽B\n"
                          "• 辽B72\n"
                          "• 辽B7238").arg(prefix), true);
    } else {
        showRecordInTable(results);
        QString message = QString("前缀查找成功！\n\n"
                                "查找前缀：%1\n"
                                "找到 %2 条记录\n\n"
                                "说明：前缀查找会匹配所有以输入前缀开头的车牌号。").arg(prefix).arg(results.size());
        showMessage(message);
        updateStatusBar(QString("前缀查找: %1，找到 %2 条记录").arg(prefix).arg(results.size()));
    }
}

void MainWindow::onStatistics()
{
    if (database->getRecordCount() == 0) {
        showMessage("当前无记录，无法统计！", true);
        return;
    }
    
    std::ostringstream oss;
    oss << "========== 统计信息 ==========\n";
    oss << "当前共有记录条数：" << database->getRecordCount() << "\n";
    oss << "城市数量：" << database->getCityCount() << "\n";
    oss << "=============================\n";
    
    QString statsText = QString::fromStdString(oss.str());
    infoText->append(statsText);
    database->statistics();
    
    // 显示弹窗
    QMessageBox::information(this, "统计信息", statsText);
    updateStatusBar("统计信息已显示");
}

void MainWindow::onCityStatistics()
{
    if (database->getRecordCount() == 0) {
        showMessage("当前无记录，无法统计！", true);
        return;
    }
    
    auto stats = database->getCityStatistics();
    std::ostringstream oss;
    oss << "========== 城市统计排序 ==========\n";
    for (const auto& p : stats) {
        oss << p.first << ": " << p.second << " 条\n";
    }
    oss << "==================================\n";
    
    QString statsText = QString::fromStdString(oss.str());
    infoText->append(statsText);
    
    // 显示弹窗
    QMessageBox::information(this, "城市统计排序", statsText);
    updateStatusBar("城市统计排序已显示");
}

void MainWindow::onPerformanceStats()
{
    std::string stats = database->getPerformanceStats();
    infoText->append(QString::fromStdString(stats));
    infoText->append("\n");
    
    // 同时显示在消息框中
    QString statsQStr = QString::fromStdString(stats);
    QMessageBox::information(this, "性能统计", statsQStr);
    
    updateStatusBar("性能统计已显示在信息面板和对话框中");
}

void MainWindow::onValidateData()
{
    std::string result = database->getValidateDataResult();
    infoText->append(QString::fromStdString(result));
    infoText->append("\n");
    
    // 同时显示在消息框中
    QString resultQStr = QString::fromStdString(result);
    
    // 判断验证是否通过
    bool isValid = database->validateData();
    if (isValid) {
        QMessageBox::information(this, "数据验证", resultQStr);
        updateStatusBar("数据验证完成：✓ 数据完整性通过");
    } else {
        QMessageBox::warning(this, "数据验证", resultQStr + "\n\n警告：发现数据问题，请检查！");
        updateStatusBar("数据验证完成：✗ 发现数据问题");
    }
}

void MainWindow::onSaveToFile()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "保存数据文件",
        ".",
        "数据文件 (*.txt *.csv);;文本文件 (*.txt);;CSV 文件 (*.csv);;所有文件 (*)");
    if (!filename.isEmpty()) {
        QString lower = filename.toLower();
        bool ok = false;
        if (lower.endsWith(".csv")) {
            ok = database->exportToCSV(filename.toStdString());
        } else {
            ok = database->saveToFile(filename.toStdString());
        }
        
        if (ok) {
            showMessage("保存成功！");
        } else {
            showMessage("保存失败！", true);
        }
    }
}

void MainWindow::onClearAll()
{
    int ret = QMessageBox::warning(this, "确认清空", "确定要清空所有数据吗？此操作不可恢复！", 
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        database->clearAll();
        showMessage("已清空所有数据");
        refreshTable();
    }
}

void MainWindow::onTableSelectionChanged()
{
    QList<QTableWidgetItem*> items = tableWidget->selectedItems();
    if (!items.isEmpty()) {
        int row = items.first()->row();
        plateEdit->setText(tableWidget->item(row, 1)->text());  // 车牌号在第1列
        cityEdit->setText(tableWidget->item(row, 2)->text());   // 城市在第2列
        ownerEdit->setText(tableWidget->item(row, 3)->text());  // 车主在第3列
    }
    updateActionStates();
}

void MainWindow::onFontIncrease()
{
    applyFontScale(fontScale + 0.1);
}

void MainWindow::onFontDecrease()
{
    applyFontScale(fontScale - 0.1);
}

void MainWindow::onFontReset()
{
    applyFontScale(1.0);
}

void MainWindow::onTableDoubleClick(int row, int column)
{
    Q_UNUSED(column);
    plateEdit->setText(tableWidget->item(row, 1)->text());  // 车牌号在第1列
    cityEdit->setText(tableWidget->item(row, 2)->text());   // 城市在第2列
    ownerEdit->setText(tableWidget->item(row, 3)->text());  // 车主在第3列
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于", 
        "辽宁省汽车牌照快速查询系统 v2.0\n\n"
        "基于C++和Qt开发\n"
        "支持链式基数排序、折半查找、分块索引查找等功能\n\n"
        "开发时间: 2024");
}

void MainWindow::onHelp()
{
    QMessageBox::information(this, "使用帮助",
        "1. 数据录入：填写车牌号、城市、车主信息后点击添加\n"
        "   - 输入车牌号时，系统会自动根据第一个字母填充对应城市\n"
        "   - 系统会验证车牌字母与城市是否匹配\n"
        "2. 查找功能：\n"
        "   - 折半查找：输入完整车牌号进行精确查找\n"
        "   - 前缀查找：输入车牌前缀（如：辽B72）进行模糊查找\n"
        "   - 城市查找：选择或输入城市名查找该城市所有车牌\n"
        "3. 排序功能：点击基数排序对车牌进行排序\n"
        "4. 文件操作：可以导入/导出 TXT 与 CSV 数据文件\n"
        "5. 双击表格中的记录可以快速填充到输入框\n\n"
        "车牌格式：\n"
        "  - 燃油车：辽+地市字母(A-Z,排除I和O)+5位编号(数字和字母,排除I和O)\n"
        "  - 新能源：辽+地市字母(A-Z,排除I和O)+D/F(纯电/插混)+5位编号(数字和字母,排除I和O)\n"
        "示例：燃油车 辽B7238A，新能源 辽BDF12345\n\n"
        "车牌字母对应关系（辽宁省）：\n"
        "A-沈阳 B-大连 C-鞍山 D-抚顺 E-本溪 F-丹东\n"
        "G-锦州 H-营口 J-阜新 K-辽阳 L-盘锦\n"
        "M-铁岭 N-朝阳 P-葫芦岛\n\n"
        "注意：车牌编号中不能使用字母I和O，避免与数字1和0混淆。");
}

// 车牌输入变化时的处理：自动填充城市
void MainWindow::onPlateTextChanged()
{
    QString plate = plateEdit->text();
    
    // 验证车牌格式（支持燃油车与新能源车）
    std::string plateStr = plate.toStdString();
    if (!plateStr.empty() && !Utils::isValidPlate(plateStr)) {
        // 如果输入不符合规范，显示错误提示
        if (plateStr.length() >= 9) {
            // 输入已满，显示错误
            plateEdit->setStyleSheet("background-color: #f8d7da;"); // 红色背景
            updateStatusBar("车牌格式错误！请参考：燃油车“辽A12345”，新能源“辽BDF12345”");
            return;
        }
    } else {
        plateEdit->setStyleSheet(""); // 恢复正常颜色
    }
    
    // 转换为大写（但保留"辽"字）
    QString upperPlate = plate;
    if (plate.length() >= 4) {
        // 保留前3个字节（"辽"字），后面的转换为大写
        QString prefix = plate.left(3); // "辽"字
        QString suffix = plate.mid(3).toUpper();
        upperPlate = prefix + suffix;
        
        if (plateEdit->text() != upperPlate) {
            int cursorPos = plateEdit->cursorPosition();
            plateEdit->setText(upperPlate);
            plateEdit->setCursorPosition(cursorPos);
        }
    }
    
    // 从车牌中提取字母（跳过"辽"字）
    char plateLetter = Utils::extractPlateLetter(upperPlate.toStdString());
    
    if (plateLetter != '\0') {
        std::string city = Utils::getCityByPlateLetter(plateLetter);
        if (!city.empty()) {
            // 如果城市输入框为空，或者当前城市与车牌字母不匹配，则自动填充
            QString currentCity = cityEdit->text().trimmed();
            if (currentCity.isEmpty() || !Utils::validatePlateCityMatch(plate.toStdString(), currentCity.toStdString())) {
                // 使用blockSignals避免触发onCityTextChanged
                cityEdit->blockSignals(true);
                cityEdit->setText(QString::fromStdString(city));
                cityEdit->blockSignals(false);
                
                // 如果城市不匹配，改变背景色提示
                if (!currentCity.isEmpty() && currentCity != QString::fromStdString(city)) {
                    cityEdit->setStyleSheet("background-color: #fff3cd;"); // 浅黄色提示
                } else {
                    cityEdit->setStyleSheet(""); // 恢复正常颜色
                }
            }
        }
    } else if (plate.isEmpty()) {
        // 车牌为空时，恢复城市输入框样式
        cityEdit->setStyleSheet("");
    }
}

// 城市输入变化时的处理：验证是否匹配
void MainWindow::onCityTextChanged()
{
    QString plate = plateEdit->text().trimmed();
    QString city = cityEdit->text().trimmed();
    
    if (plate.isEmpty() || city.isEmpty()) {
        cityEdit->setStyleSheet("");
        return;
    }
    
    // 验证车牌格式（支持燃油车与新能源车）
    if (!Utils::isValidPlate(plate.toStdString())) {
        cityEdit->setStyleSheet("background-color: #fff3cd;"); // 黄色：车牌格式错误
        return;
    }
    
    // 验证车牌字母和城市是否匹配
    if (Utils::validatePlateCityMatch(plate.toStdString(), city.toStdString())) {
        cityEdit->setStyleSheet("background-color: #d4edda;"); // 浅绿色表示匹配
    } else {
        // 从车牌中提取字母
        char plateLetter = Utils::extractPlateLetter(plate.toStdString());
        
        if (plateLetter != '\0') {
            std::string expectedCity = Utils::getCityByPlateLetter(plateLetter);
            if (!expectedCity.empty()) {
                cityEdit->setStyleSheet("background-color: #f8d7da;"); // 浅红色表示不匹配
            } else {
                cityEdit->setStyleSheet("");
            }
        } else {
            cityEdit->setStyleSheet("");
        }
    }
}

