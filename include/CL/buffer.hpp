#pragma once
#include "CL/cl.hpp"
#include "queue.hpp"

namespace cl {
    class Buffer final : public details::Wrapper<cl_mem> {
    public:
        Buffer() : details::Wrapper<cl_mem>(NULL), queue_() {}

        Buffer(const CommandQueue &queue, cl_mem_flags flag, size_t size) : size_(size), queue_(queue) {
            obj_ = clRUN(clCreateBuffer, queue_.GetContext().Get(), flag, size, nullptr);
        }

        Buffer(const Buffer &other) : details::Wrapper<cl_mem>(other), size_(other.size_), queue_(other.queue_) {}

        Buffer &operator=(const Buffer &other) {
            details::Wrapper<cl_mem>::operator=(other);
            size_ = other.size_;
            queue_ = other.queue_;
            
            return *this;
        }

        Buffer(Buffer &&other) noexcept : details::Wrapper<cl_mem>(std::move(other)) {
            std::swap(size_, other.size_);
            std::swap(queue_, other.queue_);
        }

        Buffer &operator=(Buffer &&other) noexcept {
            if (this != &other) {
                details::Wrapper<cl_mem>::operator=(std::move(other));
                std::swap(size_, other.size_);
                std::swap(queue_, other.queue_);
            }

            return *this;
        }

        template <typename IterT>
        Buffer(CommandQueue &queue, IterT start, IterT end, cl_mem_flags flag) :
                queue_(queue) {
            using T = typename std::iterator_traits<IterT>::value_type;
            static_assert(!std::is_same_v<bool, T>, "Incorrect value type of iterator");
            std::vector<T> data{start, end};
            size_ = data.size() * sizeof(T);
            obj_ = clRUN(clCreateBuffer, queue_.GetContext().Get(), flag, size_, nullptr);
            clRUN(clEnqueueWriteBuffer, queue_.Get(), obj_, CL_TRUE, 0, size_, data.data(), 0, nullptr, nullptr);
        }

        size_t GetSize() const {
            return size_;
        }

        template <typename IterT>
        void Read(IterT start_it) {
            using T = typename std::iterator_traits<IterT>::value_type;
            static_assert(!std::is_same_v<bool, T>, "Incorrect value type of iterator");
            size_t num_elems = size_ / sizeof(T);
            std::vector<T> data{num_elems};
            clRUN(clEnqueueReadBuffer, queue_.Get(), obj_, CL_TRUE, 0, size_, data.data(), 0, nullptr, nullptr);

            IterT it = start_it;
            for (size_t i = 0; i < num_elems; ++i, ++it)
                *it = data[i];
        }

        template <typename IterT>
        void Write(IterT start, IterT end) {
            using T = typename std::iterator_traits<IterT>::value_type;
            static_assert(!std::is_same_v<bool, T>, "Incorrect value type of iterator");

            std::vector<T> data{start, end};
            if (data.size() * sizeof(T) != size_) {
                throw std::runtime_error("Data size does not match with buffer");
            }

            clRUN(clEnqueueWriteBuffer, queue_.Get(), obj_, CL_TRUE, 0, size_, data, 0, nullptr, nullptr);
        }

        template <cl_mem_info param_name>
        typename details::param_traits<cl_mem_info, param_name>::type 
        GetInfo() {
            return details::InfoManager<cl_mem>::template GetInfo<param_name>(obj_);
        }

    private:
        size_t size_ = 0;
        CommandQueue queue_;
    }; // class Buffer
}; // namespace cl