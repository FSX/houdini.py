#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "houdini/buffer.h"
#include "houdini/houdini.h"


#define HTML_ESCAPE(buf, str, len) houdini_escape_html0(buf, str, len, 0)
#define HTML_ESCAPE_SECURE(buf, str, len) houdini_escape_html0(buf, str, len, 1)

#if PY_MAJOR_VERSION >= 3
#define HOUDINI_FUNCTION(name, escape_function) \
    static PyObject * name (PyObject *self, PyObject *args) {\
        PyObject *obj;\
        PyObject *pstr;\
        if (!PyArg_ParseTuple(args, "O", &obj)) {\
            return NULL;\
        }\
        Py_INCREF(obj);\
        pstr = PyObject_Str(obj);\
        if (pstr == NULL) {\
            PyErr_SetString(PyExc_TypeError, "Argument must be str type.");\
            return NULL;\
        }\
        gh_buf buf = GH_BUF_INIT;\
        char *cstr = PyUnicode_AsUTF8(pstr);\
        Py_DECREF(pstr);\
        if ( escape_function (&buf, (const uint8_t *) cstr, strlen(cstr))) {\
            Py_DECREF(obj);\
            obj = PyUnicode_FromStringAndSize(buf.ptr, buf.size);\
            gh_buf_free(&buf);\
        }\
        return obj;\
    }
#else
#define HOUDINI_FUNCTION(name, escape_function) \
    static PyObject * name (PyObject *self, PyObject *args) {\
        PyObject *obj;\
        PyObject *pstr;\
        if (!PyArg_ParseTuple(args, "O", &obj)) {\
            return NULL;\
        }\
        Py_INCREF(obj);\
        pstr = PyObject_Str(obj);\
        if (pstr == NULL) {\
            PyErr_SetString(PyExc_TypeError, "Argument must be str type.");\
            return NULL;\
        }\
        gh_buf buf = GH_BUF_INIT;\
        char *cstr = PyString_AsString(pstr);\
        Py_DECREF(pstr);\
        if ( escape_function (&buf, (const uint8_t *) cstr, strlen(cstr))) {\
            Py_DECREF(obj);\
            obj = PyString_FromStringAndSize(buf.ptr, buf.size);\
            gh_buf_free(&buf);\
        }\
        return obj;\
    }
#endif

HOUDINI_FUNCTION(py_escape_html,        HTML_ESCAPE);
HOUDINI_FUNCTION(py_escape_html_secure, HTML_ESCAPE_SECURE);
HOUDINI_FUNCTION(py_unescape_html,      houdini_unescape_html);
HOUDINI_FUNCTION(py_escape_xml,         houdini_escape_xml);
HOUDINI_FUNCTION(py_escape_uri,         houdini_escape_uri);
HOUDINI_FUNCTION(py_escape_url,         houdini_escape_url);
HOUDINI_FUNCTION(py_escape_href,        houdini_escape_href);
HOUDINI_FUNCTION(py_unescape_uri,       houdini_unescape_uri);
HOUDINI_FUNCTION(py_unescape_url,       houdini_unescape_url);
HOUDINI_FUNCTION(py_escape_js,          houdini_escape_js);
HOUDINI_FUNCTION(py_unescape_js,        houdini_unescape_js);


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
