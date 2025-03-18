#pragma once

#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>
#include <stdexcept>
#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

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
            clRun(__LINE__, __FILE__, #func, func, ##__VA_ARGS__)

    void clCheckError(cl_int error, int line, std::string_view file_name, std::string_view func_name) {
        if (error != CL_SUCCESS) {
            std::string mes = "OpenCL lib '"; mes += std::to_string(error); mes += "' error in '";
            mes += file_name; mes += "' file on line '"; mes += std::to_string(line);
            mes += "', when calling the function '"; mes += func_name; mes += "'.\n";

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
}; // namespace cl