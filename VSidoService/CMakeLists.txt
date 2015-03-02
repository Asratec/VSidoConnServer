
file(GLOB __CXX "./*.cpp")
file(GLOB __HXX "./*.hpp")
file(GLOB __H "./*.h")

include_directories(${CMAKE_INSTALL_PREFIX}/include)

LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)


add_executable (VSido.srv 
	${__CXX}
	${__HXX}
	${__H}
)
target_link_libraries( VSido.srv 
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

