//
// Copyright 2022 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/pxr.h"

#include "pxr/usd/usd/usdFileFormat.h"
#include "pxr/usd/sdf/layer.h"
#include "pxr/base/tf/pyStaticTokens.h"

#include <boost/python/bases.hpp>
#include <boost/python/class.hpp>
#include <boost/python/scope.hpp>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

void
wrapUsdFileFormat()
{
    using This = UsdUsdFileFormat;

    scope s = class_<This, bases<SdfFileFormat>, boost::noncopyable>
        ("UsdFileFormat", no_init)

        .def("GetUnderlyingFormatForLayer", 
            &This::GetUnderlyingFormatForLayer)
        .staticmethod("GetUnderlyingFormatForLayer")
        ;

    TF_PY_WRAP_PUBLIC_TOKENS(
        "Tokens",
        UsdUsdFileFormatTokens,
        USD_USD_FILE_FORMAT_TOKENS);
}