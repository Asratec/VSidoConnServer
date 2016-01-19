



include_directories(${CMAKE_INSTALL_PREFIX}/include)

LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)
include_directories(./)

include_directories(${PROJECT_SOURCE_DIR}/VSidoCommand/cpp)



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wl,--allow-multiple-definition -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wl,--allow-multiple-definition -fprofile-arcs -ftest-coverage")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -g -fprofile-arcs -Wl,--allow-multiple-definition")


file(GLOB _PARSER_CXX "parser_*.cpp")


add_executable( VSidoConnect.parser.ut 
	${_PARSER_CXX}
)


target_link_libraries( VSidoConnect.parser.ut
	VSidoCommand_s
	pthread
	websockets
	z
)

file(GLOB _IO_CXX "io_*.cpp")


add_executable( VSidoConnect.io.ut 
	${_IO_CXX}
)


target_link_libraries( VSidoConnect.io.ut
	VSidoCommand_s
	pthread
	websockets
	z
)

