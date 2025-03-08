#pragma once

#include <CL/cl.h>
#include <stdexcept>
#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <format>

#include "CL/support_structs.hpp"

namespace cl {

    class clException : public std::exception {
    public:
        explicit clException(const std::string &message) : message_(std::move(message)) {}

        const char *what() const noexcept override {
            return message_.data();
        }
    private:
        std::string message_;
    }; // class clException       

    #define clRUN(func, ...) \
            clRun(__LINE__, __FILE__, #func, func __VA_OPT__(,) __VA_ARGS__)

    void clCheckError(cl_int error, int line, std::string_view file_name, std::string_view func_name) {
        if (error != CL_SUCCESS) {
            std::string mes = std::format("OpenCL lib {} error in '{}' file on line '{}', when calling the function '{}'.\n", 
                error, file_name, line, func_name);

            throw clException(mes);
        }
    }

    template<typename Func, typename... Args>
    auto clRun(int line, std::string_view file_name, std::string_view func_name, Func&& func, Args&&... args)
    -> std::invoke_result_t<Func, Args..., cl_int*> {
        cl_int error = CL_SUCCESS;
        auto result = std::invoke(func, std::forward<Args>(args)..., &error);
        clCheckError(error, line, file_name, func_name);
        return result;
    }

    template<typename Func, typename... Args>
    auto clRun(int line, std::string_view file_name, std::string_view func_name, Func&& func, Args&&... args)
    -> std::invoke_result_t<Func, Args...> {
        auto error = std::invoke(func, std::forward<Args>(args)...);
        clCheckError(error, line, file_name, func_name);
        return error;
    }

    namespace details {

        /*  Wrapper  */
        template <typename cl_type> class Wrapper {
        public:
            Wrapper(cl_type obj = NULL) : obj_(obj) { Retain(); }
            
            Wrapper(const Wrapper &other) : obj_(other.obj_) {
                Retain();
            }

            Wrapper &operator=(const Wrapper &other) {
                if (this != &other) {
                    Release();
                    obj_ = other.obj_;
                    Retain();
                }
                
                return *this;
            }

            Wrapper(Wrapper &&other) noexcept : obj_(other.obj_) {
                other.obj_ = NULL;
            }

            Wrapper &operator=(Wrapper &&other) noexcept {
                if (this != &other) {
                    Release();
                    obj_ = other.obj_;
                    other.obj_ = NULL;
                }
                
                return *this;
            }
            
            ~Wrapper() { if (obj_) Release(); }
            
            const cl_type& operator()() const { 
                return obj_;
            }

            cl_type& operator()() {
                return obj_;
            }
            
            cl_type Get() const {
                return obj_;
            }

            cl_int Retain() const {
                return ReferenceHandler<cl_type>::Retain(obj_);
            }

            cl_int Release() const {
                return ReferenceHandler<cl_type>::Release(obj_);
            }
        protected:
            cl_type obj_;
        }; // class Wrapper
    } // namespace details    
}; // namespace cl