cmake_minimum_required(VERSION 3.16)
project(AdvancedProgramming)

set(CMAKE_CXX_STANDARD 23)


# --- Собираем все .cpp из папки source ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/source/*.cpp
)

# --- Создаём исполняемый файл ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- Подключаем include директории ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/3rd_party
)

# --- Ищем SDL3 ---
set(SDL3_DIR C:/libs/SDL3/cmake)
find_package(SDL3 REQUIRED CONFIG)

# --- Линкуем SDL3 ---
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# --- Добавляем Optick профайлер ---
set(OPTICK_INSTALL_TARGETS OFF CACHE BOOL "Don't install Optick" FORCE)
add_subdirectory(3rd_party/optick)

target_link_libraries(${PROJECT_NAME} PRIVATE OptickCore)
target_include_directories(${PROJECT_NAME} PRIVATE 3rd_party/optick/src)