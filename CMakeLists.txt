cmake_minimum_required(
	VERSION
	3.4
)

project(
	dags
)

include(
	FcpptCMakeUtils
	OPTIONAL
	RESULT_VARIABLE
	HAVE_FCPPT_UTILS
)

function(
	setup_target
	TARGET
)
	# TODO
	if(
		HAVE_FCPPT_UTILS
	)
		fcppt_utils_set_target_compiler_flags(
			${TARGET}
		)
	endif()

	target_include_directories(
		${TARGET}
		PRIVATE
		${CMAKE_SOURCE_DIR}/libdag/include
	)
endfunction()

add_subdirectory(
	canon
)
