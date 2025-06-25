#################################
#                               #
#  Open Garden Platformer Game  #
#                               #
#################################

add_executable (OGPGame
	"src/Game.cpp"
	"src/main.cpp"
	"src/SceneManagement/GameSceneLoader.cpp"
	"src/SceneManagement/IntroductionSceneLoader.cpp"
	"src/SceneManagement/MainMenuSceneLoader.cpp"
	"src/Scripting/DroppedGardenCollectionFileLoaderScript.cpp"
	"src/Scripting/IntroductionScript.cpp"
	"src/Scripting/PlayGardenCollection.cpp"
)

set_property (TARGET OGPGame
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_compile_features (OGPGame
	PRIVATE cxx_std_23
)

target_include_directories (OGPGame
	PUBLIC "include"
)

target_link_libraries (OGPGame
	PRIVATE OGPStatic
)

file (COPY "${CMAKE_SOURCE_DIR}/OGPGame/res/Resources" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
