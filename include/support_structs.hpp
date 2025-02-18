#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace cl {

    #define PARSE_ERR(msg_text, err)  \
    if (err != CL_SUCCESS) {  \
        std::string msg = "Error with " + std::string(msg_text) + ". Code: " + std::to_string(err); \
        throw std::runtime_error(msg); \
    } \

    namespace details {
        template <typename T, cl_int Name> struct param_traits {};

        /*  Platform  */
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_NAME> {
            enum { value = CL_PLATFORM_NAME };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };
        
        template<> struct param_traits<cl_platform_info, CL_PLATFORM_PROFILE> {
            enum { value = CL_PLATFORM_PROFILE };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_platform_info, CL_PLATFORM_VERSION> {
            enum { value = CL_PLATFORM_VERSION };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_platform_info, CL_PLATFORM_VENDOR> {
            enum { value = CL_PLATFORM_VENDOR };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Device  */
        template<> struct param_traits<cl_device_info, CL_DEVICE_TYPE> {
            enum { value = CL_DEVICE_TYPE };
            using type = cl_device_type;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };
        
        template<> struct param_traits<cl_device_info, CL_DEVICE_VENDOR_ID> {
            enum { value = CL_DEVICE_VENDOR_ID };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<const unsigned int*>(str.data());
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS> {
            enum { value = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_ITEM_SIZES> {
            enum { value = CL_DEVICE_MAX_WORK_ITEM_SIZES };
            using type = std::vector<size_t>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(size_t));
                for (size_t i = 0; i < size; i += sizeof(size_t)) {
                    size_t tmp = *reinterpret_cast<size_t*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_device_info, CL_DEVICE_MAX_WORK_GROUP_SIZE> {
            enum { value = CL_DEVICE_MAX_WORK_GROUP_SIZE };
            using type = size_t;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };
        
        /*  Context  */
        template<> struct param_traits<cl_context_info, CL_CONTEXT_REFERENCE_COUNT> {
            enum { value = CL_CONTEXT_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_NUM_DEVICES> {
            enum { value = CL_CONTEXT_NUM_DEVICES };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_DEVICES> {
            enum { value = CL_CONTEXT_DEVICES };
            using type = std::vector<cl_device_id>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_device_id));
                for (size_t i = 0; i < size; i += sizeof(cl_device_id)) {
                    cl_device_id tmp = *reinterpret_cast<cl_device_id*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_context_info, CL_CONTEXT_PROPERTIES> {
            enum { value = CL_CONTEXT_PROPERTIES };
            using type = std::vector<cl_context_properties>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_context_properties));
                for (size_t i = 0; i < size; i += sizeof(cl_context_properties)) {
                    cl_context_properties tmp = *reinterpret_cast<cl_context_properties*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };
        
        /*  Program  */
        template<> struct param_traits<cl_program_info, CL_PROGRAM_REFERENCE_COUNT> {
            enum { value = CL_PROGRAM_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_CONTEXT> {
            enum { value = CL_PROGRAM_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_NUM_DEVICES> {
            enum { value = CL_PROGRAM_NUM_DEVICES };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_DEVICES> {
            enum { value = CL_PROGRAM_DEVICES };
            using type = std::vector<cl_device_id>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(cl_device_id));
                for (size_t i = 0; i < size; i += sizeof(cl_device_id)) {
                    cl_device_id tmp = *reinterpret_cast<cl_device_id*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_SOURCE> {
            enum { value = CL_PROGRAM_SOURCE };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };
        
        template<> struct param_traits<cl_program_info, CL_PROGRAM_BINARY_SIZES> {
            enum { value = CL_PROGRAM_BINARY_SIZES };
            using type = std::vector<size_t>;

            static type CastToType(std::string &str) {
                size_t size = str.size();
                type v; v.reserve(size / sizeof(size_t));
                for (size_t i = 0; i < size; i += sizeof(size_t)) {
                    size_t tmp = *reinterpret_cast<size_t*>(&str[i]);
                    v.push_back(tmp);
                }
                return v;
            }
        };

        // template<> struct param_traits<cl_program_info, CL_PROGRAM_BINARIES> {
        //     enum { value = CL_PROGRAM_BINARIES };
        //     using type = unsigned char**;

        //     static type CastToType(std::string &str) {
        //         char* temp = new char[str.size() + 1U];
        //         std::strcpy(temp, str.c_str());
        //         return reinterpret_cast<type>(temp);
        //     }
        // };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_NUM_KERNELS> {
            enum { value = CL_PROGRAM_NUM_KERNELS };
            using type = size_t;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type*>(str.data());
            }
        };

        template<> struct param_traits<cl_program_info, CL_PROGRAM_KERNEL_NAMES> {
            enum { value = CL_PROGRAM_KERNEL_NAMES };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        /*  Command Queue  */
        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_CONTEXT> {
            enum { value = CL_QUEUE_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(str.data());
            }
        };

        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_DEVICE> {
            enum { value = CL_QUEUE_DEVICE };
            using type = cl_device_id;

            static type CastToType(std::string &str) {
                auto tmp = *reinterpret_cast<cl_device_id*>(str.data());
                return tmp;
            }
        };

        template<> struct param_traits<cl_command_queue_info, CL_QUEUE_REFERENCE_COUNT> {
            enum { value = CL_QUEUE_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        // template<> struct param_traits<cl_command_queue_info, CL_QUEUE_SIZE> {
        //     enum { value = CL_QUEUE_SIZE };
        //     using type = unsigned int;

        //     static type CastToType(std::string &str) {
        //         return static_cast<unsigned int>(std::stoi(str));
        //     }
        // };
    
        /*  Memory Object  */
        template<> struct param_traits<cl_mem_info, CL_MEM_TYPE> {
            enum { value = CL_MEM_TYPE };
            using type = cl_mem_object_type;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_FLAGS> {
            enum { value = CL_MEM_FLAGS };
            using type = cl_mem_flags;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_SIZE> {
            enum { value = CL_MEM_SIZE };
            using type = size_t;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_HOST_PTR> {
            enum { value = CL_MEM_HOST_PTR };
            using type = void*;

            static type CastToType(std::string &str) {
                return reinterpret_cast<type>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_MAP_COUNT> {
            enum { value = CL_MEM_MAP_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_REFERENCE_COUNT> {
            enum { value = CL_MEM_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_mem_info, CL_MEM_CONTEXT> {
            enum { value = CL_MEM_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(&str[0]);
            }
        };

        /*  Kernel  */
        template<> struct param_traits<cl_kernel_info, CL_KERNEL_FUNCTION_NAME> {
            enum { value = CL_KERNEL_FUNCTION_NAME };
            using type = std::string;

            static type CastToType(std::string &str) {
                return str;
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_NUM_ARGS> {
            enum { value = CL_KERNEL_NUM_ARGS };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_REFERENCE_COUNT> {
            enum { value = CL_KERNEL_REFERENCE_COUNT };
            using type = unsigned int;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<type *>(str.data());
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_CONTEXT> {
            enum { value = CL_KERNEL_CONTEXT };
            using type = cl_context;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_context*>(str.data());
            }
        };

        template<> struct param_traits<cl_kernel_info, CL_KERNEL_PROGRAM> {
            enum { value = CL_KERNEL_PROGRAM };
            using type = cl_program;

            static type CastToType(std::string &str) {
                return *reinterpret_cast<cl_program*>(str.data());
            }
        };

        /*  ReferenceHandler  */
        template <typename T> struct ReferenceHandler {};

        template <> struct ReferenceHandler<cl_platform_id> {
            static cl_int Retain(cl_platform_id platform) {
                (void) platform;
                return 0;
            }
            
            static cl_int Release(cl_platform_id platform) { 
                (void) platform;
                return 0;
            }
        };

        template <> struct ReferenceHandler<cl_device_id> {
            static cl_int Retain(cl_device_id device) { 
                return ::clRetainDevice(device);
            }

            static cl_int Release(cl_device_id device) { 
                return ::clReleaseDevice(device);
            }
        };

        template <> struct ReferenceHandler<cl_context> {
            static cl_int Retain(cl_context context) { 
                return ::clRetainContext(context); 
            }
            static cl_int Release(cl_context context) { 
                return ::clReleaseContext(context); 
            }
        };
        
        template <> struct ReferenceHandler<cl_program> {
            static cl_int Retain(cl_program program) {          
                return ::clRetainProgram(program); 
            }
            static cl_int Release(cl_program program) {
                return ::clReleaseProgram(program); 
            }
        };

        template <> struct ReferenceHandler<cl_command_queue> {
            static cl_int Retain(cl_command_queue command_queue) { 
                return ::clRetainCommandQueue(command_queue); 
            }
            static cl_int Release(cl_command_queue command_queue) { 
                return ::clReleaseCommandQueue(command_queue);
            }
        };

        template <> struct ReferenceHandler<cl_mem> {
            static cl_int Retain(cl_mem memory) { 
                return ::clRetainMemObject(memory); 
            }
            static cl_int Release(cl_mem memory) { 
                return ::clReleaseMemObject(memory); 
            }
        };

        template <> struct ReferenceHandler<cl_kernel> {
            static cl_int Retain(cl_kernel kernal) { 
                return ::clRetainKernel(kernal); 
            }
            static cl_int Release(cl_kernel kernal) { 
                return ::clReleaseKernel(kernal); 
            }
        };

        /*  Info Manager  */
        template <typename T> struct InfoManager {};

        template <> struct InfoManager<cl_platform_id> {
            template <cl_platform_info param_name>
            static typename param_traits<cl_platform_info, param_name>::type
            GetInfo(cl_platform_id platform) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetPlatformInfo(platform, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting platform info", err)

                std::string value(info_size, '\0');
                err = clGetPlatformInfo(platform, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting platform info", err)
                
                return param_traits<cl_platform_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_device_id> {
            template <cl_device_info param_name>
            static typename details::param_traits<cl_device_info, param_name>::type
            GetInfo(cl_device_id device) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetDeviceInfo(device, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting device info", err)

                std::string value(info_size, '\0');
                err = clGetDeviceInfo(device, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting device info", err)
                
                return param_traits<cl_device_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_context> {
            template <cl_context_info param_name>
            static typename details::param_traits<cl_context_info, param_name>::type
            GetInfo(cl_context context) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetContextInfo(context, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting context info", err)

                std::string value(info_size, '\0');
                err = clGetContextInfo(context, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting context info", err)
                
                return param_traits<cl_context_info, param_name>::CastToType(value);
            }
        };
        
        template <> struct InfoManager<cl_program> {
            template <cl_program_info param_name>
            static typename details::param_traits<cl_program_info, param_name>::type
            GetInfo(cl_program program) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetProgramInfo(program, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting program info", err)

                std::string value(info_size, '\0');
                err = clGetProgramInfo(program, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting program info", err)
                
                return param_traits<cl_program_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_command_queue> {
            template <cl_command_queue_info param_name>
            static typename details::param_traits<cl_command_queue_info, param_name>::type
            GetInfo(cl_command_queue command_queue) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetCommandQueueInfo(command_queue, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting command_queue info", err)

                std::string value(info_size, '\0');
                err = clGetCommandQueueInfo(command_queue, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting command_queue info", err)
                
                return param_traits<cl_command_queue_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_mem> {
            template <cl_mem_info param_name>
            static typename details::param_traits<cl_mem_info, param_name>::type
            GetInfo(cl_mem memory) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetMemObjectInfo(memory, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting memory info", err)

                std::string value(info_size, '\0');
                err = clGetMemObjectInfo(memory, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting memory info", err)
                
                return param_traits<cl_mem_info, param_name>::CastToType(value);
            }
        };

        template <> struct InfoManager<cl_kernel> {
            template <cl_kernel_info param_name>
            static typename details::param_traits<cl_kernel_info, param_name>::type
            GetInfo(cl_kernel kernel) {
                cl_int err = 0;
                size_t info_size = 0;
                err = clGetKernelInfo(kernel, param_name, 0, NULL, &info_size);
                PARSE_ERR("getting kernel info", err)

                std::string value(info_size, '\0');
                err = clGetKernelInfo(kernel, param_name, info_size, value.data(), NULL);
                PARSE_ERR("getting kernel info", err)
                
                return param_traits<cl_kernel_info, param_name>::CastToType(value);
            }
        };
    } // namespace details
} // namespace cl