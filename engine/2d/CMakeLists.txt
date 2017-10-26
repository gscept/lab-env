#--------------------------------------------------------------------------
# 2d
#--------------------------------------------------------------------------

SET(files_2d
	2dapp.h
	2dapp.cc)
SOURCE_GROUP("2D App" FILES ${files_2d})


ADD_LIBRARY(2dapp STATIC ${files_2d})
ADD_DEPENDENCIES(2dapp render core)
TARGET_LINK_LIBRARIES(2dapp PUBLIC core engine exts glew glfw ${OPENGL_LIBS})