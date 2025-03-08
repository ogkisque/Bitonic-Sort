#pragma once
#include "CL/cl.hpp"
#include "CL/context.hpp"

namespace cl {
    class CommandQueue final : public details::Wrapper<cl_command_queue> {
    public:
        CommandQueue() : details::Wrapper<cl_command_queue>(NULL), context_() {}

        CommandQueue(const Context &context) : context_(context) {
            obj_ = clRUN(clCreateCommandQueue, context_.Get(), context_.GetDevice().Get(), 0);
        }
        
        CommandQueue(const CommandQueue &other) : details::Wrapper<cl_command_queue>(other), context_(other.context_) {}

        CommandQueue &operator=(const CommandQueue &other) {
            details::Wrapper<cl_command_queue>::operator=(other);
            context_ = other.context_;

            return *this;
        }

        CommandQueue(CommandQueue &&other) noexcept : details::Wrapper<cl_command_queue>(std::move(other)) {
            std::swap(context_, other.context_);
        }

        CommandQueue &operator=(CommandQueue &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_command_queue>::operator=(std::move(other));
                std::swap(context_, other.context_);
            }

            return *this;
        }

        void PutBarrier() {
            clRUN(clEnqueueBarrier, obj_);
        }

        void Finish() {
            clRUN(clFinish, obj_);
        }

        Context GetContext() const {
            return context_;
        }

        template <cl_command_queue_info param_name>
        typename details::param_traits<cl_command_queue_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_command_queue>::template GetInfo<param_name>(obj_);
        }

    private:
        Context context_;
    };
}; // namespace cl