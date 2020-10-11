file(GLOB ctve_self_tests *.cpp)

add_custom_target(ctve_self_test)

foreach(ctve_self_test_file ${ctve_self_tests})
	get_filename_component(ctve_self_test_name ${ctve_self_test_file} NAME)
	message(STATUS ${ctve_self_test_name})
	
	add_library(ctve_self_test_${ctve_self_test_name} STATIC ${ctve_self_test_name})
	target_link_libraries(ctve_self_test_${ctve_self_test_name} ctve)
	target_compile_features(ctve_self_test_${ctve_self_test_name} PRIVATE cxx_std_17)
	
	add_dependencies(ctve_self_test ctve_self_test_${ctve_self_test_name})
endforeach()
 