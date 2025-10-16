cmake_minimum_required(VERSION 3.16)

project(AdvancedProgramming C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDL3_DIR C:/libs/SDL3/cmake)
set(OPTICK_INSTALL_TARGETS OFF CACHE BOOL "Don't install Optick" FORCE)


# --- Ищем SDL3 ---
find_package(SDL3 REQUIRED CONFIG)


# --- Собираем все .cpp из папки source ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/source/*.cpp
)

# --- Создаём исполняемый файл ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Подключаем include директории ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/source
    ${PROJECT_SOURCE_DIR}/3rd_party
    ${PROJECT_SOURCE_DIR}/3rd_party/optick/src
)

# --- Линкуем библиотеки ---
target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL3::SDL3 OptickCore)


# --- Добавляем Optick профайлер ---
add_subdirectory(3rd_party/optick)
