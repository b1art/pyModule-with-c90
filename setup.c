#include <Python.h>
#include <stdlib.h>
#include <math.h>

static PyObject *
bisection(PyObject *self, PyObject *args)   //метод бисекции, дихотомии, деления отрезка пополам
{
    PyObject *cb1;
	double eps, x_n, x_k, tmp, x_i;
	double res1, res2;
	int steps = 1;

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
	int steps = 1;

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
	int steps = 1;

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



static PyObject *
muller(PyObject *self, PyObject *args)  //метод Мюллера(парабол)
{
    PyObject *cb1;
	double eps, xnm0, xnm1, xnm2, x1,w;
	double res0, res1, res2;
    int steps = 1;
    if (!PyArg_ParseTuple(args, "Odddd", &cb1, &xnm0, &xnm1, &xnm2, &eps))
        return 0;

    if (!PyCallable_Check(cb1)) {
        PyErr_SetString(PyExc_TypeError, "muller: a callable is required");
        return 0;
    }

	PyObject *arg = Py_BuildValue("(d)", xnm0);
	res0 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
	PyObject *arg1 = Py_BuildValue("(d)", xnm1);
	res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg1));
	PyObject *arg2 = Py_BuildValue("(d)", xnm2);
	res2 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg2));
	w = ((res2-res1)/(xnm2-xnm1))+((res2-res0)/(xnm2-xnm0))-((res1-res0)/(xnm1-xnm0));
	if (fabs(w+sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))) > fabs(w-sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))))
	{
	x1  = xnm2-((2*res2)/(w+sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))));
	}
	else {
	x1  = xnm2-((2*res2)/(w-sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))));
	}
	while(fabs(xnm0-x1)>eps) {
	xnm0 = xnm1;
	xnm1=xnm2;
	xnm2=x1;
	PyObject *arg = Py_BuildValue("(d)", xnm0);
	res0 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
	PyObject *arg1 = Py_BuildValue("(d)", xnm1);
	res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg1));
	PyObject *arg2 = Py_BuildValue("(d)", xnm2);
	res2 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg2));
	w = ((res2-res1)/(xnm2-xnm1))+((res2-res0)/(xnm2-xnm0))-((res1-res0)/(xnm1-xnm0));
	if (fabs(w+sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))) > fabs(w-sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))))
	{
	x1  = xnm2-((2*res2)/(w+sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))));
	}
	else {
	x1  = xnm2-((2*res2)/(w-sqrt(pow(w,2)-4*res2*(((res2-res1)/(xnm2-xnm1))-((res1-res0)/(xnm1-xnm0)))/(xnm2-xnm0))));
	}
	steps++;
	}
	return Py_BuildValue("db", x1, steps);
}


static PyObject *
halley(PyObject *self, PyObject *args)  //метод Галлея
{
    PyObject *cb1, *cb2, *cb3;
	double eps, xn, x1, x0;
	double res1, res2, res3;
    int steps = 1;
    if (!PyArg_ParseTuple(args, "OOOdd", &cb1, &cb2, &cb3, &xn, &eps))
        return 0;

    if (!PyCallable_Check(cb1) || !PyCallable_Check(cb2) || !PyCallable_Check(cb3)) {
        PyErr_SetString(PyExc_TypeError, "helley: a callable is required");
        return 0;
    }

	PyObject *arg = Py_BuildValue("(d)", xn);
	res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
	res2 = PyFloat_AsDouble(PyObject_CallObject(cb2, arg));
	res3 = PyFloat_AsDouble(PyObject_CallObject(cb3, arg));
	x1  = xn - (2*res1*res2/(2*pow(res2,2)-res1*res3));
	x0 = xn;
	while(fabs(x0-x1)>eps) {
		x0 = x1;
		arg = Py_BuildValue("(d)", x1);
		res1 = PyFloat_AsDouble(PyObject_CallObject(cb1, arg));
		res2 = PyFloat_AsDouble(PyObject_CallObject(cb2, arg));
		res3 = PyFloat_AsDouble(PyObject_CallObject(cb3, arg));
		x1  = x0 - ((2*res1*res2)/(2*pow(res2,2)-res1*res3));
		steps++;
	}
	return Py_BuildValue("db", x1, steps);
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
    {
        "muller",
        muller,
        METH_VARARGS,
        "muller function"
    },
    {
        "halley",
        halley,
        METH_VARARGS,
        "halley function"
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
