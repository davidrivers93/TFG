# Install script for directory: /Users/David/Desktop/ProyectosC++/TFG/fuentes/P1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/cimg_display")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/cimg_display")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/cimg_display" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/cimg_display")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/cimg_display")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/cimg_display")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/cimg_rgb2components")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/cimg_rgb2components")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/cimg_rgb2components" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/cimg_rgb2components")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/cimg_rgb2components")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/cimg_rgb2components")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/cimg_compare_images")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/cimg_compare_images")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/cimg_compare_images" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/cimg_compare_images")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/cimg_compare_images")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/cimg_compare_images")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/opencv_display")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/opencv_display")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/opencv_display" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/opencv_display")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/opencv_display")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/opencv_display")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/opencv_videofile")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/opencv_videofile")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/opencv_videofile" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/opencv_videofile")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/opencv_videofile")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/opencv_videofile")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/camara_opencv")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/camara_opencv")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/camara_opencv" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/camara_opencv")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/camara_opencv")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/camara_opencv")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/opencv_cimg_display")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/opencv_cimg_display")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/opencv_cimg_display" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/opencv_cimg_display")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/opencv_cimg_display")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/opencv_cimg_display")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/camara_cimg")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/camara_cimg")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/camara_cimg" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/camara_cimg")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/camara_cimg")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/camara_cimg")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/David/bin/cimg_videofile")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/David/bin" TYPE EXECUTABLE FILES "/Users/David/Desktop/ProyectosC++/TFG/build_mac/P1/cimg_videofile")
  if(EXISTS "$ENV{DESTDIR}/Users/David/bin/cimg_videofile" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/David/bin/cimg_videofile")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/opt/local/lib"
      "$ENV{DESTDIR}/Users/David/bin/cimg_videofile")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/Users/David/bin/cimg_videofile")
    endif()
  endif()
endif()

