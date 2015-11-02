file(GLOB __CXX "./*.cpp")
file(GLOB __HXX "./*.hpp")
file(GLOB __H "./*.h")

include_directories(./)
include_directories(../include)


add_executable (motiontest
	main.cpp
)
target_link_libraries( motiontest
	VSidoMotion
	VSidoConnect_s
	pthread
)


