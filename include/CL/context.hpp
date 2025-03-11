#pragma once

#include "CL/cl.hpp"
#include "CL/support_structs.hpp"
#include "CL/platform.hpp"
#include "CL/device.hpp"

namespace cl {
    class Context final : public details::Wrapper<cl_context> {
    public:
        Context() : details::Wrapper<cl_context>(NULL), device_() {}
        
        Context(const Device &device) : device_(device) {
            obj_ = clRUN(clCreateContext, nullptr, 1, &device_(), nullptr, nullptr);
        }

        Context(const Context &other) : details::Wrapper<cl_context>(other), device_(other.device_) {}

        Context &operator=(const Context &other) {
            details::Wrapper<cl_context>::operator=(other);
            device_ = other.device_;
            return *this;
        }

        Context(Context &&other) noexcept : details::Wrapper<cl_context>(std::move(other)), device_(std::move(other.device_)) {}

        Context &operator=(Context &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_context>::operator=(std::move(other));
                std::swap(device_, other.device_);
            }

            return *this;
        }

        Device GetDevice() const {
            return device_;
        }

        template <cl_context_info param_name>
        typename details::param_traits<cl_context_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_context>::template GetInfo<param_name>(obj_);
        }

    private:
        Device device_;
    }; // class Context
} // namespace cl