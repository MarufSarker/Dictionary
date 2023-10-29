cmake_minimum_required(VERSION 3.16)

project(Dictionary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Sql)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(Dictionary
    main.cpp
)

qt_add_qml_module(Dictionary
    URI Dictionary
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES wordelementslistmodel.h wordelementslistmodel.cpp
    SOURCES wordslistmodel.h wordslistmodel.cpp
    RESOURCES assets/databases/dictionary.db
    RESOURCES assets/images/search-black.svg
    RESOURCES assets/images/search-white.svg
    RESOURCES assets/images/x-black.svg
    RESOURCES assets/images/x-white.svg
)

set(QT_ANDROID_BUILD_ALL_ABIS TRUE CACHE BOOL "" FORCE)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Dictionary PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Dictionary
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(NOT(ANDROID))
    set_target_properties(Dictionary PROPERTIES
        # as <build-dir>/Dictionary is an existing directory
        OUTPUT_NAME "${PROJECT_NAME}-${CMAKE_BUILD_TYPE}"
    )
endif()

target_link_libraries(Dictionary
    PRIVATE Qt6::Quick Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS Dictionary
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
