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


.PHONY: all build install package clean doc doc-service doc-client doc-connect scan-build test cccc
all:package
	
build:
	make -f Build.mk PACKROOT=/home/sysroot/usr build
install:
	make -f Build.mk PACKROOT=/home/sysroot/usr install

package:
	make -f Build.mk PACKROOT=$(shell pwd)/package/usr install
	tar czf VSidoConnServer.tar.gz -C $(shell pwd)/package ./

clean:
	make -f Build.mk clean
	rm -rf $(shell pwd)/package
	rm -f VSidoConnServer.tar.gz
	rm -rf unittest

copy:
	cp -f objects/VSidoService/VSido.srv /usr/share/nginx/html/
	
doc:pre-doc doc-service doc-client doc-connect

pre-doc:
	rm -rf ./doc

doc-service:
	mkdir -p ./doc/VSidoService
	cd VSidoService && doxygen  Doxyfile
	
doc-client:
	mkdir -p ./doc/VSidoClient/javascript
	cd VSidoClient/javascript && yuidoc --themedir ./simple
	
	mkdir -p ./doc/VSidoClient/javascript.Server.Config
	cd VSidoClient/javascript.Server.Config && yuidoc --themedir ./simple
	
	mkdir -p ./doc/VSidoClient/JSON
	cd VSidoClient/JSON && doxygen  Doxyfile
	cp -f VSidoClient/JSON/vscad.png ./doc/VSidoClient/JSON/html/vscad.png
	
	mkdir -p ./doc/VSidoClient/JSON.Server.Config
	cd VSidoClient/JSON.Server.Config && doxygen  Doxyfile
	cp -f VSidoClient/JSON.Server.Config/vscad.png ./doc/VSidoClient/JSON.Server.Config/html/vscad.png
doc-connect:
	mkdir -p ./doc/VSidoConnect/cpp
	cd VSidoConnect/cpp && doxygen  Doxyfile
	mkdir -p ./doc/VSidoConnect/ruby
	cd VSidoConnect/ruby && rdoc  VSido.rb
	cp -rf VSidoConnect/ruby/doc/* ./doc/VSidoConnect/ruby/
	
	
scan-build:
	mkdir -p scan-doc
	scan-build-3.6 -o scan-doc -stats make


package-ut:
	mkdir -p unittest/ut
	mkdir -p unittest/gcov
	make -f Build.UT.mk PACKROOT=$(shell pwd)/unittest/usr install

test: test-cov-service test-cov-conncet

test-run-service:package-ut
	-objects/test/VSidoService/VSidoService.ut --log_format=XML --log_sink=unittest/ut/service.results.xml --log_level=all --report_level=no
test-cov-service:test-run-service
	cd objects/VSidoService && gcovr --xml > ../../unittest/gcov/service.result.xml
	sed -e 's/<source>.<\/source>/<source>\/<\/source>/g' ./unittest/gcov/service.result.xml > ./unittest/gcov/service.result2.xml 2> /dev/null
	mv -f ./unittest/gcov/service.result2.xml ./unittest/gcov/service.result.xml	
	
test-run-conncet:package-ut
	-objects/test/VSidoConnect/VSidoConnect.parser.ut --log_format=XML --log_sink=unittest/ut/connect.parser.results.xml --log_level=all --report_level=no
test-cov-conncet:test-run-conncet
	cd objects/VSidoConnect && gcovr --xml > ../../unittest/gcov/connect.parser.result.xml
	sed -e 's/<source>.<\/source>/<source>\/<\/source>/g' ./unittest/gcov/connect.parser.result.xml > ./unittest/gcov/connect.parser.result2.xml 2> /dev/null
	mv -f ./unittest/gcov/connect.parser.result2.xml ./unittest/gcov/connect.parser.result.xml
	
	
CCCC_FILES := $(shell find VSidoConnect/cpp/ -type f -name *pp)
CCCC_FILES += $(shell find VSidoService/ -type f  -name *pp)

cccc:
	cccc $(CCCC_FILES)
	
	
test-motion:package-ut
	./objects/VSidoMotion/test/motiontest $(shell pwd)/VSidoMotion/data/
	