include_directories(../src)

MESSAGE(STATUS "Test: " ${name})

macro(TEST_CASE name src)
MESSAGE(STATUS " +" ${name})
add_executable(${name} ${src})
TARGET_LINK_LIBRARIES(${name} ${Boost_LIBRARIES})
add_test(${name} ${name})
endmacro(TEST_CASE)

TEST_CASE(btree_test btree_test.cpp ../src/btree.h ../src/btree.hpp)
