



include_directories(${CMAKE_INSTALL_PREFIX}/include)

LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)
include_directories(./)

include_directories(${PROJECT_SOURCE_DIR}/VSidoService)
include_directories(${PROJECT_SOURCE_DIR}/VSidoCommand/cpp)



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wl,--allow-multiple-definition -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wl,--allow-multiple-definition -fprofile-arcs -ftest-coverage")
set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -g -Wl,--allow-multiple-definition -fprofile-arcs")


file(GLOB _TEST_CXX "*.cpp")


add_executable( VSidoService.ut 
	${_TEST_CXX}
)


target_link_libraries( VSidoService.ut
	VSido.Service
	VSidoCommand_s
	VSidoMotion
	pthread
	websockets
	z
#	ssl
#	crypto
)


