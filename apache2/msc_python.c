/*
* ModSecurity for Apache 2.x, http://www.modsecurity.org/
* Copyright (c) 2004-2013 Trustwave Holdings, Inc. (http://www.trustwave.com/)
*
* You may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* If any of the files related to licensing are missing or if you have any
* other questions related to licensing please contact Trustwave Holdings, Inc.
* directly using the email address security@modsecurity.org.
*/

#ifdef WITH_PYTHON

#include "msc_python.h"

#include "apr_lib.h"
#include "apr_strmatch.h"
#include "apr_strings.h"
#include "apache2.h"
#include <Python.h>

#define PY_STR_DBG(arg, msr, z) { if (arg == NULL) { msr_log(msr, 8, "Python -%s-:", z); } else { PyObject *e = PyObject_Repr(arg); PyObject *a = PyUnicode_AsEncodedString(e, "utf-8", NULL); char *s = PyBytes_AsString(a); msr_log(msr, 8, "Python -%s-: %s", z, s); } }

/* ModSecurityI */

static PyObject *pyModSecurityI_log(PyObject *self, PyObject *args, PyObject *kwds) {
    char *str = NULL;
    int level = 0;
    PyObject *capsuleModSecurity = NULL;
    modsec_rec *msr = NULL;
    
    if (PyArg_ParseTuple(args, "is", &level, &str) == 0)
    {
        /* PyArg already set this.
         * PyErr_SetString(PyExc_TypeError, "log() takes exactly 2 arguments.");
         */
        goto end;
    }

    capsuleModSecurity = PyObject_GetAttrString(self, "capsuleModSecurity");
    if (capsuleModSecurity == NULL)
    {
        // FIXME: Use the correct error paramenter.
        PyErr_SetString(PyExc_TypeError, "log() needs ModSecurity core to be attached.");
        goto end;
    }

    msr = PyCapsule_GetPointer(capsuleModSecurity, "modsecurity");
    if (msr == NULL)
    {
        // FIXME: Use the correct error paramenter.
        PyErr_SetString(PyExc_TypeError, "log() needs ModSecurity core to be attached.");
        goto end_no_msr;
    }
    //msr_log(msr, 1, "%s", str);
    msr_log(msr, level, str);

end_no_msr:
    Py_DECREF(capsuleModSecurity);
end:
    Py_RETURN_NONE;
}

static PyObject *pyModSecurityI_setCapsuleModSecurity(PyObject *self, PyObject *args, PyObject *kwds) {
    PyObject *capsule = NULL;

    if (PyArg_ParseTuple(args, "O", &capsule) == 0)
    {
        /* PyArg already set this.
         * PyErr_SetString(PyExc_TypeError, "setCapsuleModSecurity() takes exactly 1 argument.");
         */
        goto end;
    }

    if (capsule == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setCapsuleModSecurity() Capsule cannot be NULL.");
        goto end;
    }

    if (PyModule_AddObject(self, "capsuleModSecurity", capsule) == -1)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setCapsuleModSecurity() Failed to save capsule.");
        goto end;
    }

end:
    Py_RETURN_NONE;
}

static PyObject *pyModSecurityI_setCapsuleRule(PyObject *self, PyObject *args, PyObject *kwds) {
    PyObject *capsule = NULL;

    if (PyArg_ParseTuple(args, "O", &capsule) == 0)
    {
        /* PyArg already set this.
         * PyErr_SetString(PyExc_TypeError, "setCapsuleRule() takes exactly 1 argument.");
         */
        goto end;
    }

    if (capsule == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setCapsuleRule() Capsule cannot be NULL.");
        goto end;
    }

    if (PyModule_AddObject(self, "capsuleRule", capsule) == -1)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setCapsuleRule() Failed to save capsule.");
        goto end;
    }

end:
    Py_RETURN_NONE;
}

static PyObject *pyModSecurityI_getVariable(PyObject *self, PyObject *args, PyObject *kwds) {
    char *my_error_msg = NULL;
    const char *var_name = NULL;
    const char *param_name = NULL;
    const apr_array_header_t *arr = NULL;
    PyObject *ret = Py_None;
    PyObject *capsuleRule = NULL;
    PyObject *capsuleModSecurity = NULL;
    msre_rule *rule = NULL;
    msre_var *var = NULL;
    modsec_rec *msr = NULL;
    apr_table_t *vartab = NULL;



    if (PyArg_ParseTuple(args, "sz", &var_name, &param_name) == 0)
    {
        goto end;
    }
    
    capsuleRule = PyObject_GetAttrString(self, "capsuleRule");
    if (capsuleRule == NULL)
    {

        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "getVariable() needs ModSecurity core to be attached.");
        goto end_no_rule;
    }

    capsuleModSecurity = PyObject_GetAttrString(self, "capsuleModSecurity");
    if (capsuleModSecurity == NULL)
    {
        
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "getVariable() needs ModSecurity core to be attached.");
        goto end_no_msr;
    }

    rule = PyCapsule_GetPointer(capsuleRule, "rule");
    msr = PyCapsule_GetPointer(capsuleModSecurity, "modsecurity");

    if (rule == NULL || msr == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "getVariable() needs ModSecurity core to be attached.");
        goto end_no_rule_or_msr;
    }

    if(param_name == NULL){
        msr_log(msr, 9, "getVariable(): prased collection %s with no parameters",var_name);
        var = msre_create_var_ex(msr->msc_rule_mptmp, msr->modsecurity->msre,
        var_name, '\0', msr, &my_error_msg);
    }else{
        msr_log(msr, 9, "getVariable(): prased collection %s with paramater %s",var_name, param_name);
        var = msre_create_var_ex(msr->msc_rule_mptmp, msr->modsecurity->msre,
        var_name, param_name, msr, &my_error_msg);
    }

    if (var == NULL)
    {
        PyErr_SetString(PyExc_KeyError, "getVariable() did not receive a valid collection name");
        msr_log(msr, 1, "Warning: getVariable() did not receive a valid collection name %s",var_name);
        goto end_no_var;
    }

    vartab = apr_table_make(msr->msc_rule_mptmp, 16);
    if (vartab == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "getVariable() Internal error, failed to create var table.");
        goto end_no_vartab;
    }

    var->metadata->generate(msr, var, rule, vartab, msr->msc_rule_mptmp);
    arr = apr_table_elts(vartab);
    
    if (arr->nelts == 1)
    {
        msre_var *vx = generate_single_var(msr, var, NULL, rule, msr->msc_rule_mptmp);
        if (vx != NULL)
        {
            ret = Py_BuildValue("s", vx->value);
        }
    }
    else if (arr->nelts > 1)
    { 
        // FIXME: We should have an object to encapsulate this dictionary, in order to auto-save the changes.
        int i = 0;
        const apr_table_entry_t *te = NULL;
        PyObject *pDict = PyDict_New();

        te = (apr_table_entry_t *)arr->elts;

        for (i = 0; i < arr->nelts; i++)
        {
            msre_var *str = (msre_var *) te[i].val;
            PyDict_SetItemString(pDict, str->name + strlen(var->name) + 1, Py_BuildValue("s", str->value));
        }

        ret = pDict;
    }

end_no_vartab:
end_no_var:
end_no_rule_or_msr:
    Py_DECREF(capsuleModSecurity);
end_no_msr:
    Py_DECREF(capsuleRule);
end_no_rule:
end:
    return ret;
}

static PyObject *pyModSecurityI_applyTransformation(PyObject *self, PyObject *args, PyObject *kwds)
{

    // TODO: Implement.
    
    PyErr_SetString(PyExc_TypeError, "applyTransformation() is not ready yet.");

    Py_RETURN_NONE;
}

static PyObject *pyModSecurityI_setVariable(PyObject *self, PyObject *args, PyObject *kwds)
{
    char *name = NULL;
    char *value = NULL;
    PyObject *ret = Py_None;
    PyObject *capsuleRule = NULL;
    PyObject *capsuleModSecurity = NULL;
    msre_rule *rule = NULL;
    modsec_rec *msr = NULL;
 
    if (PyArg_ParseTuple(args, "ss", &name, &value) == 0)
    {
        goto end;
    }

    capsuleRule = PyObject_GetAttrString(self, "capsuleRule");
    if (capsuleRule == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setVariable() needs ModSecurity core to be attached.");
        goto end_no_rule;
    }

    capsuleModSecurity = PyObject_GetAttrString(self, "capsuleModSecurity");
    if (capsuleModSecurity == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setVariable() needs ModSecurity core to be attached.");
        goto end_no_msr;
    }

    rule = PyCapsule_GetPointer(capsuleRule, "rule");
    msr = PyCapsule_GetPointer(capsuleModSecurity, "modsecurity");
    if (rule == NULL || msr == NULL)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setVariable() needs ModSecurity core to be attached.");
        goto end_no_rule_or_msr;
    }


    if (msre_action_setvar_execute(msr, msr->msc_rule_mptmp, rule, name, value) != 1)
    {
        // FIXME: Use the correct error object.
        PyErr_SetString(PyExc_TypeError, "setVariable() Internal error. Failed to save variable.");
        goto end_failed_to_set;
    }

end_failed_to_set:
end_no_rule_or_msr:
    Py_DECREF(capsuleModSecurity);
end_no_msr:
    Py_DECREF(capsuleRule);
end_no_rule:
end:
    return ret;
}


static PyMethodDef pyModSecurityI_functions[] = {
    { "log", (PyCFunction)pyModSecurityI_log, METH_VARARGS, NULL },
    { "setCapsuleModSecurity", (PyCFunction)pyModSecurityI_setCapsuleModSecurity, METH_VARARGS, NULL },
    { "setCapsuleRule", (PyCFunction)pyModSecurityI_setCapsuleRule, METH_VARARGS, NULL },
    { "getVariable", (PyCFunction)pyModSecurityI_getVariable, METH_VARARGS, NULL },
    { "setVariable", (PyCFunction)pyModSecurityI_setVariable, METH_VARARGS, NULL },
    { "applyTransformation", (PyCFunction)pyModSecurityI_applyTransformation, METH_VARARGS, NULL },
    { NULL, NULL }
};


static struct PyModuleDef pyModSecurityI_def = {
        PyModuleDef_HEAD_INIT,
        "ModSecurityI",
        NULL,
        -1,
        pyModSecurityI_functions,
        NULL,
        NULL,
        NULL,
        NULL
};


// FIXME: Split files.

/**
 *
 */
char *python_load(msc_python_script **script, const char *filename, apr_pool_t *pool)
{
    PyObject *pName, *pModule;
    const char *path = NULL;
    const char *file = NULL;
    const char *module = NULL;

    /*
     * Script path?
     * FIXME: Avoid to use apr_ functions
     */
    file = apr_filepath_name_get(filename);
    path = apr_pstrndup(pool, filename, strlen(filename) - strlen(file));

    Py_Initialize();

    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Append(sysPath, PyUnicode_FromFormat("."));
    PyList_Append(sysPath, PyUnicode_FromFormat(path));

    module = apr_pstrndup(pool, file, strlen(file) - strlen(".py"));
    Py_SetProgramName((wchar_t *)module);  /* FIXME */

    pName = PyUnicode_FromString(module);
    pModule = PyImport_Import(pName);

    if (pModule == NULL) {
        const char *s = NULL;
        PyObject *err = NULL;
        PyObject *exc_type = NULL, *exc_value = NULL, *exc_tb = NULL;

        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); //Now a unicode object
        PyObject* pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);

        return apr_psprintf(pool, "ModSecurity: Failed to load script: %s - %s",
            filename, s);
    }

    (*script) = apr_pcalloc(pool, sizeof(msc_python_script));
    (*script)->name = strdup(filename);
    (*script)->pName = pName;
    (*script)->pModule = pModule;
    (*script)->extInstance = NULL;

    Py_DECREF(pName);

    return NULL;
}


// FIXME: Error handling
int python_execute(msc_python_script *script, char *param, modsec_rec *msr, msre_rule *rule, char **error_msg) {
    apr_time_t time_before;
    int ret = RULE_NO_MATCH;
    char * s = NULL;
    PyObject* methodRes = NULL;
    PyObject *extObject = NULL;
    PyObject *pmeth = NULL; 
    PyObject *pargs = NULL;
    PyObject *exc_type = NULL;
    PyObject *exc_value = NULL;
    PyObject *exc_tb = NULL;
    PyObject *err = NULL;
    PyObject* pyStr = NULL;

    if (error_msg == NULL) {
        return -1;
    }

    *error_msg = NULL;

//    if (script->extInstance == NULL) // FIXME: without cache this will draing the performance.
//    {
    extObject = PyObject_GetAttrString(script->pModule, "ModSecurityExtension");
    script->extInstance = PyObject_CallObject(extObject, NULL);

    PyObject *logMod = PyModule_Create(&pyModSecurityI_def);
    if (logMod == NULL){
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); 
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error couldn't create ModSecurityI module: %s", s); 
        goto end;
    }
    PyObject *capsule_msr = PyCapsule_New(msr, "modsecurity", NULL);
    if(capsule_msr == NULL){
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); 
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error couldn't create capsule for pointer to 'modsecurity': %s", s);
        goto end;
    }
    PyObject *capsule_rule = PyCapsule_New(rule, "rule", NULL);
    if(capsule_rule == NULL){
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); 
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error couldn't create capsule for pointer to 'rule': %s", s); 
         goto end;
    }
    PyObject* setCapsule1 = PyObject_CallMethod(logMod, "setCapsuleModSecurity", "(O)", capsule_msr);
    if(setCapsule1 == NULL){
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); 
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error could not call method setCapsuleModSecurity(): %s", s);
        goto end;
    }
    PyObject* setCapsule2 = PyObject_CallMethod(logMod, "setCapsuleRule", "(O)", capsule_rule);

    if(setCapsule2 == NULL){
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); 
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error could not call method setCapsuleRule(): %s", s); 
        goto end;
    }
    
    PyObject* setLog = PyObject_CallMethod(script->extInstance, "setModSecurityCore", "(O)", logMod);
    if (setLog == NULL){
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); 
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error couldn't call method setModSecurityCore: %s", s);
        goto end;
    }
    Py_DECREF(logMod);
    Py_DECREF(setLog);
    methodRes = PyObject_CallMethod(script->extInstance, "process", NULL);
    if (methodRes == NULL)
    {
        PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
        err = PyObject_Repr(exc_value); //Now a unicode object
        pyStr = PyUnicode_AsEncodedString(err, "utf-8", NULL);
        s = PyBytes_AS_STRING(pyStr);
        *error_msg = apr_psprintf(msr->mp, "Error couldn't call method process: %s", s);
        goto end;
    }
    if (methodRes == Py_True)
    {
        msr_log(msr, 7, "Python script matched");
        ret = RULE_MATCH;
    }

    Py_DECREF(methodRes);
end: 
    Py_XDECREF(pyStr);
    Py_XDECREF(exc_type);
    Py_XDECREF(exc_value);
    Py_XDECREF(exc_tb);
    return ret;
}

#endif /* WITH_PYTHON */
