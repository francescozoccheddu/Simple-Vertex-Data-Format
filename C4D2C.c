#include "C4D2C.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64) 
#pragma warning(disable : 4996)
#endif

#define _C4D2C_HEAD_DECL "C4D2C v1.0"
#define _C4D2C_HEAD_DECL_LEN (sizeof(_C4D2C_HEAD_DECL) / sizeof(char) - 1)
#define _C4D2C_FIELD_SEP '\n'
#define _C4D2C_VALUE_SEP ','

int _c4d2c_sep (FILE * file, char sep);

int _c4d2c_head_decl (FILE * file);

int _c4d2c_ushort (FILE * file, unsigned short * out);

int _c4d2c_float (FILE * file, float * out);

int _c4d2c_sep (FILE * file, char sep)
{
	return getc (file) != sep;
}

int _c4d2c_head_decl (FILE * file)
{
	int pos;
	pos = 0;
	while (pos < _C4D2C_HEAD_DECL_LEN && getc (file) == _C4D2C_HEAD_DECL[pos])
	{
		pos++;
	}
	return pos != _C4D2C_HEAD_DECL_LEN;
}

int _c4d2c_ushort (FILE * file, unsigned short * out)
{
	return fscanf (file, "%hu", out) != 1;
}

int _c4d2c_float (FILE * file, float * out)
{
	return fscanf (file, "%f", out) != 1;
}

const char * c4d2c_errstr (int error)
{
	switch (error)
	{
		case C4D2C_SUCCESS:
			return "Success";
		case C4D2C_IO_ERROR:
			return "File IO error";
		case C4D2C_BAD_HEAD_DECL:
			return "Bad header declaration";
		case C4D2C_BAD_VERT_COUNT:
			return "Bad vertices count";
		case C4D2C_BAD_IND_COUNT:
			return "Bad indices count";
		case C4D2C_BAD_VERT_DATA:
			return "Bad vertices data";
		case C4D2C_BAD_IND_DATA:
			return "Bad indices data";
		case C4D2C_UNEXPECTED_TAIL:
			return "Unexpected data at the end of the file";
		default:
			return "Unknown";
	}
}

int c4d2c_isformaterror (int error)
{
	return error & C4D2C_FORMAT_ERROR_BIT;
}

int c4d2c (const char * filename, unsigned short * vert_count, float ** verts, unsigned short * ind_count, unsigned short ** inds)
{
	FILE * file = fopen (filename, "r");
	if (file)
	{
		unsigned short vc, ic;
		float *vs;
		unsigned short *is;

		/* Header declaration */
		if (_c4d2c_head_decl (file) || _c4d2c_sep (file, _C4D2C_FIELD_SEP))
		{
			return C4D2C_BAD_HEAD_DECL;
		}

		/* Vertices count */
		if (_c4d2c_ushort (file, &vc) || _c4d2c_sep (file, _C4D2C_FIELD_SEP))
		{
			return C4D2C_BAD_VERT_COUNT;
		}

		/* Indices count */
		if (_c4d2c_ushort (file, &ic) || _c4d2c_sep (file, _C4D2C_FIELD_SEP))
		{
			return C4D2C_BAD_IND_COUNT;
		}

		/* Vertices */
		vs = NULL;
		if (vc > 0)
		{
			vs = malloc (sizeof (float) * vc);
			if (!vs)
			{
				return C4D2C_MALLOC_FAIL;
			}
			for (int v = 0; v < vc; v++)
			{
				if (_c4d2c_float (file, &vs[v]) || _c4d2c_sep (file, _C4D2C_VALUE_SEP))
				{
					free (vs);
					return C4D2C_BAD_VERT_DATA;
				}
			}
		}
		if (_c4d2c_sep (file, _C4D2C_FIELD_SEP))
		{
			free (vs);
			return C4D2C_BAD_VERT_DATA;
		}

		/* Indices */
		is = NULL;
		if (ic > 0)
		{
			is = malloc (sizeof (float) * ic);
			if (!is)
			{
				return C4D2C_MALLOC_FAIL;
			}
			for (int i = 0; i < ic; i++)
			{
				if (_c4d2c_ushort (file, &is[i]) || _c4d2c_sep (file, _C4D2C_VALUE_SEP))
				{
					free (vs);
					free (is);
					return C4D2C_BAD_IND_DATA;
				}
			}
		}
		if (_c4d2c_sep (file, _C4D2C_FIELD_SEP))
		{
			free (vs);
			free (is);
			return C4D2C_BAD_IND_DATA;
		}

		/* End of file */
		if (getc(file) != EOF)
		{
			free (vs);
			free (is);
			return C4D2C_UNEXPECTED_TAIL;
		}

		fclose (file);

		*vert_count = vc;
		*verts = vs;
		*ind_count = ic;
		*inds = is;

		return C4D2C_SUCCESS;
	}
	else
	{
		return C4D2C_IO_ERROR;
	}
}
