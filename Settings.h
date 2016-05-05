#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject {

Q_OBJECT

public:

    Settings();

    /**
     * @brief Загружает настройки из XML-файла
     * @param fileName Имя XML-файла с настройками
     * @return true, если настройки успешно загружены. false, иначе.
     */
    bool loadFromFile(QString fileName);
    
};

#endif // SETTINGS_H
