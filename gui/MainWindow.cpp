#include "MainWindow.h"
#include "../include/Utils.h"
#include <QHeaderView>
#include <QInputDialog>
#include <QDateTime>
#include <QAbstractItemView>
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
    
    // 中央部件
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // 左侧：数据录入和管理面板
    QVBoxLayout* leftLayout = new QVBoxLayout();
    
    // 数据录入组
    QGroupBox* inputGroup = new QGroupBox("数据录入", this);
    QGridLayout* inputLayout = new QGridLayout();
    
    inputLayout->addWidget(new QLabel("车牌号:"), 0, 0);
    plateEdit = new QLineEdit(this);
    plateEdit->setPlaceholderText("例如: 01B7238");
    plateEdit->setMaxLength(7);
    inputLayout->addWidget(plateEdit, 0, 1);
    
    inputLayout->addWidget(new QLabel("城市:"), 1, 0);
    cityEdit = new QLineEdit(this);
    cityEdit->setPlaceholderText("例如: 沈阳");
    inputLayout->addWidget(cityEdit, 1, 1);
    
    inputLayout->addWidget(new QLabel("车主:"), 2, 0);
    ownerEdit = new QLineEdit(this);
    ownerEdit->setPlaceholderText("例如: 张三");
    inputLayout->addWidget(ownerEdit, 2, 1);
    
    addBtn = new QPushButton("添加记录", this);
    modifyBtn = new QPushButton("修改记录", this);
    deleteBtn = new QPushButton("删除记录", this);
    
    QHBoxLayout* btnLayout1 = new QHBoxLayout();
    btnLayout1->addWidget(addBtn);
    btnLayout1->addWidget(modifyBtn);
    btnLayout1->addWidget(deleteBtn);
    inputLayout->addLayout(btnLayout1, 3, 0, 1, 2);
    
    loadFileBtn = new QPushButton("从文件导入", this);
    randomBtn = new QPushButton("随机生成", this);
    QHBoxLayout* btnLayout2 = new QHBoxLayout();
    btnLayout2->addWidget(loadFileBtn);
    btnLayout2->addWidget(randomBtn);
    inputLayout->addLayout(btnLayout2, 4, 0, 1, 2);
    
    inputGroup->setLayout(inputLayout);
    leftLayout->addWidget(inputGroup);
    
    // 查找组
    QGroupBox* searchGroup = new QGroupBox("查找功能", this);
    QVBoxLayout* searchLayout = new QVBoxLayout();
    
    searchLayout->addWidget(new QLabel("车牌号查找:"));
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("输入完整车牌号");
    searchLayout->addWidget(searchEdit);
    
    QHBoxLayout* searchBtnLayout = new QHBoxLayout();
    searchBtn = new QPushButton("折半查找", this);
    prefixSearchBtn = new QPushButton("前缀查找", this);
    searchBtnLayout->addWidget(searchBtn);
    searchBtnLayout->addWidget(prefixSearchBtn);
    searchLayout->addLayout(searchBtnLayout);
    
    searchLayout->addWidget(new QLabel("前缀查找:"));
    prefixEdit = new QLineEdit(this);
    prefixEdit->setPlaceholderText("例如: 01B");
    searchLayout->addWidget(prefixEdit);
    
    searchLayout->addWidget(new QLabel("城市查找:"));
    cityCombo = new QComboBox(this);
    cityCombo->setEditable(true);
    cityCombo->addItems({"沈阳", "大连", "鞍山", "抚顺", "本溪", "丹东", 
                         "锦州", "营口", "阜新", "辽阳", "盘锦", "铁岭", "朝阳", "葫芦岛"});
    searchLayout->addWidget(cityCombo);
    
    citySearchBtn = new QPushButton("城市查找", this);
    searchLayout->addWidget(citySearchBtn);
    
    searchGroup->setLayout(searchLayout);
    leftLayout->addWidget(searchGroup);
    
    // 操作组
    QGroupBox* operationGroup = new QGroupBox("操作功能", this);
    QVBoxLayout* operationLayout = new QVBoxLayout();
    
    sortBtn = new QPushButton("链式基数排序", this);
    statsBtn = new QPushButton("统计信息", this);
    cityStatsBtn = new QPushButton("城市统计排序", this);
    perfBtn = new QPushButton("性能统计", this);
    validateBtn = new QPushButton("数据验证", this);
    
    operationLayout->addWidget(sortBtn);
    operationLayout->addWidget(statsBtn);
    operationLayout->addWidget(cityStatsBtn);
    operationLayout->addWidget(perfBtn);
    operationLayout->addWidget(validateBtn);
    
    operationGroup->setLayout(operationLayout);
    leftLayout->addWidget(operationGroup);
    
    // 文件操作组
    QGroupBox* fileGroup = new QGroupBox("文件操作", this);
    QVBoxLayout* fileLayout = new QVBoxLayout();
    
    saveFileBtn = new QPushButton("保存到文件", this);
    exportCSVBtn = new QPushButton("导出CSV", this);
    clearBtn = new QPushButton("清空数据", this);
    
    fileLayout->addWidget(saveFileBtn);
    fileLayout->addWidget(exportCSVBtn);
    fileLayout->addWidget(clearBtn);
    
    fileGroup->setLayout(fileLayout);
    leftLayout->addWidget(fileGroup);
    
    leftLayout->addStretch();
    
    // 中间：数据表格
    QVBoxLayout* centerLayout = new QVBoxLayout();
    
    QLabel* tableLabel = new QLabel("车牌记录列表", this);
    tableLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    centerLayout->addWidget(tableLabel);
    
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "车牌号" << "城市" << "车主");
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setAlternatingRowColors(true);
    centerLayout->addWidget(tableWidget);
    
    // 右侧：信息显示
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    QLabel* infoLabel = new QLabel("系统信息", this);
    infoLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    rightLayout->addWidget(infoLabel);
    
    infoText = new QTextEdit(this);
    infoText->setReadOnly(true);
    infoText->setMaximumWidth(300);
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
    connect(exportCSVBtn, &QPushButton::clicked, this, &MainWindow::onExportToCSV);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearAll);
    connect(tableWidget, &QTableWidget::itemDoubleClicked, this, &MainWindow::onTableDoubleClick);
    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);
    
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
    QAction* exportAction = fileMenu->addAction("导出CSV(&E)");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("退出(&X)");
    
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadFromFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveToFile);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportToCSV);
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
        tableWidget->setItem(static_cast<int>(i), 0, new QTableWidgetItem(QString::fromStdString(records[i].plate)));
        tableWidget->setItem(static_cast<int>(i), 1, new QTableWidgetItem(QString::fromStdString(records[i].city)));
        tableWidget->setItem(static_cast<int>(i), 2, new QTableWidgetItem(QString::fromStdString(records[i].owner)));
    }
    
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
    
    if (database->addRecord(plate.toStdString(), city.toStdString(), owner.toStdString())) {
        showMessage("添加成功！");
        plateEdit->clear();
        cityEdit->clear();
        ownerEdit->clear();
        refreshTable();
        updateStatusBar(QString("当前记录数: %1").arg(database->getRecordCount()));
    } else {
        showMessage("添加失败！车牌号格式错误或已存在。", true);
    }
}

void MainWindow::onLoadFromFile()
{
    QString filename = QFileDialog::getOpenFileName(this, "选择文件", ".", "文本文件 (*.txt);;所有文件 (*)");
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
    QString plate = plateEdit->text().trimmed().toUpper();
    if (plate.isEmpty()) {
        showMessage("请输入要修改的车牌号！", true);
        return;
    }
    
    int idx = database->findRecord(plate.toStdString());
    if (idx == -1) {
        showMessage("未找到该车牌号！", true);
        return;
    }
    
    QString city = cityEdit->text().trimmed();
    QString owner = ownerEdit->text().trimmed();
    
    if (city.isEmpty() || owner.isEmpty()) {
        showMessage("请输入新的城市和车主信息！", true);
        return;
    }
    
    if (database->modifyRecord(plate.toStdString(), city.toStdString(), owner.toStdString())) {
        showMessage("修改成功！");
        plateEdit->clear();
        cityEdit->clear();
        ownerEdit->clear();
        refreshTable();
    }
}

void MainWindow::onDeleteRecord()
{
    QString plate = plateEdit->text().trimmed().toUpper();
    if (plate.isEmpty()) {
        showMessage("请输入要删除的车牌号！", true);
        return;
    }
    
    int ret = QMessageBox::question(this, "确认删除", "确定要删除这条记录吗？", 
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
    QString prefix = prefixEdit->text().trimmed().toUpper();
    if (prefix.isEmpty()) {
        prefix = searchEdit->text().trimmed().toUpper();
    }
    
    if (prefix.isEmpty()) {
        showMessage("请输入车牌前缀！", true);
        return;
    }
    
    auto results = database->prefixSearch(prefix.toStdString());
    if (results.empty()) {
        showMessage(QString("未找到以 %1 为前缀的车牌").arg(prefix), true);
    } else {
        showRecordInTable(results);
        showMessage(QString("找到 %1 条记录").arg(results.size()));
    }
}

void MainWindow::onStatistics()
{
    std::ostringstream oss;
    oss << "========== 统计信息 ==========\n";
    oss << "当前共有记录条数：" << database->getRecordCount() << "\n";
    oss << "城市数量：" << database->getCityCount() << "\n";
    oss << "=============================\n";
    
    infoText->append(QString::fromStdString(oss.str()));
    database->statistics();
    updateStatusBar("统计信息已显示");
}

void MainWindow::onCityStatistics()
{
    auto stats = database->getCityStatistics();
    std::ostringstream oss;
    oss << "========== 城市统计 ==========\n";
    for (const auto& p : stats) {
        oss << p.first << ": " << p.second << " 条\n";
    }
    oss << "=============================\n";
    
    infoText->append(QString::fromStdString(oss.str()));
    updateStatusBar("城市统计已显示");
}

void MainWindow::onPerformanceStats()
{
    database->showPerformanceStats();
    updateStatusBar("性能统计已显示");
}

void MainWindow::onValidateData()
{
    database->validateData();
    updateStatusBar("数据验证完成");
}

void MainWindow::onSaveToFile()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", "文本文件 (*.txt);;所有文件 (*)");
    if (!filename.isEmpty()) {
        if (database->saveToFile(filename.toStdString())) {
            showMessage("保存成功！");
        } else {
            showMessage("保存失败！", true);
        }
    }
}

void MainWindow::onExportToCSV()
{
    QString filename = QFileDialog::getSaveFileName(this, "导出CSV", ".", "CSV文件 (*.csv);;所有文件 (*)");
    if (!filename.isEmpty()) {
        if (database->exportToCSV(filename.toStdString())) {
            showMessage("导出成功！");
        } else {
            showMessage("导出失败！", true);
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
        plateEdit->setText(tableWidget->item(row, 0)->text());
        cityEdit->setText(tableWidget->item(row, 1)->text());
        ownerEdit->setText(tableWidget->item(row, 2)->text());
    }
}

void MainWindow::onTableDoubleClick(int row, int column)
{
    Q_UNUSED(column);
    plateEdit->setText(tableWidget->item(row, 0)->text());
    cityEdit->setText(tableWidget->item(row, 1)->text());
    ownerEdit->setText(tableWidget->item(row, 2)->text());
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
        "2. 查找功能：输入车牌号进行查找，或选择城市进行城市查找\n"
        "3. 排序功能：点击基数排序对车牌进行排序\n"
        "4. 文件操作：可以导入/导出数据文件\n"
        "5. 双击表格中的记录可以快速填充到输入框");
}

