cmake_minimum_required(VERSION 4.4)
project(LR1-Explorer)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()


qt_add_executable(LR1-Explorer
        src/main.cpp
)
target_link_libraries(LR1-Explorer PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

set_target_properties(LR1-Explorer PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)

install(TARGETS LR1-Explorer
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
)

qt_generate_deploy_app_script(
        TARGET LR1-Explorer
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})