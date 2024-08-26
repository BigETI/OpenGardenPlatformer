###################################
#                                 #
#  Open Garden Platformer Engine  #
#                                 #
###################################

add_subdirectory ("deps")

#################################
#                               #
#  Statically linkable library  #
#                               #
#################################

add_library (OGPStatic STATIC
	"src/EventSystem/ObserverID.cpp"
	"src/Game.cpp"
	"src/InputSystem/InputEvent.cpp"	
	"src/InputSystem/Raylib/RaylibGamepadInputEvents.cpp"
	"src/InputSystem/Raylib/RaylibInputHandler.cpp"
	"src/Rendering/Raylib/RaylibWindowRenderer.cpp"
	"src/Rendering/RenderingContext.cpp"
	"src/Rendering/RenderingContextElement.cpp"
	"src/ResourceManagement/FileSystem.cpp"
	"src/ResourceManagement/Raylib/RaylibTexture2DResourceManager.cpp"
	"src/SceneManagement/Node.cpp"
	"src/Scripting/Cells/CellScript.cpp"
	"src/Scripting/Cells/ClimbableCellScript.cpp"
	"src/Scripting/Cells/DeadlyCellScript.cpp"
	"src/Scripting/Cells/DiggableCellScript.cpp"
	"src/Scripting/Cells/DoorCellScript.cpp"
	"src/Scripting/Cells/MoleHillCellScript.cpp"
	"src/Scripting/Cells/SolidCellScript.cpp"
	"src/Scripting/Cells/TopDeadlyCellScript.cpp"
	"src/Scripting/Entities/EntityScript.cpp"
	"src/Scripting/Entities/LiftEntityScript.cpp"
	"src/Scripting/Entities/MarmotEntityScript.cpp"
	"src/Scripting/Entities/PickupEntityScript.cpp"
	"src/Scripting/Entities/PlayerEntityScript.cpp"
	"src/Scripting/Entities/QuestionMarkEntityScript.cpp"
	"src/Scripting/Entities/WormEntityScript.cpp"
	"src/Scripting/Environment/GardenCameraScript.cpp"
	"src/Scripting/Environment/GardenCollectionScript.cpp"
	"src/Scripting/Environment/GardenScript.cpp"
	"src/Scripting/Rendering/CameraScript.cpp"
	"src/Scripting/Rendering/SpriteRendererScript.cpp"
	"src/Scripting/Script.cpp"
	"src/Serialization/LegacyGardenCollectionDeserializer.cpp"
)

set_property (TARGET OGPStatic
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_compile_features (OGPStatic
	PRIVATE cxx_std_23
)

target_compile_definitions (OGPStatic
	PRIVATE IS_OPEN_GARDEN_PLATFORMER_STATIC
)

target_include_directories (OGPStatic
	PUBLIC "include"
)

target_link_libraries (OGPStatic
	PUBLIC magic_enum
	PUBLIC raylib
)

##################################
#                                #
#  Dynamically linkable library  #
#                                #
##################################

add_library (OGP SHARED
	"src/EventSystem/ObserverID.cpp"
	"src/Game.cpp"
	"src/InputSystem/InputEvent.cpp"	
	"src/InputSystem/Raylib/RaylibGamepadInputEvents.cpp"
	"src/InputSystem/Raylib/RaylibInputHandler.cpp"
	"src/Rendering/Raylib/RaylibWindowRenderer.cpp"
	"src/Rendering/RenderingContext.cpp"
	"src/Rendering/RenderingContextElement.cpp"
	"src/ResourceManagement/FileSystem.cpp"
	"src/ResourceManagement/Raylib/RaylibTexture2DResourceManager.cpp"
	"src/SceneManagement/Node.cpp"
	"src/Scripting/Cells/CellScript.cpp"
	"src/Scripting/Cells/ClimbableCellScript.cpp"
	"src/Scripting/Cells/DeadlyCellScript.cpp"
	"src/Scripting/Cells/DiggableCellScript.cpp"
	"src/Scripting/Cells/DoorCellScript.cpp"
	"src/Scripting/Cells/MoleHillCellScript.cpp"
	"src/Scripting/Cells/SolidCellScript.cpp"
	"src/Scripting/Cells/TopDeadlyCellScript.cpp"
	"src/Scripting/Entities/EntityScript.cpp"
	"src/Scripting/Entities/LiftEntityScript.cpp"
	"src/Scripting/Entities/MarmotEntityScript.cpp"
	"src/Scripting/Entities/PickupEntityScript.cpp"
	"src/Scripting/Entities/PlayerEntityScript.cpp"
	"src/Scripting/Entities/QuestionMarkEntityScript.cpp"
	"src/Scripting/Entities/WormEntityScript.cpp"
	"src/Scripting/Environment/GardenCameraScript.cpp"
	"src/Scripting/Environment/GardenCollectionScript.cpp"
	"src/Scripting/Environment/GardenScript.cpp"
	"src/Scripting/Rendering/CameraScript.cpp"
	"src/Scripting/Rendering/SpriteRendererScript.cpp"
	"src/Scripting/Script.cpp"
	"src/Serialization/LegacyGardenCollectionDeserializer.cpp"
)

target_compile_features (OGP
	PRIVATE cxx_std_23
)

set_property (TARGET OGP
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_link_libraries (OGP
	PUBLIC magic_enum
	PUBLIC raylib
)

target_compile_definitions (OGP
	PRIVATE IS_OPEN_GARDEN_PLATFORMER_EXPORTING
)

include_directories (OGP
	PUBLIC "include"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options (OGP
		PRIVATE -fPIC
	)
	add_compile_options (-fPIC)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
endif ()
