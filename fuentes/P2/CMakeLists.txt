SET (CIMG_PROGRAMS autorange_one)
#SET (OPENCV_PROGRAMS )
SET (MIXED_PROGRAMS video_time_average autorange_video)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

add_library(intensity autorange.cpp)

foreach(program ${CIMG_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
		target_link_libraries(${program} intensity)
endforeach(program)

foreach(program ${OPENCV_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})
		target_link_libraries(${program} intensity)        
endforeach(program)

foreach(program ${MIXED_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})
        target_link_libraries(${program} intensity)
endforeach(program)

install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS}
DESTINATION ${CMAKE_INSTALL_BINDIR})




