#ifndef FASTCGI_H
#define FASTCGI_H

#include <fcgiapp.h>

// reads a stream
int fastcgi_read_stream(FCGX_Stream *stream, char **string);

#endif
