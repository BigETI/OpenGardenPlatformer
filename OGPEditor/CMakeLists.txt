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

target_compile_features (OGPEditor
	PRIVATE cxx_std_23
)

target_include_directories (OGPEditor
	PRIVATE "include"
)

target_link_libraries (OGPEditor
	PRIVATE OGPStatic
)
