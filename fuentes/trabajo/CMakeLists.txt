
SET (MIXED_PROGRAMS dorsales make_trainer video bbdd_conn)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

pkg_check_modules(PC_ZBAR QUIET zbar)

add_library(functions functions.cpp Qr_proc Qr_proc.cpp database_mng database_mng.cpp)

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
	target_link_libraries(${program} ${ZBAR_LIBRARIES})	
	target_link_libraries(${program} mysqlcppconn)
endforeach(program)


install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS} 
DESTINATION ${CMAKE_INSTALL_BINDIR})




