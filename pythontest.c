

#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdlib.h>

#define PY_SSIZE_T_CLEAN


#include <Python.h>

#define BUILDING_DLL

int main(int argc, char *argv[])
{
    /*
    // Get a reference to the main module.
    PyObject *main_module =

    PyImport_AddModule("__main__");

    // Get the main module's dictionary
    // and make a copy of it.
    PyObject *main_dict =
        PyModule_GetDict(main_module);
    PyObject *main_dict_copy =
        PyDict_Copy(main_dict);
    FILE *mail = fopen("mail.py", "r");
    Py_Initialize();
    PyRun_File(mail, "mail.py", Py_file_input, main_dict_copy, main_dict_copy);
    Py_Finalize();
    return 0;
    */
    system("python \"mail.py\"");
    return 0;
}