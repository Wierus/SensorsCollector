#ifndef CONSTS_H
#define CONSTS_H

// Название программы
#define PROGRAM_NAME                              "SensorsCollector"

// Версия программы
#define PROGRAM_VERSION                           "1.0.0"

// Префикс имени лог-файла
#define LOG_FILE_NAME_PREFIX                      "SensorsCollector-"

// Шаблон даты в имени лог-файла
#define LOG_FILE_NAME_DATETIME_PATTERN            "yyyy.MM.dd"

// Постфикс имени лог-файла
#define LOG_FILE_NAME_POSTFIX                     ".log"

// Шаблон даты и времени в сообщении в лог-файле
#define LOG_MESSAGE_DATETIME_PATTERN              "yyyy.MM.dd hh:mm:ss.zzz"

// Кодировка сообщений
#define TEXT_CODEC_NAME                           "UTF-8"

// Драйвер базы данных
#define DATABASE_DRIVER_TYPE                      "QMYSQL"

// Имя сервера баз данных
#define DATABASE_HOST_NAME                        "localhost"

// Порт сервера баз данных
#define DATABASE_PORT                             3306

// Имя базы данных
#define DATABASE_NAME                             "Sensors"

// Имя пользователя для входа на сервер баз данных
#define DATABASE_USER_NAME                        "root"

// Пароль пользователя
#define DATABASE_PASSWORD                         "root"

// Таймаут подключения к локальному серверу, мс
#define SENSOR_CLIENT_WAIT_FOR_CONNECTED_TIMEOUT  5000

// Таймаут получения данных от локального сервера, мс
#define SENSOR_CLIENT_WAIT_FOR_READY_READ_TIMEOUT 10000

#endif // CONSTS_H
