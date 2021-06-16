#include "Python.h"

int main(int argc, char *argv[])
{
    // Get a reference to the main module.
    PyObject *main_module =
    PyImport_AddModule("__main__");
    PyImport_AddModule("smtplib");
    PyImport_AddModule("csv");
    PyImport_AddModule("email.message");

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
}