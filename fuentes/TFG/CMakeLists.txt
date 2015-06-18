
SET (MIXED_PROGRAMS tfg_dorsal bib_generator_tfg)

SET(EXTERNAL_LIBS ${SYSTEM_LIBS})

pkg_check_modules(ZBAR QUIET zbar)

add_library(functions functions.cpp db_functions db_functions.cpp database_mng database_mng.cpp img_functions img_functions.cpp misc_functions misc_functions.cpp)

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
	include_directories(${TESSERACT_INCLUDE_DIRS})
	target_link_libraries( ${program} tesseract)
	target_link_libraries( ${program} lept)
	target_link_libraries(${program} ${ZBAR_LIBRARIES})	
	target_link_libraries(${program} cairo)	
	target_link_libraries(${program} mysqlcppconn)
endforeach(program)


install(TARGETS ${CIMG_PROGRAMS} ${OPENCV_PROGRAMS} ${MIXED_PROGRAMS} 
DESTINATION ${CMAKE_INSTALL_BINDIR})




