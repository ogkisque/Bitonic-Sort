#pragma once

#include "CL/cl.hpp"
#include "CL/context.hpp"
#include "CL/support_structs.hpp"
#include "utility.hpp"

namespace cl {
    class Program final : public details::Wrapper<cl_program> {
    public:
        Program() : details::Wrapper<cl_program>(NULL), context_() {}
        
        Program(const Context &context, std::string &file_name) : context_(context) {
            std::string program_text = ReadFileToString(file_name);
            const char *text = program_text.data();
            size_t size = program_text.size();

            obj_ = clRUN(clCreateProgramWithSource, context_.Get(), 1, &text, &size);
            clRUN(clBuildProgram, obj_, 1, &context_.GetDevice()(), nullptr, nullptr, nullptr);
        }

        Program(const Context &context, const char **program_text) : context_(context) {
            obj_ = clRUN(clCreateProgramWithSource, context_.Get(), 1, program_text, nullptr);
            clRUN(clBuildProgram, obj_, 1, &context_.GetDevice()(), nullptr, nullptr, nullptr);
        }
        
        Program(const Program &other) : details::Wrapper<cl_program>(other), context_(other.context_) {}

        Program& operator=(const Program &other) {
            details::Wrapper<cl_program>::operator=(other);
            context_ = other.context_;
            
            return *this;
        }

        Program(Program &&other) noexcept : details::Wrapper<cl_program>(std::move(other)), context_(std::move(other.context_)) {}

        Program &operator=(Program &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_program>::operator=(std::move(other));
                std::swap(context_, other.context_);
            }

            return *this;
        }

        Context GetContext() const {
            return context_;
        }

        template <cl_program_info param_name>
        typename details::param_traits<cl_program_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_program>::template GetInfo<param_name>(obj_);
        }

    private:
        std::string ReadFileToString(std::string &file_name) {
            std::ifstream stream(file_name);
            if (!stream.is_open())
                throw std::runtime_error("File did not open");

            return std::string(std::istreambuf_iterator<char>(stream), 
                                std::istreambuf_iterator<char>());
        }

        Context context_;
    }; // class Program
}; // namespace cl 