#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QProgressBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "../include/PlateDatabase.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 数据录入
    void onAddRecord();
    void onLoadFromFile();
    void onGenerateRandom();
    
    // 数据管理
    void onShowAllRecords();
    void onModifyRecord();
    void onDeleteRecord();
    
    // 排序与查找
    void onRadixSort();
    void onBinarySearch();
    void onCitySearch();
    void onPrefixSearch();
    
    // 统计与分析
    void onStatistics();
    void onCityStatistics();
    void onPerformanceStats();
    void onValidateData();
    
    // 文件操作
    void onSaveToFile();
    void onExportToCSV();
    
    // 系统维护
    void onClearAll();
    
    // 表格相关
    void onTableSelectionChanged();
    void onTableDoubleClick(int row, int column);
    
    // 菜单
    void onAbout();
    void onHelp();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void refreshTable();
    void showRecordInTable(const std::vector<PlateRecord>& records);
    void showMessage(const QString& message, bool isError = false);
    void updateStatusBar(const QString& message);
    
    // UI组件
    QTableWidget* tableWidget;
    QLineEdit* plateEdit;
    QLineEdit* cityEdit;
    QLineEdit* ownerEdit;
    QLineEdit* searchEdit;
    QLineEdit* prefixEdit;
    QComboBox* cityCombo;
    QTextEdit* infoText;
    QLabel* statusLabel;
    QProgressBar* progressBar;
    
    // 按钮
    QPushButton* addBtn;
    QPushButton* modifyBtn;
    QPushButton* deleteBtn;
    QPushButton* searchBtn;
    QPushButton* prefixSearchBtn;
    QPushButton* citySearchBtn;
    QPushButton* sortBtn;
    QPushButton* clearBtn;
    QPushButton* loadFileBtn;
    QPushButton* saveFileBtn;
    QPushButton* exportCSVBtn;
    QPushButton* randomBtn;
    QPushButton* statsBtn;
    QPushButton* cityStatsBtn;
    QPushButton* perfBtn;
    QPushButton* validateBtn;
    
    // 数据
    PlateDatabase* database;
};

#endif // MAINWINDOW_H

