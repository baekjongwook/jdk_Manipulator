cmake_minimum_required(VERSION 3.0.2)
project(jdk)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  serial
)

catkin_package(
   INCLUDE_DIRS inc
   LIBRARIES ${PROJECT_NAME}
   CATKIN_DEPENDS roscpp std_msgs
)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/node.cpp
  src/stm_read.cpp
  src/stm_write.cpp
  src/dynamixel_read.cpp
  src/dynamixel_write.cpp
  src/test.cpp
  src/manipulator.cpp
  src/minibot.cpp)

target_link_libraries(jdk ${catkin_LIBRARIES})

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

install(
    DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
