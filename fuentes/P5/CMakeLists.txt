#SET (CIMG_PROGRAMS )
#SET (OPENCV_PROGRAMS )
SET (MIXED_PROGRAMS cimg_perspective_transform estimate_transform mosaic homography)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

add_library(geometry tgeometry.cpp)

foreach(program ${CIMG_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
		target_link_libraries(${program} geometry)
endforeach(program)

foreach(program ${OPENCV_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})
		target_link_libraries(${program} geometry)        
endforeach(program)

foreach(program ${MIXED_PROGRAMS})
        add_executable(${program} ${program}.cpp)
	target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} geometry)
        target_link_libraries(${program} ${OpenCV_LIBS})

endforeach(program)

install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS}
DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS geometry DESTINATION ${CMAKE_INSTALL_LIBDIR})




