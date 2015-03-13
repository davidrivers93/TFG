SET (CIMG_PROGRAMS cimg_binsegment otsu median_th cimg_umbral )
SET (OPENCV_PROGRAMS)
SET (MIXED_PROGRAMS)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

add_library(functions functions.cpp)

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

install(TARGETS segmentlib DESTINATION ${CMAKE_INSTALL_LIBDIR})




