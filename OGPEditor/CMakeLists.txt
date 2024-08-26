###################################
#                                 #
#  Open Garden Platformer Editor  #
#                                 #
###################################

add_executable (OGPEditor
	"src/main.cpp"
)

target_compile_features (OGPEditor
	PRIVATE cxx_std_23
)

set_property (TARGET OGPEditor
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_compile_definitions (OGPEditor
	PRIVATE IS_OPEN_GARDEN_PLATFORMER_STATIC
)

target_include_directories (OGPEditor
	PUBLIC "include"
)

target_link_libraries (OGPEditor
	PRIVATE OGPStatic
)
