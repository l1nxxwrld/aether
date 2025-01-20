#pragma once
#include <cstdint>

class ID3D11Device;
class ID3D11DeviceContext;

namespace aether::cs2 {

    class CSwapChainDx11;
    class CRenderThreadDx11;

    class CRenderDeviceBase {
    public:

    };

    class CRenderDeviceDx11
        : public CRenderDeviceBase {
    public:
        // 0x330F8 : CRenderThreadDx11*;
        // 0x330C8: ID3D11Device*;
        // 0x330D0: ID3D11DeviceContext*;

        static CRenderDeviceDx11* get();

        CSwapChainDx11* get_swap_chain() const;
        ID3D11Device* get_d3d11_device() const;
        ID3D11DeviceContext* get_d3d11_device_context() const;
    };
}
