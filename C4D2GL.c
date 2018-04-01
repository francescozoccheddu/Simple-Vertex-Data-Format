#include "C4D2GL.h"
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

#define _C4D2GL_HEAD_DECL "C4D2GL v1.0"
#define _C4D2GL_HEAD_DECL_LEN (sizeof(_C4D2GL_HEAD_DECL) / sizeof(char) - 1)

#define _C4D2GL_EOS -7
#define _C4D2GL_PARSE_SUCCESS 0
#define _C4D2GL_PARSE_FAILURE 1
#define _C4D2GL_MAX_INT USHRT_MAX

typedef int (*_c4d2gl_pop_func)(void * stream);

int _c4d2gl_pop_file (FILE * file);

int _c4d2gl_pop_charp (char ** charp);

int _c4d2gl_parse_head_decl (_c4d2gl_pop_func pop_stream_func, void * stream, char separator);

int _c4d2gl_parse_unsigned_integer (_c4d2gl_pop_func pop_stream_func, void * stream, char separator, int max, int * out);

int _c4d2gl_head (_c4d2gl_pop_func pop_stream_func, void * stream, int * vertices_count, int * indices_count);

int _c4d2gl_pop_file (FILE * file)
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

int _c4d2gl_pop_charp (char ** charp)
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

int _c4d2gl_parse_head_decl (_c4d2gl_pop_func pop, void * stream, char sep)
{
	int pos, ch;
	pos = 0;
	ch = pop (stream);
	while (pos < _C4D2GL_HEAD_DECL_LEN && ch == _C4D2GL_HEAD_DECL[pos])
	{
		pos++;
	}
	if (pos == _C4D2GL_HEAD_DECL_LEN && pop(stream) == sep)
	{
		return _C4D2GL_PARSE_SUCCESS;
	}
	else
	{
		return _C4D2GL_PARSE_FAILURE;
	}
}

int _c4d2gl_parse_unsigned_integer (_c4d2gl_pop_func pop, void * stream, char sep, int max, int * out)
{
	unsigned int val;
	int ch;
	val = 0;
	ch = pop (stream);
	if (isdigit (ch))
	{
		while (isdigit (ch))
		{
			unsigned int oldval = val;
			val = val * 10 + ch - '0';
			if (val < oldval || val > max)
			{
				return _C4D2GL_PARSE_FAILURE;
			}
			ch = pop (stream);
		}
		if (ch == sep)
		{
			*out = val;
			return _C4D2GL_PARSE_SUCCESS;
		}
		else
		{
			return _C4D2GL_PARSE_FAILURE;
		}
	}
	else
	{
		return _C4D2GL_PARSE_FAILURE;
	}
}
