#########################
# Переменные для сборки #
#########################

# Имя службы
DAEMON_NAME="sensors-collector"
# Версия службы
DAEMON_VERSION="1.0.0"
# Имя пакета (включая версию)
PACKAGE_NAME=$DAEMON_NAME"_"$DAEMON_VERSION
# Имя проекта в Qt
QT_PROJECT_NAME="SensorsCollector"
# Конфигурация проекта в Qt (Release/Debug)
QT_PROJECT_CONFIGURATION="Release"
# Директория файлов пакета для сборки (будет пересоздана)
PACKAGE_TARGET_DIR=$QT_PROJECT_NAME-$QT_PROJECT_CONFIGURATION/$PACKAGE_NAME

###################################
# Инициализация пакета для сборки #
###################################

# Удаление предыдущих файлов
rm --force $QT_PROJECT_NAME-$QT_PROJECT_CONFIGURATION/$PACKAGE_NAME*.deb
rm --force $QT_PROJECT_NAME-$QT_PROJECT_CONFIGURATION/$PACKAGE_NAME*.tar.gz
rm --force $QT_PROJECT_NAME-$QT_PROJECT_CONFIGURATION/$PACKAGE_NAME*.tar.xz

# Удаление предыдущей директории с файлами пакета
rm --recursive --force $PACKAGE_TARGET_DIR

# Создание директории для файлов пакета
mkdir --parents --verbose $PACKAGE_TARGET_DIR

# Переход в директорию пакета
cd $PACKAGE_TARGET_DIR

# Подготовка файлов пакета
dh_make --createorig --single --packagename $PACKAGE_NAME --yes

# Возврат в исходную директорию
cd ../..

#########################################
# Копирование файлов в пакет для сборки #
#########################################

# Копирование файлов в директорию "debian"
cp package/package.control  $PACKAGE_TARGET_DIR/debian/control
cp package/package.init     $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME.init
cp package/package.postinst $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME.postinst

# Создание в пакете директорий
mkdir --parents --verbose $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME/etc
mkdir --parents --verbose $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME/usr/local/bin
mkdir --parents --verbose $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME/var/log/$DAEMON_NAME

# Копирование файлов в пакет (файлы должны существовать)
cp $QT_PROJECT_NAME-$QT_PROJECT_CONFIGURATION/$QT_PROJECT_NAME $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME/usr/local/bin/$DAEMON_NAME

# Установка флага "+x" для запускаемых файлов
chmod +x $PACKAGE_TARGET_DIR/debian/$DAEMON_NAME/usr/local/bin/$DAEMON_NAME

###########################
# Сборка пакета из файлов #
###########################

# Переход в директорию пакета
cd $PACKAGE_TARGET_DIR

# Выполнение команд сборки пакета
fakeroot dh_testroot
fakeroot dh_fixperms
fakeroot dh_makeshlibs
fakeroot dh_shlibdeps --dpkg-shlibdeps-params=--ignore-missing-info -l/mnt/rasp-pi-rootfs/lib/arm-linux-gnueabihf:/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf:/mnt/rasp-pi-rootfs/usr/local/qt5pi/lib
fakeroot dh_installinit
fakeroot dh_installdeb
fakeroot dh_gencontrol
fakeroot dh_md5sums
fakeroot dh_builddeb

# Возврат в исходную директорию
cd ../..
