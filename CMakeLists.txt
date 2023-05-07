cmake_minimum_required(VERSION 3.0.2)

project(gazebo_camera_viewer_plugin)

include(GNUInstallDirs)

find_package(GAZEBO REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

find_package(OpenCV REQUIRED)

include_directories(include ${GAZEBO_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(${PROJECT_NAME} SHARED src/${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} ${GAZEBO_LIBRARIES} ${OpenCV_LIBS})

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)