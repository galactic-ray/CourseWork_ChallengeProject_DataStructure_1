#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("PlateQuerySystem");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("Liaoning University");
    
    // 设置样式
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 设置样式表（可选，美化界面）
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        app.setStyleSheet(stream.readAll());
    }
    
    MainWindow window;
    window.show();
    
    return app.exec();
}

