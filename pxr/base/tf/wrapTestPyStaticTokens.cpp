//
// Copyright 2016 Pixar
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
/// \file wrapTestPyStaticTokens.cpp

#include "pxr/pxr.h"

#include "pxr/base/tf/pyStaticTokens.h"

PXR_NAMESPACE_OPEN_SCOPE

#define TF_TEST_TOKENS                  \
    (orange)                            \
    ((pear, "d'Anjou"))                 

TF_DECLARE_PUBLIC_TOKENS(tfTestStaticTokens, TF_API, TF_TEST_TOKENS);
TF_DEFINE_PUBLIC_TOKENS(tfTestStaticTokens, TF_TEST_TOKENS);

PXR_NAMESPACE_CLOSE_SCOPE

PXR_NAMESPACE_USING_DIRECTIVE

namespace {
struct _DummyScope {
};
}

void
wrapTf_TestPyStaticTokens()
{
    TF_PY_WRAP_PUBLIC_TOKENS("_testStaticTokens",
                             tfTestStaticTokens, TF_TEST_TOKENS);

    boost::python::class_<_DummyScope, boost::noncopyable>
        cls("_TestStaticTokens", boost::python::no_init);
    boost::python::scope testScope = cls;

    TF_PY_WRAP_PUBLIC_TOKENS_IN_CURRENT_SCOPE(
        tfTestStaticTokens, TF_TEST_TOKENS);
}
