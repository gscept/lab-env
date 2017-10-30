#--------------------------------------------------------------------------
# assignment project
#--------------------------------------------------------------------------

PROJECT(assignment)
FILE(GLOB assignment_headers code/*.h)
FILE(GLOB assignment_sources code/*.cc)

SET(files_assignment ${assignment_headers} ${assignment_sources})
SOURCE_GROUP("assignment" FILES ${files_assignment})

ADD_EXECUTABLE(assignment ${files_assignment})
TARGET_LINK_LIBRARIES(assignment core render 2dapp)
ADD_DEPENDENCIES(assignment core render 2dapp)