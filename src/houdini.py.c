#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "houdini/buffer.h"
#include "houdini/houdini.h"


#define HTML_ESCAPE(buf, str, len) houdini_escape_html0(buf, str, len, 0)
#define HTML_ESCAPE_SECURE(buf, str, len) houdini_escape_html0(buf, str, len, 1)

#define FUNC_SIG(name) static PyObject * name (PyObject *self, PyObject *args)
#define FUNC_BODY(escape_function) \
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
    return py_str;


FUNC_SIG(py_escape_html)        { FUNC_BODY(HTML_ESCAPE);           }
FUNC_SIG(py_escape_html_secure) { FUNC_BODY(HTML_ESCAPE_SECURE);    }
FUNC_SIG(py_unescape_html)      { FUNC_BODY(houdini_unescape_html); }
FUNC_SIG(py_escape_xml)         { FUNC_BODY(houdini_escape_xml);    }
FUNC_SIG(py_escape_uri)         { FUNC_BODY(houdini_escape_uri);    }
FUNC_SIG(py_escape_url)         { FUNC_BODY(houdini_escape_url);    }
FUNC_SIG(py_escape_href)        { FUNC_BODY(houdini_escape_href);   }
FUNC_SIG(py_unescape_uri)       { FUNC_BODY(houdini_unescape_uri);  }
FUNC_SIG(py_unescape_url)       { FUNC_BODY(houdini_unescape_url);  }
FUNC_SIG(py_escape_js)          { FUNC_BODY(houdini_escape_js);     }
FUNC_SIG(py_unescape_js)        { FUNC_BODY(houdini_unescape_js);   }


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
