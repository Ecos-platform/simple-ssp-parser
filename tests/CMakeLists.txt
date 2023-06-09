

add_executable(test_ssp_parser test_ssp_parser.cpp)
add_test(NAME test_ssp_parser COMMAND test_ssp_parser)
target_link_libraries(test_ssp_parser ssp Catch2::Catch2)
