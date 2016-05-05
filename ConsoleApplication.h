#ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <QCoreApplication>

#include "ApplicationLog.h"
#include "Consts.h"
#include "SensorsCollector.h"
#include "Settings.h"

class ConsoleApplication : public QCoreApplication {

Q_OBJECT

private:

    /**
     * @brief Лог программы, сохраняемый в файлы по дням.
     */
    static ApplicationLog* log;

    /**
     * @brief Настройки программы, загружаемые из XML-файла.
     */
    static Settings* settings;

    /**
     * @brief Обработчик датчиков.
     */
    static SensorsCollector* sensorsCollector;

public:

    ConsoleApplication(int argc, char* argv[]);

    bool initialize(QString logFilesDirectoryPath, QString settingsFilePath);

private slots:

    void on_quit();

};

#endif // CONSOLEAPPLICATION_H
