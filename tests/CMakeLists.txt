find_package(Boost COMPONENTS unit_test_framework)

add_executable(test_assign test_assign.cpp)
target_link_libraries(test_assign ${Boost_LIBRARIES})
add_test(NAME test_assign COMMAND test_assign --report_level=detailed)

add_executable(test_compare test_compare.cpp)
target_link_libraries(test_compare ${Boost_LIBRARIES})
add_test(NAME test_compare COMMAND test_compare --report_level=detailed)
