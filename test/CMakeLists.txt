include_directories(${PROJECT_SOURCE_DIR}/stlite)
set(APP_SRC test.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(stltest ${APP_SRC})