#ifndef _INCLUDE_C4D2GL_H
#define _INCLUDE_C4D2GL_H

#define C4D2GL_SUCCESS 0
#define C4D2GL_FORMAT_ERROR_BIT (1 << 4)
#define C4D2GL_RUNTIME_ERROR_BIT (1 << 5)
#define C4D2GL_IO_ERROR (1 | C4D2GL_RUNTIME_ERROR_BIT)
#define C4D2GL_MALLOC_FAIL (2 | C4D2GL_RUNTIME_ERROR_BIT)
#define C4D2GL_BAD_HEAD_DECL (1 | C4D2GL_FORMAT_ERROR_BIT)
#define C4D2GL_BAD_VERT_COUNT (2 | C4D2GL_FORMAT_ERROR_BIT)
#define C4D2GL_BAD_IND_COUNT (3 | C4D2GL_FORMAT_ERROR_BIT)
#define C4D2GL_BAD_VERT (4 | C4D2GL_FORMAT_ERROR_BIT)
#define C4D2GL_EXPECTED_VERT (5 | C4D2GL_FORMAT_ERROR_BIT)
#define C4D2GL_BAD_IND (6 | C4D2GL_FORMAT_ERROR_BIT)
#define C4D2GL_EXPECTED_IND (7 | C4D2GL_FORMAT_ERROR_BIT)

const char * c4d2gl_errstr (int error);

int c4d2gl_isformaterror (int error);

int c4d2gl (const char *filename, unsigned short * vert_count, float ** verts, unsigned short * ind_count, unsigned short ** inds);

#endif