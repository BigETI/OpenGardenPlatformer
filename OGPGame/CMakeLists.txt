#################################
#                               #
#  Open Garden Platformer Game  #
#                               #
#################################

add_executable (OGPGame
	"src/main.cpp"
)

target_compile_features (OGPGame
	PRIVATE cxx_std_23
)

set_property (TARGET OGPGame
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_compile_definitions (OGPGame
	PRIVATE IS_OGP_STATIC
)

#target_include_directories (OGPGame
#	PUBLIC "include"
#)

target_link_libraries (OGPGame
	PRIVATE OGPStatic
)

file (COPY "${CMAKE_SOURCE_DIR}/OGPGame/res/Resources" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
