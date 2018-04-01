#ifndef _INCLUDE_C4D2C_H
#define _INCLUDE_C4D2C_H

#define C4D2C_SUCCESS 0
#define C4D2C_FORMAT_ERROR_BIT (1 << 4)
#define C4D2C_RUNTIME_ERROR_BIT (1 << 5)
#define C4D2C_IO_ERROR (1 | C4D2C_RUNTIME_ERROR_BIT)
#define C4D2C_MALLOC_FAIL (2 | C4D2C_RUNTIME_ERROR_BIT)
#define C4D2C_BAD_HEAD_DECL (1 | C4D2C_FORMAT_ERROR_BIT)
#define C4D2C_BAD_VERT_COUNT (2 | C4D2C_FORMAT_ERROR_BIT)
#define C4D2C_BAD_IND_COUNT (3 | C4D2C_FORMAT_ERROR_BIT)
#define C4D2C_BAD_VERT_DATA (4 | C4D2C_FORMAT_ERROR_BIT)
#define C4D2C_BAD_IND_DATA (5 | C4D2C_FORMAT_ERROR_BIT)
#define C4D2C_UNEXPECTED_TAIL (6 | C4D2C_FORMAT_ERROR_BIT)

const char * c4d2c_errstr (int error);

int c4d2c_isformaterror (int error);

int c4d2c (const char *filename, unsigned short * vert_count, float ** verts, unsigned short * ind_count, unsigned short ** inds);

#endif