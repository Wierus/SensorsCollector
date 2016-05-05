#include "ConsoleApplication.h"
#include "Consts.h"

#include <QTextCodec>

int main(int argc, char* argv[]) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC_NAME));
    if (argc <= 2) {
        qDebug("Not enough parameters.");
        qDebug("Usage:");
        qDebug("%s LogFilesDirectory SettingsFileName", argv[0]);
        qDebug("LogFilesDirectory - path to the directory for the log files");
        qDebug("SettingsFileName - path to the settings file");
        return -1;
    }
    ConsoleApplication application(argc, argv);
    if (!application.initialize(argv[1], argv[2])) {
        qDebug("An error occurred while starting program.");
        return -1;
    }
    return application.exec();
}
