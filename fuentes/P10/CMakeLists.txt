SET (CIMG_PROGRAMS cimg_binfeatures estimate_rotation estimate_text_rotation segment_numbers)
SET (OPENCV_PROGRAMS)
SET (MIXED_PROGRAMS)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

include_directories(../P8)
foreach(program ${CIMG_PROGRAMS})
	add_executable(${program} ${program}.cpp)
	target_link_libraries(${program} ${EXTERNAL_LIBS})
	target_link_libraries(${program} segmentlib)
endforeach(program)

foreach(program ${OPENCV_PROGRAMS})
	add_executable(${program} ${program}.cpp)
	target_link_libraries(${program} ${EXTERNAL_LIBS})
	target_link_libraries(${program} ${OpenCV_LIBS})       
	target_link_libraries(${program} segmentlib)
endforeach(program)

foreach(program ${MIXED_PROGRAMS})
	add_executable(${program} ${program}.cpp)
	target_link_libraries(${program} ${EXTERNAL_LIBS})
	target_link_libraries(${program} ${OpenCV_LIBS})
	target_link_libraries(${program} segmentlib)
endforeach(program)

install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS}
DESTINATION ${CMAKE_INSTALL_BINDIR})




