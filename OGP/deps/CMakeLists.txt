##################
#                #
#  Dependencies  #
#                #
##################

include (FetchContent)

set (FETCHCONTENT_QUIET FALSE)

###########
#         #
#  Klein  #
#         #
###########

set (KLEIN_DISABLE_TESTS ON)

FetchContent_Declare (Klein
	GIT_REPOSITORY https://github.com/BigETI/Klein.git
	GIT_TAG "dev"
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable (Klein)

################
#              #
#  Magic Enum  #
#              #
################

FetchContent_Declare (magic_enum
	GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
	GIT_TAG "v0.9.3"
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable (magic_enum)
