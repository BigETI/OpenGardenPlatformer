###################################
#                                 #
#  Open Garden Platformer Editor  #
#                                 #
###################################

add_executable (OGPEditor
	"src/main.cpp"
)

set_property (TARGET OGPEditor
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

if (OGP_USE_CXX_STD_17)
	target_compile_features (OGPEditor
		PRIVATE cxx_std_17
	)
else ()
	target_compile_features (OGPEditor
		PRIVATE cxx_std_23
	)
endif()

target_include_directories (OGPEditor
	PRIVATE "include"
)

target_link_libraries (OGPEditor
	PRIVATE OGPStatic
)
