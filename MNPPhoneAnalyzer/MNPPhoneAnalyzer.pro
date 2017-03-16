
QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MNPPhoneAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp \
    networkhandler.cpp \
    filehandler.cpp \
    welcomewindow.cpp \
    settingwindow.cpp \
    pieview.cpp \
    graphicwindow.cpp \
    servercheckavailablewindow.cpp \
    checkinternet.cpp \
    checkuserwindow.cpp \
    marketuserwindow.cpp \
    marketworker.cpp \
    marketanalysiswindow.cpp \
    startwindow.cpp
unix:!mac:!vxworks:!integrity:LIBS+= -lm

HEADERS  += mainwindow.h \
    worker.h \
    networkhandler.h \
    filehandler.h \
    welcomewindow.h \
    settingwindow.h \
    pieview.h \
    graphicwindow.h \
    servercheckavailablewindow.h \
    checkinternet.h \
    checkuserwindow.h \
    marketuserwindow.h \
    marketworker.h \
    marketanalysiswindow.h \
    startwindow.h

FORMS    += mainwindow.ui \
    welcomewindow.ui \
    settingwindow.ui \
    graphicwindow.ui \
    servercheckavailablewindow.ui \
    checkuserwindow.ui \
    marketuserwindow.ui \
    marketanalysiswindow.ui \
    startwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    res.qrc
RC_FILE = exeIcon.rc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

