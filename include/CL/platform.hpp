#pragma once

#include "CL/cl.hpp"
#include "CL/support_structs.hpp"

namespace cl {
    class Device;
    class Platform final : public details::Wrapper<cl_platform_id> {
    public:
        Platform() {
            clRUN(clGetPlatformIDs, 1, &obj_, nullptr);
        }
    
        explicit Platform(cl_platform_id id) : details::Wrapper<cl_platform_id>(id) {}
    
        static void GetPlatforms(std::vector<Platform> &platforms) {
            cl_uint num_platforms;
            clRUN(clGetPlatformIDs, 0, nullptr, &num_platforms);
    
            if (num_platforms == 0) {
                throw std::runtime_error("platforms were not found");
            }
    
            std::vector<cl_platform_id> platform_ids(num_platforms);
            clRUN(clGetPlatformIDs, num_platforms, platform_ids.data(), nullptr);
    
            platforms.clear();
            platforms.reserve(num_platforms);
            for (auto id : platform_ids) {
                platforms.emplace_back(id);
            }
        }
    
        void GetDevices(cl_device_type device_type, std::vector<Device> &devices) const;
    
        template <cl_platform_info param_name>
        typename details::param_traits<cl_platform_info, param_name>::type 
        GetInfo() const {
            return details::InfoManager<cl_platform_id>::template GetInfo<param_name>(obj_);
        }
    }; // class Platform
} // namespace cl
