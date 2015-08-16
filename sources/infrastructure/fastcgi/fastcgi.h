#ifndef FASTCGI_H
#define FASTCGI_H

#include <fcgiapp.h>

// reads a stream
int fastcgi_read_stream(FCGX_Stream *stream, char **string);

// writes a header
int fastcgi_write_header(FCGX_Stream *stream, char *name, char *value);

// closes the headers
int fastcgi_close_headers(FCGX_Stream *stream);

#endif
