cdef extern from *:
    ctypedef char const_char 'const char'


cdef extern from 'houdini/buffer.h':
    cdef struct buf:
        unsigned char *data
        size_t size
        size_t asize
        size_t unit

    buf* bufnew(size_t)
    void bufrelease(buf *)
    char* bufcstr(buf *)


cdef extern from 'houdini/houdini.h':
    void houdini_escape_html0(buf *ob, unsigned char *src, size_t size, int secure)
    void houdini_unescape_html(buf *ob, unsigned char *src, size_t size)
    void houdini_escape_uri(buf *ob, unsigned char *src, size_t size)
    void houdini_escape_url(buf *ob, unsigned char *src, size_t size)
    void houdini_unescape_uri(buf *ob, unsigned char *src, size_t size)
    void houdini_unescape_url(buf *ob, unsigned char *src, size_t size)
    void houdini_escape_js(buf *ob, unsigned char *src, size_t size)
    void houdini_unescape_js(buf *ob, unsigned char *src, size_t size)
