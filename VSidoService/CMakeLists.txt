
#file(GLOB __CXX "./*.cpp")
#file(GLOB __HXX "./*.hpp")
#file(GLOB __H "./*.h")

file(GLOB _WS_CXX "ws_*.cpp")
file(GLOB _RS_CXX "rs_*.cpp")
file(GLOB _VSIDO_CXX "cmd_*.cpp")
file(GLOB _BT_CXX "bt_*.cpp")
file(GLOB _DISPATH_CXX "dispatch*.cpp")






include_directories(${CMAKE_INSTALL_PREFIX}/include)
include_directories(${PROJECT_SOURCE_DIR}/VSidoConnect/cpp)
include_directories(${PROJECT_SOURCE_DIR}/VSidoMotion/include)

LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)
include_directories(./)




add_library (VSido.Service 
	${_UART_CXX}
	${_WS_CXX}
	${_RS_CXX}
	${_VSIDO_CXX}
	${_BT_CXX}
	${_DISPATH_CXX}
)


add_executable (VSido.srv 
	main.cpp
)
target_link_libraries( VSido.srv 
	VSido.Service
	VSidoMotion
	VSidoConnect_s
	pthread
	websockets
	z
#	ssl
#	crypto
)


install(TARGETS VSido.srv RUNTIME DESTINATION bin)
install(FILES VSido.srv.start PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION bin)
install(FILES btsetup PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION bin)
install(FILES btscan PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION bin)
install(FILES bt.bind PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ DESTINATION etc)

