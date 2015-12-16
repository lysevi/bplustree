include_directories(.)

FILE(GLOB SRC "*.cpp" "*.h" "*.hpp")

if(MSVC)
        add_library(trees STATIC ${SRC})
else(MSVC)
	add_library(trees SHARED ${SRC})
	TARGET_LINK_LIBRARIES(trees ${Boost_LIBRARIES})
endif(MSVC)
