#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "houdini/buffer.h"
#include "houdini/houdini.h"


#define HTML_ESCAPE(buf, str, len) houdini_escape_html0(buf, str, len, 0)
#define HTML_ESCAPE_SECURE(buf, str, len) houdini_escape_html0(buf, str, len, 1)

#define HOUDINI_FUNC(name, body) static PyObject * name (PyObject *self, PyObject *args) { body; }
#define HOUDINI_WRAPPER(escape_function) {\
    PyObject *py_str;\
    if (!PyArg_ParseTuple(args, "O", &py_str)) {\
        return NULL;\
    }\
    if (!PyUnicode_Check(py_str)) {\
        PyErr_SetString(PyExc_TypeError, "Argument must be a string.");\
        return NULL;\
    }\
    gh_buf buf = GH_BUF_INIT;\
    char *str = PyUnicode_AsUTF8(py_str);\
    if ( escape_function (&buf, (const uint8_t *) str, strlen(str)))\
    {\
        Py_DECREF(py_str);\
        py_str = Py_BuildValue("s#", buf.ptr, (int) buf.size);\
        gh_buf_free(&buf);\
    }\
    return py_str;\
}


HOUDINI_FUNC(py_escape_html, HOUDINI_WRAPPER(HTML_ESCAPE));
HOUDINI_FUNC(py_escape_html_secure, HOUDINI_WRAPPER(HTML_ESCAPE_SECURE));
HOUDINI_FUNC(py_unescape_html, HOUDINI_WRAPPER(houdini_unescape_html));
HOUDINI_FUNC(py_escape_xml, HOUDINI_WRAPPER(houdini_escape_xml));
HOUDINI_FUNC(py_escape_uri, HOUDINI_WRAPPER(houdini_escape_uri));
HOUDINI_FUNC(py_escape_url, HOUDINI_WRAPPER(houdini_escape_url));
HOUDINI_FUNC(py_escape_href, HOUDINI_WRAPPER(houdini_escape_href));
HOUDINI_FUNC(py_unescape_uri, HOUDINI_WRAPPER(houdini_unescape_uri));
HOUDINI_FUNC(py_unescape_url, HOUDINI_WRAPPER(houdini_unescape_url));
HOUDINI_FUNC(py_escape_js, HOUDINI_WRAPPER(houdini_escape_js));
HOUDINI_FUNC(py_unescape_js, HOUDINI_WRAPPER(houdini_unescape_js));


static PyMethodDef houdini_methods[] = {
    {"escape_html", py_escape_html, METH_VARARGS, NULL},
    {"escape_html_secure", py_escape_html_secure, METH_VARARGS, NULL},
    {"unescape_html", py_unescape_html, METH_VARARGS, NULL},
    {"escape_xml", py_escape_xml, METH_VARARGS, NULL},
    {"escape_uri", py_escape_uri, METH_VARARGS, NULL},
    {"escape_url", py_escape_url, METH_VARARGS, NULL},
    {"escape_href", py_escape_href, METH_VARARGS, NULL},
    {"unescape_uri", py_unescape_uri, METH_VARARGS, NULL},
    {"unescape_url", py_unescape_url, METH_VARARGS, NULL},
    {"escape_js", py_escape_js, METH_VARARGS, NULL},
    {"unescape_js", py_unescape_js, METH_VARARGS, NULL},
    {NULL, NULL, 0, NULL} /* Sentinel */
};


#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "houdini",
    NULL,
    -1,
    houdini_methods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyObject *
PyInit_houdini(void)
{
    PyObject *module = PyModule_Create(&moduledef);
    return module;
}
#else
PyMODINIT_FUNC
inithoudini(void)
{
    PyObject *module = Py_InitModule("houdini", houdini_methods);
}
#endif
