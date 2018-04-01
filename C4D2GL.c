#include "C4D2GL.h"
#include <stdio.h>

#pragma warning(disable : 4996)

#define _C4D2GL_HEAD_DECL "C4D2GL v1.0"
#define _C4D2GL_HEAD_DECL_LEN (sizeof(_C4D2GL_HEAD_DECL) / sizeof(char) - 1)
#define _C4D2GL_FIELD_SEP '\n'
#define _C4D2GL_VALUE_SEP ','

int _c4d2gl_sep (FILE * file, char sep);

int _c4d2gl_head_decl (FILE * file);

int _c4d2gl_ushort (FILE * file, unsigned short * out);

int _c4d2gl_float (FILE * file, float * out);

int _c4d2gl_sep (FILE * file, char sep)
{
	return getc (file) == sep;
}

int _c4d2gl_head_decl (FILE * file)
{
	int pos, ch;
	pos = 0;
	ch = getc (file);
	while (pos < _C4D2GL_HEAD_DECL_LEN && ch == _C4D2GL_HEAD_DECL[pos])
	{
		ch = getc (file);
		pos++;
	}
	return pos == _C4D2GL_HEAD_DECL_LEN;
}

int _c4d2gl_ushort (FILE * file, unsigned short * out)
{
	return fscanf (file, "%hu", out) == 1;
}

int _c4d2gl_float (FILE * file, float * out)
{
	return fscanf (file, "%f", out);
}

const char * c4d2gl_errstr (int error)
{
	switch (error)
	{
		case C4D2GL_SUCCESS:
			return "Success";
		case C4D2GL_IO_ERROR:
			return "File IO error";
		case C4D2GL_BAD_HEAD_DECL:
			return "Bad header declaration";
		case C4D2GL_BAD_VERT_COUNT:
			return "Bad vertices count";
		case C4D2GL_BAD_IND_COUNT:
			return "Bad indices count";
		default:
			return "Unknown";
	}
}

int c4d2gl_isformaterror (int error)
{
	return error & C4D2GL_FORMAT_ERROR_BIT;
}

int c4d2gl (const char * filename, unsigned short * vert_count, float ** verts, unsigned short * ind_count, unsigned short ** inds)
{
	FILE * file = fopen (filename, 'r');
	if (file)
	{
		if (_c4d2gl_head_decl (file) || _c4d2gl_sep(file, _C4D2GL_FIELD_SEP))
		{
			return C4D2GL_BAD_HEAD_DECL;
		}
		if (_c4d2gl_ushort (file, vert_count) || _c4d2gl_sep (file, _C4D2GL_FIELD_SEP))
		{
			return C4D2GL_BAD_VERT_COUNT;
		}
		if (_c4d2gl_ushort (file, ind_count) || _c4d2gl_sep (file, _C4D2GL_FIELD_SEP))
		{
			return C4D2GL_BAD_IND_COUNT;
		}
		return C4D2GL_SUCCESS;
	}
	else
	{
		return C4D2GL_IO_ERROR;
	}
}
