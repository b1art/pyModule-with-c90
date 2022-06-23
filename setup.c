#include <Python.h>
#include <stdlib.h>
#include <math.h>

static PyObject *
bisection(PyObject *self, PyObject *args)
{
    PyObject *cb1;
	double eps, x_n, x_k, tmp, x_i;
	double res1, res2;

    if (!PyArg_ParseTuple(args, "Oddd", &cb1, &x_n, &x_k, &eps))
        return 0;

    if (!PyCallable_Check(cb1)) {
        PyErr_SetString(PyExc_TypeError, "bisection: a callable is required");
        return 0;
    }

	PyObject *arg = Py_BuildValue("(d)", x_n);
	res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
	PyObject *arg1 = Py_BuildValue("(d)", x_k);
	res2 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg1));
	if (res1 == 0)
	    return Py_BuildValue("d", x_n);
	if (res2 == 0)
	    return Py_BuildValue("d", x_k);

	while(fabs(x_k - x_n) > eps)
	{
        tmp = (x_k - x_n) / 2;
        x_i = x_n + tmp;
        PyObject *arg = Py_BuildValue("(d)", x_n);
        res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
        PyObject *arg1 = Py_BuildValue("(d)", x_i);
        res2 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg1));
        if ((res1 < 0 && res2 > 0)||(res1 > 0 && res2 < 0)){
        x_k = x_i;
        }
        else {
        x_n = x_i;
        }
	}
	return Py_BuildValue("d", x_i);
}


static PyMethodDef ownmod_methods[] = {
    {
        "bisection",
        bisection,
        METH_VARARGS,
        "bisection function"
    },
    { NULL, NULL, 0, NULL }
};


static PyModuleDef simple_module = {
    /* Info about module */
    PyModuleDef_HEAD_INIT,
    "solver", // solver.__name__
    "amazing documentation", // solver.__doc__
    -1,
    ownmod_methods, // methods are here
    NULL,
    NULL,
    NULL,
    NULL
};


PyMODINIT_FUNC PyInit_solver(void)
{
    PyObject* m;
    // creating module object
    m = PyModule_Create(&simple_module);
    if (m == NULL)
        return NULL;

    return m;
}