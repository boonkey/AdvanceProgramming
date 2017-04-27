set(user1 dansheinin)
set(user2 shukrun)
set(user3 tomertub)
set(id1 "204093124")
set(id2 "204664205")
set(id3 "301285896")
set (SOURCE_FILES
	data_types.h
	fileParsing.cpp
	fileParsing.h
	main.h
	Main.cpp
	Player.cpp
	utilities.cpp
	utilities.h
	IBattleshipGameAlgo.h
)

### DO NOT CHANGE FROM HERE ON ###

message("user: ${user1}, id: ${id1}")
message("user: ${user2}, id: ${id2}")
message("user: ${user3}, id: ${id3}")

set(EXE_NAME "ex1.${user1}")
add_executable( ${EXE_NAME}
	${SOURCE_FILES}
)
#BUILD_BINARY_OUTPUT_DIRECTORY
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex1/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")
