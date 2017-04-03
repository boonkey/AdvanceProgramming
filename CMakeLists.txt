### YOU SHOULD EDIT THE BELOW WITH YOUR DETAILS AND FILES ###

set(user1 dansheinin)
set(user2 shukrun)
set(user3 tomertub)
set(id1 "204093124")
set(id2 "204664205")
set(id3 "301285896")
set (SOURCE_FILES
	file1.h
	file1.cpp
	file2.h
	file2.cpp
)

### DO NOT CHANGE FROM HERE ON ###

message("user: ${user1}, id: ${id1}")
message("user: ${user2}, id: ${id2}")
message("user: ${user3}, id: ${id3}")

set(EXE_NAME "ex1.${user1}")
add_executable( ${EXE_NAME}
	SOURCE_FILES
)

set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex1/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "ex1.${user1})

install (TARGETS ${EXE_NAME} DESTINATION ${INSTALL_BINARY_DIR})
