#ifndef _INCLUDE_C4D2GL_H
#define _INCLUDE_C4D2GL_H

int c4d2gl_count(const char * data, int * vertCount, int * intCount);

int c4d2gl_fill(const char * data, float * verts, unsigned short * inds);

int c4d2gl_malloc(const char * data, float ** verts, unsigned short ** inds);

int c4d2gl_file_malloc(const char * filename, float ** verts, unsigned short ** inds);

#endif