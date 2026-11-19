cmake_minimum_required(VERSION 4.4)
project(LR1-Explorer)

set(CMAKE_CXX_STANDARD 20)

# Dependencies
include(FetchContent)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

FetchContent_Declare(
        LR1
        GIT_REPOSITORY https://github.com/MH321Productions/LibLR1-conv.git
        GIT_TAG main
)
FetchContent_MakeAvailable(LR1)

qt_standard_project_setup()

include_directories(include)

qt_add_executable(LR1-Explorer
        src/main.cpp
        src/ui/mainwindow.cpp

        include/LR1-Explorer/model/JamFileSystemModel.hpp
        src/model/JamFileSystemModel.cpp
        src/model/JamFileSystemEntry.cpp

        include/LR1-Explorer/ui/viewer/viewer.hpp
        include/LR1-Explorer/ui/viewer/unsupportedviewer.hpp
        include/LR1-Explorer/ui/viewer/imageviewer.hpp
        src/ui/viewer/imageviewer.cpp
        include/LR1-Explorer/ui/viewer/audioviewer.hpp
        src/ui/viewer/audioviewer.cpp
)
target_link_libraries(LR1-Explorer PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia LR1)

set_target_properties(LR1-Explorer PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)

install(TARGETS LR1-Explorer LR1
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
)

qt_generate_deploy_app_script(
        TARGET LR1-Explorer
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})