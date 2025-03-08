#pragma once
#include "CL/cl.hpp"
#include "CL/platform.hpp"

namespace cl {
    class Device final : public details::Wrapper<cl_device_id> {
    public:
        Device() : platform_() {
            clRUN(clGetDeviceIDs, platform_.Get(), CL_DEVICE_TYPE_DEFAULT, 1, &obj_, nullptr);
        }

        Device(cl_device_id id, const Platform& platform) : details::Wrapper<cl_device_id>(id), platform_(platform) {}

        Device(const Platform &platform) : platform_(platform) {
            cl_int err = clGetDeviceIDs(platform_.Get(), CL_DEVICE_TYPE_GPU, 1, &obj_, NULL);
            if (err == CL_DEVICE_NOT_FOUND)
                clRUN(clGetDeviceIDs, platform_.Get(), CL_DEVICE_TYPE_CPU, 1, &obj_, nullptr);
        }

        Device(const Device &other) : details::Wrapper<cl_device_id>(other), platform_(other.platform_) {}

        Device &operator=(const Device &other) {
            details::Wrapper<cl_device_id>::operator=(other);
            platform_ = other.platform_;
            return *this;
        }

        Device(Device &&other) noexcept : details::Wrapper<cl_device_id>(std::move(other)) {
            std::swap(platform_, other.platform_);
        }

        Device &operator=(Device &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_device_id>::operator=(std::move(other));
                std::swap(platform_, other.platform_);
            }

            return *this;
        }
        
        template <cl_device_info param_name>
        typename details::param_traits<cl_device_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_device_id>::template GetInfo<param_name>(obj_);
        }

    private:
        Platform platform_;
    }; // class Device

    void Platform::GetDevices(cl_device_type device_type, std::vector<Device> &devices) const {
        cl_uint num_devices;

        clRUN(clGetDeviceIDs, obj_, device_type, 0, nullptr, &num_devices);
        
        if (num_devices == 0) {
            throw std::runtime_error("devices were not found in this platform");
        }

        std::vector<cl_device_id> device_ids(num_devices);

        clRUN(clGetDeviceIDs, obj_, device_type, num_devices, device_ids.data(), nullptr);

        devices.clear();
        devices.reserve(num_devices);
        for (auto id : device_ids) {
            devices.emplace_back(id, *this);
        }
    }
}; //namecpace cl