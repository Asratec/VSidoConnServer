#Copyright (c) 2015, Asratec Corp.
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#* Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#
#* Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the following disclaimer in the documentation
#  and/or other materials provided with the distribution.
#
#* Neither the name of VSidoConnServer nor the names of its
#  contributors may be used to endorse or promote products derived from
#  this software without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



include_directories(./)

file(GLOB __CXX "./VSidoConnect*.cpp")


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    aux_source_directory("mac" __PLATFORM)
    aux_source_directory("uart_termios" __UART)
	include_directories(./mac)
	include_directories(./uart_termios)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    aux_source_directory("linux" __PLATFORM)
    aux_source_directory("uart_termios" __UART)
	include_directories(./linux)
	include_directories(./uart_termios)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    aux_source_directory("win" __PLATFORM)
    set(__UART "")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


add_library( VSidoConnect SHARED
	VSidoConnect.hpp
	vsido_servo_info_table.hpp
	vsido_vid_table.hpp
	${__CXX}
	${__PLATFORM}
	${__UART}
)

add_library( VSidoConnect_s STATIC
	VSidoConnect.hpp
	vsido_servo_info_table.hpp
	vsido_vid_table.hpp
	${__CXX}
	${__PLATFORM}
	${__UART}
)


add_executable( VSidoConnect.sample 
	VSidoConnect.hpp
	sampleAll.cpp
)

target_link_libraries( VSidoConnect.sample 
	VSidoConnect_s
)


