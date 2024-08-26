##################
#                #
#  Dependencies  #
#                #
##################

include (FetchContent)

set (FETCHCONTENT_QUIET FALSE)

################
#              #
#  Magic Enum  #
#              #
################

FetchContent_Declare (magic_enum
	GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
	GIT_TAG "v0.9.3"
)

FetchContent_MakeAvailable (magic_enum)

############
#          #
#  raylib  #
#          #
############

FetchContent_Declare (raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_TAG "5.0"
)

FetchContent_MakeAvailable (raylib)
