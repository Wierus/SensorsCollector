#ifndef APPLICATIONLOG_H
#define APPLICATIONLOG_H

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QTextStream>

#include "Consts.h"

class ApplicationLog : public QObject {

Q_OBJECT

private:

    /**
     * @brief Путь к директории для записи лог-файлов.
     */
    QString logFilesDirectoryPath;

    /**
     * @brief Текущая дата.
     */
    QDate currentDate;

    /**
     * @brief Лог-файл.
     */
    QFile* logFile;

    /**
     * @brief Мьютекс для лога.
     */
    QMutex* logMutex;

    bool createLogFile();

    /**
     * @brief Возвращает путь к лог-файлу для указанной даты.
     * @param Дата, для которой необходимо получить путь к лог-файлу.
     * @return Путь к лог файлу для указанной даты.
     */
    QString getLogFilePath(QDate date);

public:

    ApplicationLog();

    bool initialize(QString logFilesDirectoryPath);

    static void writeToStdout(QString message);

    void write(QString message);

};

#endif // APPLICATIONLOG_H
