//gcc -Wall -fPIC -shared -I/usr/include/python3.11 -lpython3.11 cod.c -o cod.so


// Tbe result of Python
// import cod
// >>> cod.cod_omg()
// Traceback (most recent call last):
//  File "<stdin>", line 1, in <module>
//  cod.UnsupportedCodError: Unsupported Cod <module 'cod' from 'cod.so'>
// >>> cod._constructors[cod.cod_omg]
// >>> 'omg'
// >>> cod.hidden()
// hide from _constructor?
// 0
// >>> cod._constructors[cod.hidden]
// Traceback (most recent call last):
//  File "<stdin>", line 1, in <module>
//  KeyError: <built-in function hidden>

#include <Python.h>

// inspired from https://pierov.org/2020/03/01/python-custom-exceptions-c-extensions/#:~:text=PyErr_NewExceptionis%20declared%20in%20Python%2Ferrors.c%3A%20it%20does%20some%20processing,can%20be%20called%20to%20create%20a%20new%20type.
// and from _hashopenssl.c


// supprt multi-phase initialization
// we need state of module
/* Module state declaration */
static PyModuleDef _codmodule;

typedef struct {
	// constructor
    PyObject *constructs;
	// custom exception
    PyObject *unsupported_cod_error;
} _codstate;


// from _hashopenssl.c
static inline _codstate*
get_cod_state(PyObject *module)
{
    void *state = PyModule_GetState(module);
    assert(state != NULL);
    return (_codstate *)state;
}

// inspired by website
static PyObject *cod_omg(PyObject *module, PyObject *args)
{
    _codstate *state = get_cod_state(module);
	PyErr_Clear();
	// raise error
	PyErr_Format(
		state->unsupported_cod_error,
		"Unsupported Cod %R", module);	
	return NULL;
}

// inspired by website
static PyObject *hidden_func(PyObject *self, PyObject *args)
{
	
	printf("hide from _constructor?\n");
	// nothing
	return PyLong_FromLong(0);
}

// inspired by website
static PyMethodDef cod_methods[] = {
	{"cod_omg",  cod_omg, METH_VARARGS,
		"Raise an exception that will make you say OMG."},
	{"hidden",  hidden_func, METH_VARARGS,
		"This function will be ignored in init"},
	{NULL, NULL, 0, NULL}
};


// from _hashopenssl.c
static int
cod_handle_exception(PyObject *module)
{
    _codstate *state = get_cod_state(module);
	// create a custom exception
    state->unsupported_cod_error = PyErr_NewException(
        "cod.UnsupportedCodError", PyExc_ValueError, NULL);
    if (state->unsupported_cod_error == NULL) {
        return -1;
    }
    if (PyModule_AddObjectRef(module, "UnsupportedCodError",
                              state->unsupported_cod_error) < 0) {
        return -1;
    }
	printf("exit 0: cod_handle_exception\n");
    return 0;
}

// copy from _hashopenssl.c
static int
cod_init_constructors(PyObject *module)
{
	/* Create dict from functions to names */
    PyModuleDef *mdef;
    PyMethodDef *fdef;
    PyObject *proxy;
    PyObject *func, *name_obj;
    _codstate *state = get_cod_state(module);

    mdef = PyModule_GetDef(module);
    if (mdef == NULL) {
        return -1;
    }

    state->constructs = PyDict_New();
    if (state->constructs == NULL) {
        return -1;
    }

    for (fdef = mdef->m_methods; fdef->ml_name != NULL; fdef++) {
        if (strncmp(fdef->ml_name, "cod_", 4)) { // signature is cod_
            continue;
        }
        name_obj = PyUnicode_FromString(fdef->ml_name + 4);
        if (name_obj == NULL) {
            return -1;
        }
        func  = PyObject_GetAttrString(module, fdef->ml_name);
        if (func == NULL) {
            Py_DECREF(name_obj);
            return -1;
        }
        int rc = PyDict_SetItem(state->constructs, func, name_obj);
        Py_DECREF(func);
        Py_DECREF(name_obj);
        if (rc < 0) {
            return -1;
        }
    }

    proxy = PyDictProxy_New(state->constructs);
    if (proxy == NULL) {
        return -1;
    }

    int rc = PyModule_AddObjectRef(module, "_constructors", proxy);
    Py_DECREF(proxy);
    if (rc < 0) {
        return -1;
    }
    return 0;
}

// don't forget clear up
static int
cod_clear(PyObject *m)
{
    _codstate *state = get_cod_state(m);
    Py_CLEAR(state->constructs);
    Py_CLEAR(state->unsupported_cod_error);

    return 0;
}

static void
cod_free(void *m)
{
    cod_clear((PyObject *)m);
}

static int
cod_traverse(PyObject *m, visitproc visit, void *arg)
{
    _codstate *state = get_cod_state(m);
    Py_VISIT(state->constructs);
    Py_VISIT(state->unsupported_cod_error);
    return 0;
}

static PyModuleDef_Slot cod_slots[] =
{
	{Py_mod_exec, cod_init_constructors},
    {Py_mod_exec, cod_handle_exception},
	{0, NULL}
};

// set
static struct PyModuleDef _codmodule = {
	PyModuleDef_HEAD_INIT,
	.m_name = "cod",
    .m_doc = "Custom excption demo",
    .m_size = sizeof(_codstate),
    .m_methods = cod_methods,
    .m_slots = cod_slots,
    .m_traverse = cod_traverse,
    .m_clear = cod_clear,
    .m_free = cod_free
};

PyMODINIT_FUNC
PyInit_cod(void)
{
    return PyModuleDef_Init(&_codmodule);
}