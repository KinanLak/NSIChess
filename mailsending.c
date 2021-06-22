#include <Python.h>
int main()
{
    PyObject *pName, *pModule, *pDict, *pFun, *pValue;
    // Initialize the Python Interpreter
    Py_Initialize();
    // Build the name object
    pName = PyString_FromString("C:\\Users\\Kinan\\Documents\\GitHub\\NSIChess\\mail.py");
    if (pName)
        printf("OK");

    // Load the module object
    pModule = PyImport_Import(pName);

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference
    pFun = PyDict_GetItemString(pDict, "prinTname");

    if (PyCallable_Check(pFun))
    {
        PyObject_CallObject(pFun, NULL);
    }
    else
    {
        PyErr_Print();
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    Py_DECREF(pDict);
    Py_DECREF(pFun);
    // Finish the Python Interpreter
    Py_Finalize();
    getchar();
    return 0;
}