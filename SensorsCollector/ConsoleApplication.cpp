#include "ConsoleApplication.h"

ApplicationLog* ConsoleApplication::log;

Settings* ConsoleApplication::settings;

SensorsCollector* ConsoleApplication::sensorsCollector;

ConsoleApplication::ConsoleApplication(int argc, char* argv[]) : QCoreApplication(argc, argv) {
}

bool ConsoleApplication::initialize(QString logFilesDirectoryPath, QString settingsFilePath) {
    ApplicationLog::writeToStdout(QString("Initializing the log file..."));
    this -> log = new ApplicationLog();
    if (!this -> log -> initialize(logFilesDirectoryPath)) {
        ApplicationLog::writeToStdout(QString("An error occurred while initializing the log file."));
        return false;
    }
    ApplicationLog::writeToStdout(QString("The log file initialized successfully."));
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(on_quit()));
    this -> log -> write(QString("%1 v%2").arg(PROGRAM_NAME).arg(PROGRAM_VERSION));
    this -> log -> write(QString("Program started."));
    this -> log -> write(QString("Loading the settings file: \"%1\"...").arg(settingsFilePath));
    this -> settings = new Settings();
    if (!this -> settings -> loadFromFile(settingsFilePath)) {
        this -> log -> write(QString("An error occurred while loading the settings file."));
        return false;
    }
    this -> log -> write(QString("The settings file loaded successfully."));
    this -> log -> write(QString("Initializing the sensors collector..."));
    this -> sensorsCollector = new SensorsCollector(this -> log);
    if (!this -> sensorsCollector -> initialize()) {
        this -> log -> write(QString("An error occurred while initializing the sensors collector."));
        return false;
    }
    if (!this -> sensorsCollector -> startCollect()) {
        this -> log -> write(QString("An error occurred while starting collect."));
        return false;
    }
    this -> log -> write(QString("The sensors collector initialized successfully."));
    return true;
}

void ConsoleApplication::on_quit() {
    this -> log -> write(QString("Program stopped."));
}
