//
// Copyright 2023 Pixar
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
////////////////////////////////////////////////////////////////////////

/* ************************************************************************** */
/* **                                                                      ** */
/* ** This file is generated by a script.                                  ** */
/* **                                                                      ** */
/* ** Do not edit it directly (unless it is within a CUSTOM CODE section)! ** */
/* ** Edit hdSchemaDefs.py instead to make changes.                        ** */
/* **                                                                      ** */
/* ************************************************************************** */

#include "hdPrman/rileyLightInstancePrim.h"

#ifdef HDPRMAN_USE_SCENE_INDEX_OBSERVER

#include "hdPrman/rileyIds.h"
#include "hdPrman/rileyTypes.h"

#include "hdPrman/rileyLightInstanceSchema.h"
#include "hdPrman/rileyCoordinateSystemPrim.h"
#include "hdPrman/rileyGeometryPrototypePrim.h"
#include "hdPrman/rileyLightShaderPrim.h"
#include "hdPrman/rileyMaterialPrim.h"
#include "hdPrman/utils.h"

#include "pxr/imaging/hd/sceneIndex.h"

PXR_NAMESPACE_OPEN_SCOPE

HdPrman_RileyLightInstancePrim::HdPrman_RileyLightInstancePrim(
    HdContainerDataSourceHandle const &primSource,
    const HdsiPrimManagingSceneIndexObserver * const observer,
    HdPrman_RenderParam * const renderParam)
  : HdPrman_RileyPrimBase(renderParam)
{
    HdPrmanRileyLightInstanceSchema schema =
        HdPrmanRileyLightInstanceSchema::GetFromParent(primSource);


    HdPrman_RileyId<HdPrman_RileyGeometryPrototypePrim> groupPrototype =
        HdPrman_RileyId<HdPrman_RileyGeometryPrototypePrim>(
            observer,
            schema.GetGroupPrototype());

    HdPrman_RileyId<HdPrman_RileyGeometryPrototypePrim> geoPrototype =
        HdPrman_RileyId<HdPrman_RileyGeometryPrototypePrim>(
            observer,
            schema.GetGeoPrototype());

    HdPrman_RileyId<HdPrman_RileyMaterialPrim> material =
        HdPrman_RileyId<HdPrman_RileyMaterialPrim>(
            observer,
            schema.GetMaterial());

    HdPrman_RileyId<HdPrman_RileyLightShaderPrim> lightShader =
        HdPrman_RileyId<HdPrman_RileyLightShaderPrim>(
            observer,
            schema.GetLightShader());

    HdPrman_RileyIdList<HdPrman_RileyCoordinateSystemPrim> coordsys =
        HdPrman_RileyIdList<HdPrman_RileyCoordinateSystemPrim>(
            observer,
            schema.GetCoordsys());

    HdPrman_RileyTransform xform =
        HdPrman_RileyTransform(
            schema.GetXform(),
            _GetShutterInterval());

    HdPrman_RileyParamList attributes =
        HdPrman_RileyParamList(
            schema.GetAttributes());


    _rileyId = _AcquireRiley()->CreateLightInstance(
        riley::UserId(),

        groupPrototype.rileyObject,
        geoPrototype.rileyObject,
        material.rileyObject,
        lightShader.rileyObject,
        coordsys.rileyObject,
        xform.rileyObject,
        attributes.rileyObject);

    _groupPrototypePrim = std::move(groupPrototype.prim);

    _geoPrototypePrim = std::move(geoPrototype.prim);

    _materialPrim = std::move(material.prim);

    _lightShaderPrim = std::move(lightShader.prim);

    _coordsysPrims = std::move(coordsys.prims);

// --(BEGIN CUSTOM CODE: Constructor)--
// --(END CUSTOM CODE: Constructor)--
}

void
HdPrman_RileyLightInstancePrim::_Dirty(
    const HdSceneIndexObserver::DirtiedPrimEntry &entry,
    const HdsiPrimManagingSceneIndexObserver * const observer)
{
    HdPrmanRileyLightInstanceSchema schema =
        HdPrmanRileyLightInstanceSchema::GetFromParent(
            observer->GetSceneIndex()->GetPrim(entry.primPath).dataSource);


    std::optional<HdPrman_RileyId<HdPrman_RileyMaterialPrim>> material;
    if (entry.dirtyLocators.Intersects(
            HdPrmanRileyLightInstanceSchema::GetMaterialLocator())) {
        material =
            HdPrman_RileyId<HdPrman_RileyMaterialPrim>(
                observer,
                schema.GetMaterial());
    };

    std::optional<HdPrman_RileyId<HdPrman_RileyLightShaderPrim>> lightShader;
    if (entry.dirtyLocators.Intersects(
            HdPrmanRileyLightInstanceSchema::GetLightShaderLocator())) {
        lightShader =
            HdPrman_RileyId<HdPrman_RileyLightShaderPrim>(
                observer,
                schema.GetLightShader());
    };

    std::optional<HdPrman_RileyIdList<HdPrman_RileyCoordinateSystemPrim>> coordsys;
    if (entry.dirtyLocators.Intersects(
            HdPrmanRileyLightInstanceSchema::GetCoordsysLocator())) {
        coordsys =
            HdPrman_RileyIdList<HdPrman_RileyCoordinateSystemPrim>(
                observer,
                schema.GetCoordsys());
    };

    std::optional<HdPrman_RileyTransform> xform;
    if (entry.dirtyLocators.Intersects(
            HdPrmanRileyLightInstanceSchema::GetXformLocator())) {
        xform =
            HdPrman_RileyTransform(
                schema.GetXform(),
                _GetShutterInterval());
    };

    std::optional<HdPrman_RileyParamList> attributes;
    if (entry.dirtyLocators.Intersects(
            HdPrmanRileyLightInstanceSchema::GetAttributesLocator())) {
        attributes =
            HdPrman_RileyParamList(
                schema.GetAttributes());
    };


    _AcquireRiley()->ModifyLightInstance(
// --(BEGIN CUSTOM CODE: Modify)--
        _groupPrototypePrim ? _groupPrototypePrim->GetRileyId() : riley::GeometryPrototypeId::InvalidId(),
        _rileyId,

        HdPrman_GetRileyObjectPtr(material),
        HdPrman_GetRileyObjectPtr(lightShader),
        HdPrman_GetRileyObjectPtr(coordsys),
        HdPrman_GetRileyObjectPtr(xform),
        HdPrman_GetRileyObjectPtr(attributes)
// --(END CUSTOM CODE: Modify)--
        );


    if (material) {
        _materialPrim = std::move(material->prim);
    }

    if (lightShader) {
        _lightShaderPrim = std::move(lightShader->prim);
    }

    // Now that the LightInstance is using the new
    // RileyCoordinateSystem, we can realease the handles
    // to the old RileyCoordinateSystem.
    if (coordsys) {
        _coordsysPrims = std::move(coordsys->prims);
    }

}

HdPrman_RileyLightInstancePrim::~HdPrman_RileyLightInstancePrim()
{
    _AcquireRiley()->DeleteLightInstance(
// --(BEGIN CUSTOM CODE: Delete)--
        _groupPrototypePrim ? _groupPrototypePrim->GetRileyId() : riley::GeometryPrototypeId::InvalidId(),
        _rileyId
// --(END CUSTOM CODE: Delete)--
        );


    // _groupPrototypePrim gets dropped after the LightInstance was
    // deleted.

    // _geoPrototypePrim gets dropped after the LightInstance was
    // deleted.

    // _materialPrim gets dropped after the LightInstance was
    // deleted.

    // _lightShaderPrim gets dropped after the LightInstance was
    // deleted.

    // _coordsysPrims gets dropped after the LightInstance was
    // deleted.
}

PXR_NAMESPACE_CLOSE_SCOPE

#endif // #ifdef HDPRMAN_USE_SCENE_INDEX_OBSERVER
