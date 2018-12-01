#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0A00
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>
#include <wrl/event.h>

#include <d3d12.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <CommonStates.h>
#include <DDSTextureLoader.h>
#include <DescriptorHeap.h>
#include <DirectXHelpers.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <EffectPipelineStateDescription.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <PostProcess.h>
#include <PrimitiveBatch.h>
#include <RenderTargetState.h>
#include <ResourceUploadBatch.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <SimpleMath.h>
#include <GraphicsMemory.h>
#include <GeometricPrimitive.h>
#include <Effects.h>
#include <Model.h>
#include <Audio.h>

#include "d3dx12.h"

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include <stdio.h>
#include <pix.h>

#include <iostream>
#include <fstream>
#include <string>
#include <Log.h>
#include <WinUser.h>
#include <cctype>

#include "Configs.h"
#include "Align.h"
#include "UIDescriptors.h"

using namespace std;
using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft;
using namespace Microsoft::WRL;

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) : result(hr) {}

        virtual const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}

using namespace DX;