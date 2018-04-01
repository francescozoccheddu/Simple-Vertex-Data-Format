#include "C4D2GL.h"
#include <stdio.h>


#define _C4D2GL_HEADER "C4D2GL v1.0"

#define _C4D2GL_EOS -7

typedef int (*_c4d2gl_stream_pop_func)(void * stream);

int _c4d2gl_file_pop (FILE * file);

int _c4d2gl_charp_pop (char ** charp);

int _c4d2gl_head(_c4d2gl_stream_pop_func stream_pop_func, void * stream, )

int _c4d2gl_file_pop (FILE * file)
{
	int ch = getc (file);
	if (ch == EOF)
	{
		if (ferror (file))
		{
			return C4D2GL_IO_ERROR;
		}
		else
		{
			return _C4D2GL_EOS;
		}
	}
	else
	{
		return ch;
	}
}

int _c4d2gl_charp_pop (char ** charp)
{
	char ch = *charp[0];
	if (ch == '\0')
	{
		return _C4D2GL_EOS;
	}
	else
	{
		(*charp)++;
		return ch;
	}
}
