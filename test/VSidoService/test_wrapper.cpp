/*
Copyright (c) 2015, Asratec Corp.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of VSidoConnServer nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <string>

bool popen_null = false;
char fgets_result[256] = {0};
char system_result[256];


extern "C" {
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	FILE *popen(const char *command, const char *type)
	{
		if(popen_null)
		{
			return (FILE *)0;
		}
		else
		{
			return (FILE *)0x1;
		}
	}
	int feof ( FILE * stream )
	{
		static int i = 0;
		if(i++%2)
		{
			return EOF +1;
		}
		return EOF;
	}
	char * fgets ( char * str, int num, FILE * stream )
	{
		int size = num > sizeof(fgets_result) ? sizeof(fgets_result):num;
		memcpy(str,fgets_result,size);
		return str;
	}
	
	int pclose(FILE *stream)
	{
		return 0;
	}
	int system(const char *command)
	{
		int num = strlen(command);
		int size = num > sizeof(system_result) ? sizeof(system_result):num;
		memcpy(system_result,command,size);
		return 0;
	}
}

