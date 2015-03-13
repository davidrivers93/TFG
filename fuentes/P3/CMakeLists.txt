SET (CIMG_PROGRAMS box_average directional_gradient)
#SET (OPENCV_PROGRAMS )
SET (MIXED_PROGRAMS focus_meter)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

add_library(linfilter linfilters.cpp)

foreach(program ${CIMG_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
		target_link_libraries(${program} linfilter)
endforeach(program)

foreach(program ${OPENCV_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})
		target_link_libraries(${program} linfilter)        
endforeach(program)

foreach(program ${MIXED_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})
        target_link_libraries(${program} linfilter)
endforeach(program)

install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS}
DESTINATION ${CMAKE_INSTALL_BINDIR})




