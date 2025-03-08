#pragma once
#include "CL/cl.hpp"
#include "CL/program.hpp"
#include "CL/queue.hpp"
#include "CL/buffer.hpp"

namespace cl {
class Kernel final : public details::Wrapper<cl_kernel> {
    public:
        Kernel() : details::Wrapper<cl_kernel>(NULL), program_(), queue_() {}

        Kernel(const Program &program, const CommandQueue &queue, const std::string &func_name) :
                program_(program), queue_(queue) {
            obj_ = clRUN(clCreateKernel, program_.Get(), func_name.data());
        }

        Kernel(const Kernel &other) : details::Wrapper<cl_kernel>(other), program_(other.program_), queue_(other.queue_) {}

        Kernel &operator=(const Kernel &other) {
            queue_ = other.queue_;
            program_ = other.program_;
            details::Wrapper<cl_kernel>::operator=(other);

            return *this;
        }

        Kernel(Kernel &&other) noexcept : details::Wrapper<cl_kernel>(std::move(other)) {
            std::swap(program_, other.program_);
            std::swap(queue_, other.queue_);
        }

        Kernel &operator=(Kernel &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_kernel>::operator=(std::move(other));
                std::swap(program_, other.program_);
                std::swap(queue_, other.queue_);
            }

            return *this;
        }

        template <typename T>
        void SetArg(cl_uint arg_num, const T &arg_value) {
            clRUN(clSetKernelArg, obj_, arg_num, sizeof(arg_value), &arg_value);
        }

        void SetArg(cl_uint arg_num, const Buffer &buf) {
            cl_mem mem = buf.Get(); buf.Retain();
            clRUN(clSetKernelArg, obj_, arg_num, sizeof(cl_mem), &mem);
        }

        template <typename IterT>
        void SetArgs(IterT start_it, IterT end_it, cl_uint first_arg_num) {
            for (IterT it = start_it; it != end_it; it++, first_arg_num++) {
                clRUN(clSetKernelArg, obj_, first_arg_num, sizeof(void*), it);
            }
        }

        void Run(size_t global_work_size, size_t local_work_size) {
            clRUN(clEnqueueNDRangeKernel, queue_.Get(), obj_, 1, nullptr, &global_work_size, &local_work_size, 0, nullptr, nullptr);
            queue_.Finish();
        }

        template <cl_kernel_info param_name>
        typename details::param_traits<cl_kernel_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_kernel>::template GetInfo<param_name>(obj_);
        }

    private:
        Program program_;
        CommandQueue queue_;
    }; // class Kernel
}; // namespace cl