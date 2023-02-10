TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        action.cpp \
        action_queue.cpp \
        board.cpp \
        bot.cpp \
        itemmap.cpp \
        lidar.cpp \
        main.cpp \
        sensor.cpp \
        sonar.cpp

HEADERS += \
    action.h \
    action_queue.h \
    board.h \
    bot.h \
    core_bot_param.h \
    initializer.h \
    itemmap.h \
    lidar.h \
    sensor.h \
    sonar.h
