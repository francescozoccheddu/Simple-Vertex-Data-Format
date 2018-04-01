#ifndef _INCLUDE_C4D2GL_H
#define _INCLUDE_C4D2GL_H

#define C4D2GL_SUCCESS 0
#define C4D2GL_BAD_HEADER -1
#define C4D2GL_UNEXPECTED_END -2
#define C4D2GL_UNEXPECTED_TOKEN -3
#define C4D2GL_VALUE_OVERFLOW -4
#define C4D2GL_IO_ERROR -5
#define C4D2GL_MALLOC_FAIL -6

int c4d2gl_count(const char * data, int * vertices_count, int * indices_count);

int c4d2gl_fill(const char * data, float * vertices, unsigned short * indices);

int c4d2gl_malloc(const char * data, float ** vertices, unsigned short ** indices);

int c4d2gl_file_malloc(const char * filename, float ** vertices, unsigned short ** indices);

#endif