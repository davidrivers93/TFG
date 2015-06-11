cmake_minimum_required(VERSION 2.6)



PROJECT(TFG)

# ### Tell CMake where to install compiled programs
SET(CMAKE_INSTALL_BINDIR $ENV{HOME}/bin)
SET(CMAKE_INSTALL_LIBDIR $ENV{HOME}/lib )

# ### Search Additional Libraries

if(APPLE)
#Para que encuentre OpenCV de macports
	set(CMAKE_PREFIX_PATH  "/opt/local/lib/cmake")
endif(APPLE)

find_package(PkgConfig)
#--- OpenCV (with pkgconfig)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(OPENCV REQUIRED opencv)
# https://bugs.freedesktop.org/show_bug.cgi?id=82245
# list(APPEND LIBRARIES ${OPENCV_LIBRARIES}) 
# list(APPEND LIBRARIES ${OPENCV_LDFLAGS})
# message(STATUS LIBS ${OPENCV_VERSION})

FIND_PACKAGE(OpenCV REQUIRED)

if(NOT WIN32)
	FIND_PACKAGE(JPEG REQUIRED)
	FIND_PACKAGE(X11 REQUIRED)
	FIND_PACKAGE(TIFF REQUIRED)
	FIND_PACKAGE(Threads REQUIRED)
	
	get_filename_component(TIFF_LIB_DIRS ${TIFF_LIBRARIES} PATH)
	get_filename_component(JPEG_LIB_DIRS ${JPEG_LIBRARIES} PATH)
	link_directories(${X11_LIB_DIRS})
	link_directories(${TIFF_LIB_DIRS})
	link_directories(${JPEG_LIB_DIRS})
	include_directories(${X11_INCLUDE_DIR})	
	include_directories("/usr/local/include")

endif()



#pkg_check_modules(TESSERACT REQUIRED tesseract )
#message("Tesseract Libs:" ${TESSERACT_LIBRARIES} )
#message("Tesseract Libs:" ${TESSERACT_INCLUDE_DIRS} )

pkg_check_modules(ZBAR  zbar)
#pkg_check_modules(PC_ZBAR QUIET zbar)  

##MYSQL-CONNECTOR
include_directories("/opt/local/include/mysql55-connector-cpp/cppconn")
link_directories("/opt/local/lib/mysql55-connector-cpp")
include_directories("/opt/local/include")
include_directories("/opt/local/include/tesseract")
include_directories("/opt/local/include/leptonica/")
link_directories("/opt/local/lib")

include_directories( ${PROJECT_SOURCE_DIR} )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PC_ZBAR_INCLUDE_DIRS})



if(NOT WIN32)
	include_directories(${X11_INCLUDE_DIR})
	link_directories(${X11_LIB_DIRS})
	SET(SYSTEM_LIBS ${TIFF_LIBRARIES} ${JPEG_LIBRARIES} ${X11_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})	
else()
	SET(SYSTEM_LIBS  gdi32)	
endif()	




# ### CIMG related stuff
# Flags to enable fast image display, using the XSHM library.
SET(CIMG_XSHM_CCFLAGS  -Dcimg_use_xshm) 
# Flags to enable screen mode switching, using the XRandr library.
SET(CIMG_XRANDR_CCFLAGS  -Dcimg_use_xrandr)

# Flags to enable native support for JPEG image files, using the JPEG library.
SET(CIMG_JPEG_CCFLAGS  -Dcimg_use_jpeg) 
# Flags to enable native support for TIFF image files, using the TIFF library.
SET(CIMG_TIFF_CCFLAGS  -Dcimg_use_tiff)

if (NOT WIN32)
	SET(CIMG_CFLAGS "${CIMG_JPEG_CCFLAGS} ${CIMG_TIFF_CCFLAGS}  ")
endif()	


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CIMG_CFLAGS}")

# ## Platform specific stuff
if(APPLE)
   # ## To prevent warnings
   ADD_DEFINITIONS(-Wno-unknown-pragmas)	
  
endif(APPLE)

if(NOT APPLE)

	if(NOT WIN32)
	# Profiling  : Uncomment for profiling
	   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -p")
       SET(CIMG_CFLAGS "${CIMG_CFLAGS} ${CIMG_XSHM_CCFLAGS} ${CIMG_XRANDR_CCFLAGS}")
       SET(SYSTEM_LIBS ${SYSTEM_LIBS} Xext Xrandr)
	endif(NOT WIN32)
endif(NOT APPLE)



# ## Subdirs
SUBDIRS(trabajo)

