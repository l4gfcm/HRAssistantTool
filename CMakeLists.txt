cmake_minimum_required(VERSION 3.5)

project(HRAssistantTool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED Sql)


add_executable(HRAssistantTool
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    newworker.cpp
    newworker.h
    newworker.ui
    editrecord.cpp
    editrecord.h
    editrecord.ui
    filter.cpp
    filter.h
    dbhandler.cpp
    dbhandler.h
    managevacancies.cpp
    managevacancies.h
    managevacancies.ui
    restartworkflow.cpp
    restartworkflow.h
    restartworkflow.ui
    login.cpp
    login.h
    login.ui
  )


target_link_libraries(HRAssistantTool PRIVATE Qt5::Widgets  Qt5::Sql)
