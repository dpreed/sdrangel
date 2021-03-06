project(bfm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(bfm_SOURCES
	bfmdemod.cpp
	bfmdemodgui.cpp
	bfmdemodsettings.cpp
	bfmplugin.cpp
	rdsdemod.cpp
	rdsdecoder.cpp
	rdsparser.cpp
	rdstmc.cpp
)

set(bfm_HEADERS
	bfmdemod.h
	bfmdemodgui.h
	bfmdemodsettings.h
	bfmplugin.h
	rdsdemod.h
	rdsdecoder.h
	rdsparser.h
	rdstmc.h
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_source_files_properties(rdstmc.cpp PROPERTIES COMPILE_FLAGS -fno-var-tracking-assignments)
endif()

set(bfm_FORMS
	bfmdemodgui.ui
)

include_directories(
	.
	${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/swagger/sdrangel/code/qt5/client	
)

#include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

qt5_wrap_ui(bfm_FORMS_HEADERS ${bfm_FORMS})

add_library(demodbfm SHARED
	${bfm_SOURCES}
	${bfm_HEADERS_MOC}
	${bfm_FORMS_HEADERS}
)

target_link_libraries(demodbfm
	${QT_LIBRARIES}
	sdrbase
	sdrgui
)

target_link_libraries(demodbfm Qt5::Core Qt5::Widgets)

install(TARGETS demodbfm DESTINATION lib/plugins/channelrx)
