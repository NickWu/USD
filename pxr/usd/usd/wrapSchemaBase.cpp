//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/pxr.h"
#include "pxr/usd/usd/schemaBase.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include "pxr/external/boost/python/class.hpp"
#include "pxr/external/boost/python/operators.hpp"

using std::string;

PXR_NAMESPACE_USING_DIRECTIVE

using namespace pxr_boost::python;

// We override __getattribute__ for UsdSchemaBase to check object validity
// and raise an exception instead of crashing from Python.

// Store the original __getattribute__ so we can dispatch to it after verifying
// validity.
static TfStaticData<TfPyObjWrapper> _object__getattribute__;

// This function gets wrapped as __getattribute__ on UsdSchemaBase.
static object
__getattribute__(object selfObj, const char *name) {
    // Allow attribute lookups if the attribute name starts with '__', or
    // if the object's prim is valid. Also add explicit exceptions for every
    // method on this base class. The real purpose here is to protect against
    // invalid calls in subclasses which will try to actually manipulate the
    // underlying (invalid) prim and likely crash.
    if ((name[0] == '_' && name[1] == '_') ||
        extract<UsdSchemaBase &>(selfObj)().GetPrim().IsValid() ||
        strcmp(name, "GetPrim") == 0 ||
        strcmp(name, "GetPath") == 0 ||
        strcmp(name, "GetSchemaClassPrimDefinition") == 0 ||
        strcmp(name, "GetSchemaAttributeNames") == 0 ||
        strcmp(name, "GetSchemaKind") == 0 ||
        strcmp(name, "IsAPISchema") == 0 ||
        strcmp(name, "IsConcrete") == 0 ||
        strcmp(name, "IsTyped") == 0 ||
        strcmp(name, "IsAppliedAPISchema") == 0 ||
        strcmp(name, "IsMultipleApplyAPISchema") == 0) {
        // Dispatch to object's __getattribute__.
        return (*_object__getattribute__)(selfObj, name);
    } else {
        // Otherwise raise a runtime error.
        TfPyThrowRuntimeError(
            TfStringPrintf("Accessed schema on invalid prim"));
    }
    // Unreachable.
    return object();
}

void wrapUsdSchemaBase()
{
    class_<UsdSchemaBase> cls("SchemaBase");

    cls
        .def(init<UsdPrim>(arg("prim")))
        .def(init<UsdSchemaBase const&>(arg("otherSchema")))
        .def(TfTypePythonClass())

        .def("GetPrim", &UsdSchemaBase::GetPrim)
        .def("GetPath", &UsdSchemaBase::GetPath)
        .def("GetSchemaClassPrimDefinition",
             &UsdSchemaBase::GetSchemaClassPrimDefinition,
             return_internal_reference<>())
        .def("GetSchemaAttributeNames", &UsdSchemaBase::GetSchemaAttributeNames,
             arg("includeInherited")=true,
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("IsAPISchema", &UsdSchemaBase::IsAPISchema)
        .def("IsConcrete", &UsdSchemaBase::IsConcrete) 
        .def("IsTyped", &UsdSchemaBase::IsTyped) 
        .def("IsAppliedAPISchema", &UsdSchemaBase::IsAppliedAPISchema) 
        .def("IsMultipleApplyAPISchema", &UsdSchemaBase::IsMultipleApplyAPISchema) 

        .def("GetSchemaKind", &UsdSchemaBase::GetSchemaKind)

        .def(!self)

        ;

    // Save existing __getattribute__ and replace.
    *_object__getattribute__ = object(cls.attr("__getattribute__"));
    cls.def("__getattribute__", __getattribute__);
}
