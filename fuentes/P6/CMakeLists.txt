SET (CIMG_PROGRAMS distance cimg_erosion cimg_dilatacion cimg_apertura cimg_cierre)
#SET (OPENCV_PROGRAMS )
#SET (MIXED_PROGRAMS)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})


foreach(program ${CIMG_PROGRAMS})
if( WIN32 )
   add_executable(${program} ${program}.cpp ../getopt_long.c)
else()
   add_executable(${program} ${program}.cpp )
endif()                
target_link_libraries(${program} ${EXTERNAL_LIBS})
endforeach(program)

foreach(program ${OPENCV_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})     
endforeach(program)

foreach(program ${MIXED_PROGRAMS})
        add_executable(${program} ${program}.cpp)
		target_link_libraries(${program} ${EXTERNAL_LIBS})
        target_link_libraries(${program} ${OpenCV_LIBS})

endforeach(program)

install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS}
DESTINATION ${CMAKE_INSTALL_BINDIR})




