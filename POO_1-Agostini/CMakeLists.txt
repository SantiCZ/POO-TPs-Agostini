cmake_minimum_required(VERSION 3.16)
project(PlanificadorTP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    main.cpp
    Usuario.cpp
    TrabajoPractico.cpp
    AccionHistorial.cpp
    UsuarioRepository.cpp
    TPRepository.cpp
    HistorialRepository.cpp
    SessionManager.cpp
    LoginWidget.cpp
    MainWidget.cpp
    TPEditorDialog.cpp
    HistorialWidget.cpp
    TPRowWidget.cpp
)

set(HEADERS
    Usuario.h
    TrabajoPractico.h
    AccionHistorial.h
    UsuarioRepository.h
    TPRepository.h
    HistorialRepository.h
    SessionManager.h
    LoginWidget.h
    MainWidget.h
    TPEditorDialog.h
    HistorialWidget.h
    TPRowWidget.h
)

add_executable(PlanificadorTP ${SOURCES} ${HEADERS})
target_link_libraries(PlanificadorTP Qt6::Core Qt6::Widgets)
