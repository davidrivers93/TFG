#SET (CIMG_PROGRAMS )
SET (OPENCV_PROGRAMS sift_one surf_opencv_one)
SET (OPENCV_PROGRAMS2 surf_opencv_two)
#SET (MIXED_PROGRAMS )

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})



foreach(program ${CIMG_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
endforeach(program)

foreach(program ${OPENCV_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})       
endforeach(program)

foreach(program ${OPENCV_PROGRAMS2})
        add_executable(${program} ${program}.cpp hough.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})       
endforeach(program)

foreach(program ${MIXED_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})
endforeach(program)

install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${OPENCV_PROGRAMS2} ${MIXED_PROGRAMS} DESTINATION ${CMAKE_INSTALL_BINDIR})




