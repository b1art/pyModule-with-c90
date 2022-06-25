#include <Python.h>
#include <stdlib.h>
#include <math.h>

static PyObject *
bisection(PyObject *self, PyObject *args)   //метод бисекции, дихотомии, деления отрезка пополам
{
    PyObject *cb1;
	double eps, x_n, x_k, tmp, x_i;
	double res1, res2;
	int steps=0;

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
        steps++;
        }
	}
	return Py_BuildValue("db", x_i, steps);
}

static PyObject *
chord(PyObject *self, PyObject *args)   //метод хорд, секущих
{
    PyObject *cb1;
	double eps, x_next, x_curr;
	double res1, res2;
	int steps=0;

    if (!PyArg_ParseTuple(args, "Oddd", &cb1, &x_curr, &x_next, &eps))
        return 0;

    if (!PyCallable_Check(cb1)) {
        PyErr_SetString(PyExc_TypeError, "chord: a callable is required");
        return 0;
    }

	PyObject *arg = Py_BuildValue("(d)", x_curr);
	res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
	PyObject *arg1 = Py_BuildValue("(d)", x_next);
	res2 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg1));
	while(fabs(x_next - x_curr) > eps)
	{
        x_curr = x_next - (x_next - x_curr) * res2 / (res2 - res1);
        PyObject *arg = Py_BuildValue("(d)", x_curr);
        res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));

        x_next = x_curr - (x_curr - x_next) * res1 / (res1 - res2);
        PyObject *arg1 = Py_BuildValue("(d)", x_next);
        res2 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg1));
        steps++;
	}
	return Py_BuildValue("db", x_next,steps);
}

static PyObject *
newton(PyObject *self, PyObject *args)  //метод Ньютона
{
    PyObject *cb1, *cb2;
	double eps, xn, x1, x0;
	double res1, res2;
	int steps=0;

    if (!PyArg_ParseTuple(args, "OOdd", &cb1, &cb2, &xn, &eps))
        return 0;

    if (!PyCallable_Check(cb1) || !PyCallable_Check(cb2)) {
        PyErr_SetString(PyExc_TypeError, "newton: a callable is required");
        return 0;
    }

	PyObject *arg = Py_BuildValue("(d)", xn);
	res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
	res2 = PyFloat_AsDouble(PyObject_CallObject(cb2, arg));
	x1 = xn - res1/res2;
	x0 = xn;
	while (fabs (x0 - x1) > eps) {
		arg = Py_BuildValue("(d)", x1);
		res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
		res2 = PyFloat_AsDouble(PyObject_CallObject(cb2, arg));
		x0 = x1;
		x1 = x1 - res1/res2;
		steps++;
	}
	return Py_BuildValue("db", x1,steps);
}


static PyMethodDef ownmod_methods[] = {
    {
        "bisection",
        bisection,
        METH_VARARGS,
        "bisection function"
    },
    {
        "chord",
        chord,
        METH_VARARGS,
        "chord function"
    },
    {
        "newton",
        newton,
        METH_VARARGS,
        "newton function"
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