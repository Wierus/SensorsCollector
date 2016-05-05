#include "ApplicationLog.h"

ApplicationLog::ApplicationLog() {
}

bool ApplicationLog::initialize(QString logFilesDirectoryPath) {
    writeToStdout(QString("Path to the directory for the log files: \"%1\".").arg(logFilesDirectoryPath));
    if (!QDir(logFilesDirectoryPath).exists()) {
        writeToStdout(QString("This directory does not exist."));
        return false;
    }
    this -> logFilesDirectoryPath = logFilesDirectoryPath;
    this -> logFile = new QFile();
    if (!this -> createLogFile()) {
        return false;
    }
    this -> logMutex = new QMutex();
    return true;
}

bool ApplicationLog::createLogFile() {
    this -> currentDate = QDate::currentDate();
    QString logFilePath = this -> getLogFilePath(this -> currentDate);
    writeToStdout(QString("Path to the log file: \"%1\".").arg(logFilePath));
    this -> logFile -> setFileName(logFilePath);
    if (!this -> logFile -> open(QFile::ReadWrite | QFile::Append)) {
        writeToStdout(QString("An error occurred while opening the log file: (%1) %2.").arg(this -> logFile -> error()).arg(this -> logFile -> errorString()));
        return false;
    }
    return true;
}

QString ApplicationLog::getLogFilePath(QDate date) {
    return QString("%1/%2%3%4").arg(this -> logFilesDirectoryPath, LOG_FILE_NAME_PREFIX, date.toString(LOG_FILE_NAME_DATETIME_PATTERN), LOG_FILE_NAME_POSTFIX);
}

void ApplicationLog::writeToStdout(QString message) {
    QString datetimeString = QDateTime::currentDateTime().toString(LOG_MESSAGE_DATETIME_PATTERN);
    QString formattedMessage = QString("%1 %2").arg(datetimeString, message);
    QTextStream stdoutStream(stdout);
    stdoutStream << formattedMessage << endl;
    stdoutStream.flush();
}

void ApplicationLog::write(QString message) {
    QString datetimeString = QDateTime::currentDateTime().toString(LOG_MESSAGE_DATETIME_PATTERN);
    QString formattedMessage = QString("%1 %2").arg(datetimeString, message);
    this -> logMutex -> lock();
    // если новая дата
    if (this -> currentDate != QDate::currentDate()) {
        writeToStdout(QString("The current log file will be closed and a new file opened for the new date."));
        this -> logFile -> close();
        if (!this -> createLogFile()) {
            return;
        }
    }
    QTextStream stdoutStream(stdout);
    QTextStream logStream(logFile);
    stdoutStream << formattedMessage << endl;
    logStream << formattedMessage << endl;
    stdoutStream.flush();
    logStream.flush();
    logMutex -> unlock();
}
