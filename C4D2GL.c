#include "C4D2GL.h"

#define HEADER "C4D2GL v1.0"

int c4d2gl_head(const char * data, int * vertCount, int * intCount, int * length);

int c4d2gl_head(const char * data, int * vertCount, int * intCount, int * length)
{
    const int headerLen = sizeof(HEADER) / sizeof(char) - 1;
    int pos = 0;
    char curr = data[0];
    while (curr != '\0' && pos < headerLen && curr == HEADER[pos])
    {
        curr = data[pos++];
    }
    if (pos != headerLen || data[pos] != '\n')
    {
        return -1;
    }
    
}

int c4d2gl_count(const char * data, int * vertCount, int * intCount)
{

}

int c4d2gl_fill(const char * data, float * verts, unsigned short * inds);

int c4d2gl_malloc(const char * data, float ** verts, unsigned short ** inds);

int c4d2gl_file_malloc(const char * filename, float ** verts, unsigned short ** inds);