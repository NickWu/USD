//
// Copyright 2020 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/pxr.h"

#include "pxr/imaging/hdSt/unitTestGLDrawing.h"
#include "pxr/imaging/hdSt/unitTestHelper.h"

#include "pxr/imaging/hdSt/resourceRegistry.h"
#include "pxr/imaging/hdSt/textureObjectRegistry.h"
#include "pxr/imaging/hdSt/textureObject.h"
#include "pxr/imaging/hdSt/textureIdentifier.h"
#include "pxr/imaging/hdSt/glslProgram.h"

#include "pxr/base/tf/errorMark.h"
#include "pxr/base/tf/stackTrace.h"

#include <iostream>

PXR_NAMESPACE_USING_DIRECTIVE

class My_TestGLDrawing : public HdSt_UnitTestGLDrawing {
public:
    My_TestGLDrawing() = default;
    
    // HdSt_UnitTestGLDrawing overrides
    void InitTest() override;
    void DrawTest() override;
    void OffscreenTest() override;
    
private:
    std::unique_ptr<HdSt_TextureTestDriver> _driver;
    std::unique_ptr<HdSt_TextureObjectRegistry> _registry;
};

void
My_TestGLDrawing::InitTest()
{
    _driver = std::make_unique<HdSt_TextureTestDriver>();
    _registry =
        std::make_unique<HdSt_TextureObjectRegistry>(
            _driver->GetResourceRegistry().get());
}

void
_CheckEqual(const int64_t a, const int64_t b, const char * const msg)
{
    if (a != b) {
        std::cout << msg << std::endl;
        std::cout << "Values: " << a << " " << b << std::endl;
        exit(EXIT_FAILURE);
    }
}

template<typename T>
void _CheckEqual(const T &a, const T &b, const char * const msg)
{
    if (a != b) {
        std::cout << msg << std::endl;
        exit(EXIT_FAILURE);
    }
}

void
My_TestGLDrawing::DrawTest()
{
    std::cout << "DrawTest not supported" << std::endl;
    exit(1);
}

void
My_TestGLDrawing::OffscreenTest()
{
    const int width = GetWidth();
    const int height = GetHeight();

    // Make output texture
    HgiTextureDesc texDesc;
    texDesc.debugName = "Output My_TestGLDrawing";
    texDesc.usage = HgiTextureUsageBitsColorTarget;
    texDesc.type = HgiTextureType2D;
    texDesc.dimensions = GfVec3i(GetWidth(), GetHeight(), 1);
    texDesc.layerCount = 1;
    texDesc.format = HgiFormatFloat32Vec4;
    texDesc.mipLevels = 1;
    texDesc.pixelsByteSize = 
        HgiGetDataSize(texDesc.format, texDesc.dimensions);

    // Fill output texture with dark gray
    std::vector<float> initialData;
    initialData.resize(width * height * 4);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            initialData[width * 4 * i + 4 * j + 0] = 0.1;
            initialData[width * 4 * i + 4 * j + 1] = 0.1;
            initialData[width * 4 * i + 4 * j + 2] = 0.1;
            initialData[width * 4 * i + 4 * j + 3] = 1.f;
        }
    }
    texDesc.initialData = initialData.data();

    HgiTextureHandle dstTexture = _driver->GetHgi()->CreateTexture(texDesc);

    // Make sampler object to use with the various input textures.
    HgiSamplerDesc samplerDesc;
    samplerDesc.magFilter = HgiSamplerFilterLinear;
    samplerDesc.minFilter = HgiSamplerFilterLinear;
    samplerDesc.mipFilter = HgiMipFilterLinear;
    HgiSamplerHandle sampler = _driver->GetHgi()->CreateSampler(samplerDesc);
    
    {
        HdStTextureObjectSharedPtr const texture1 =
            _registry->AllocateTextureObject(
                HdStTextureIdentifier(TfToken("texture1.png")),
                HdStTextureType::Uv);

        // Check that texture gets committed
        _CheckEqual(
            _registry->Commit(), { texture1 },
            "Texture 1 not committed");

        {
            HdStUvTextureObject * const uvTextureObject =
            dynamic_cast<HdStUvTextureObject*>(texture1.get());
            if (!uvTextureObject) {
                std::cout << "Invalid UV texture object" << std::endl;
                exit(EXIT_FAILURE);
            }

            _driver->Draw(dstTexture, uvTextureObject->GetTexture(), sampler);
            _driver->WriteToFile(dstTexture, "outTexture1FullRes.png");
        }

        _CheckEqual(
            _registry->GetTotalTextureMemory(), 349524,
            "Total texture memory wrong after first commit");

        // Garbage collect should have no effect.
        _registry->GarbageCollect();    

        // Check that changing target memory will recommit texture and
        // down-sample it before uploading it
        texture1->SetTargetMemory(4000);
        _CheckEqual(
            _registry->Commit(), { texture1 },
            "Texture 1 not recommitted");

        {
            HdStUvTextureObject * const uvTextureObject =
            dynamic_cast<HdStUvTextureObject*>(texture1.get());
            if (!uvTextureObject) {
                std::cout << "Invalid UV texture object" << std::endl;
                exit(EXIT_FAILURE);
            }

            _driver->Draw(dstTexture, uvTextureObject->GetTexture(), sampler);
            _driver->WriteToFile(dstTexture, "outTexture1LowRes.png");
        }

        _CheckEqual(
            _registry->GetTotalTextureMemory(), 1364,
            "Total texture memory wrong after changing target memory");
        
        // We that we get the same handle when querrying the same texture
        // again.
        _CheckEqual(
            _registry->AllocateTextureObject(
                HdStTextureIdentifier(TfToken("texture1.png")),
                HdStTextureType::Uv),
            texture1,
            "Texture was not de-duplicated");

        // Texture registry should have nothing to commit.
        _CheckEqual(
            _registry->Commit(), { },
            "Unnecessary commit");

        _CheckEqual(
            _registry->GetTotalTextureMemory(), 1364,
            "Total texture memory wrong after unnecessary commit");
    }

    // Texture 1 was dropped and can be garbaged collected.
    _registry->GarbageCollect();    

    _CheckEqual(
        _registry->GetTotalTextureMemory(), 0,
        "Total texture memory wrong after dropping all textures.");
        
    {
        HdStTextureObjectSharedPtr const texture1 =
            _registry->AllocateTextureObject(
                HdStTextureIdentifier(TfToken("texture1.png")),
                HdStTextureType::Uv);

        // Texture 1 has to be committed again since it was garbage
        // collected. Target memory should be reset.
        _CheckEqual(
            _registry->Commit(), { texture1 },
            "Texture 1 not committed again after it was dropped");

        {
            HdStUvTextureObject * const uvTextureObject =
                dynamic_cast<HdStUvTextureObject*>(texture1.get());
            if (!uvTextureObject) {
                std::cout << "Invalid UV texture object" << std::endl;
                exit(EXIT_FAILURE);
            }

            _driver->Draw(dstTexture, uvTextureObject->GetTexture(), sampler);
            _driver->WriteToFile(dstTexture, "outTexture1Reloaded.png");
        }

        // Sanity check, use a different texture.
        HdStTextureObjectSharedPtr const texture2 =
            _registry->AllocateTextureObject(
                HdStTextureIdentifier(TfToken("texture2.png")),
                HdStTextureType::Uv);
        
        _CheckEqual(
            _registry->Commit(), { texture2 },
            "Texture 2 was not committed");

        {
            HdStUvTextureObject * const uvTextureObject =
                dynamic_cast<HdStUvTextureObject*>(texture2.get());
            if (!uvTextureObject) {
                std::cout << "Invalid UV texture object" << std::endl;
                exit(EXIT_FAILURE);
            }

            _driver->Draw(dstTexture, uvTextureObject->GetTexture(), sampler);
            _driver->WriteToFile(dstTexture, "outTexture2.png");
        }

        _CheckEqual(
            _registry->GetTotalTextureMemory(), 349524 + 349524,
            "Total texture memory wrong with two textures");
    }

    // Check a grayscale texture.
    {
        HdStTextureObjectSharedPtr const texture =
            _registry->AllocateTextureObject(
                HdStTextureIdentifier(TfToken("grayscaleTexture.png")),
                HdStTextureType::Uv);
        
        _CheckEqual(
            _registry->Commit(), { texture },
            "Grayscale texture not committed");

        HdStUvTextureObject * const uvTextureObject =
            dynamic_cast<HdStUvTextureObject*>(texture.get());
        if (!uvTextureObject) {
            std::cout << "Invalid UV texture object" << std::endl;
            exit(EXIT_FAILURE);
        }

        _driver->Draw(dstTexture, uvTextureObject->GetTexture(), sampler);
        _driver->WriteToFile(dstTexture, "outGrayscaleTexture.png");

        _CheckEqual(
            _registry->GetTotalTextureMemory(), 349524 + 349524 + 409588,
            "Total texture memory wrong three textures");
    }

    _driver->GetHgi()->DestroyTexture(&dstTexture);
    _driver->GetHgi()->DestroySampler(&sampler);
    
    // Clean-up things.
    _registry->GarbageCollect();

    _CheckEqual(
        _registry->GetTotalTextureMemory(), 0,
        "Total texture memory wrong after final garbage collection");
}

void
BasicTest(int argc, char *argv[])
{
    My_TestGLDrawing driver;
    driver.RunTest(argc, argv);
}

int main(int argc, char *argv[])
{
    TfErrorMark mark;

    BasicTest(argc, argv);

    if (mark.IsClean()) {
        std::cout << "OK" << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cout << "FAILED" << std::endl;
        return EXIT_FAILURE;
    }
}
