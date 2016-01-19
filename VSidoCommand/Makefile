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
all:
	mkdir -p ruby
	/usr/local/bin/swig -c++ -ruby VSidoConnect.i
	mv VSidoConnect_wrap.cxx ./ruby/

	mkdir -p php
	/usr/local/bin/swig -c++ -php VSidoConnect.i
	mv VSidoConnect_wrap.cpp ./php/
	mv php_VSidoConnect.h ./php/
	mv *.php ./php/

	
	mkdir -p java
	/usr/local/bin/swig -c++ -java VSidoConnect.i
	mv VSidoConnect_wrap.cxx ./java/
	mv *.java ./java/

	mkdir -p lua
	/usr/local/bin/swig -c++ -lua VSidoConnect.i
	mv VSidoConnect_wrap.cxx ./lua/

	mkdir -p python
	/usr/local/bin/swig -c++ -python VSidoConnect.i
	mv VSidoConnect_wrap.cxx ./python/
	mv VSidoConnect.py ./python/
					
	mkdir -p nodejs
	/usr/local/bin/swig -c++ -javascript -node VSidoConnect.i
	mv VSidoConnect_wrap.cxx ./nodejs/
				