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
	"src/Serialization/LegacyGardenCollectionDeserializer.cpp"
)

set_property (TARGET OGPStatic
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_compile_features (OGPStatic
	PRIVATE cxx_std_23
)

target_compile_definitions (OGPStatic
	PUBLIC IS_OGP_STATIC
)

target_include_directories (OGPStatic
	PUBLIC "include"
)

target_link_libraries (OGPStatic
	PUBLIC KleinStatic
	PUBLIC magic_enum
)

##################################
#                                #
#  Dynamically linkable library  #
#                                #
##################################

add_library (OGP SHARED
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
	"src/Serialization/LegacyGardenCollectionDeserializer.cpp"
)

target_compile_features (OGP
	PRIVATE cxx_std_23
)

set_property (TARGET OGP
	PROPERTY COMPILE_WARNING_AS_ERROR ON
)

target_link_libraries (OGP
	PUBLIC Klein
	PUBLIC magic_enum
)

target_compile_definitions (OGP
	PUBLIC IS_OGP_EXPORTING
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
