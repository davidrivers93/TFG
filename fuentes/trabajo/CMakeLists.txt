
SET (MIXED_PROGRAMS dorsales make_trainer Qr_proc)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

add_library(functions functions.cpp)

SET(UTILITIES_SOURCES utilities)
foreach(src ${UTILITIES_SOURCES} )
  add_library (utilities ${src}.cpp ${src}.h)
endforeach(src)

foreach(program ${MIXED_PROGRAMS})
    add_executable(${program} ${program}.cpp)
	target_link_libraries(${program} ${EXTERNAL_LIBS})
	target_link_libraries(${program} utilities)
	target_link_libraries(${program} functions)
    target_link_libraries(${program} ${OpenCV_LIBS})
   	target_link_libraries(${program} ${TESSERACT_LIBRARIES})
endforeach(program)


install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS} 
DESTINATION ${CMAKE_INSTALL_BINDIR})




