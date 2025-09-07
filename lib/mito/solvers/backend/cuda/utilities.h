// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // a utility class to manage host memory
    template <std::floating_point T>
    class HostArray {
      public:
        // type alias for real numbers
        using real_type = T;
        // available host memory types
        enum HostMemoryType { PAGEABLE, PINNED };

      public:
        // default constructor
        HostArray() : _data(nullptr), _size(0), _memory_type(HostMemoryType::PINNED) {}

        // parameterized constructor
        HostArray(std::size_t size, HostMemoryType memory_type = HostMemoryType::PINNED) :
            _data(nullptr),
            _size(size),
            _memory_type(memory_type)
        {
            allocate(size);
        }

        // allocate on construction
        explicit HostArray(std::size_t size) : HostArray() { allocate(size); }

        // destructor
        ~HostArray() { _free_memory(); }

        // delete copy constructor
        HostArray(const HostArray &) = delete;

        // delete copy assignment operator
        HostArray & operator=(const HostArray &) = delete;

        // enable move semantics
        HostArray(HostArray && other) noexcept :
            _data(other._data),
            _size(other._size),
            _memory_type(other._memory_type)
        {
            // reset the state of the moved-from object
            other._data = nullptr;
            other._size = 0;
            other._memory_type = HostMemoryType::PINNED;
        }

        HostArray & operator=(HostArray && other) noexcept
        {
            if (this != &other) {
                // free existing memory and acquire new memory
                _free_memory();
                _data = other._data;
                _size = other._size;
                _memory_type = other._memory_type;
                // reset the state of the moved-from object
                other._data = nullptr;
                other._size = 0;
                other._memory_type = HostMemoryType::PINNED;
            }
            return *this;
        }

        // allocate or reallocate memory for the array
        void allocate(std::size_t size)
        {
            if (_size == size)
                return;

            // free old memory if any
            // NOTE: This could be expensive if called frequently. We are going ahead with this
            // for now for simplicity. But, as a first step towards improvement, we can come up with
            // a way to reuse existing memory if the new allocation size is smaller than the current
            // size.
            _free_memory();

            _size = size;
            if (_size == 0)
                return;

            // try to allocate pinned memory first for faster transfers
            cudaError_t pinned_alloc_error =
                cudaMallocHost((void **) &_data, _size * sizeof(real_type));
            if (pinned_alloc_error == cudaSuccess) {
                _memory_type = HostMemoryType::PINNED;
            } else {
                // fallback to pageable memory
                try {
                    std::cerr
                        << "[HostArray] Warning: Failed to allocate pinned memory. Falling back "
                           "to pageable memory\n";
                    _data = new real_type[_size];
                    _memory_type = HostMemoryType::PAGEABLE;
                } catch (const std::bad_alloc & e) {
                    throw std::runtime_error(
                        "Failed to allocate host memory: " + std::string(e.what()));
                }
            }

            // all done
            return;
        }

        // zero out the array
        void zero()
        {
            if (_data) {
                std::fill(_data, _data + _size, static_cast<real_type>(0.0));
            }
        }

        // data accessor
        real_type * data() { return _data; }

        // const data accessor
        const real_type * data() const { return _data; }

        // size accessor
        std::size_t size() const { return _size; }

        // accessor for memory type
        HostMemoryType memory_type() const { return _memory_type; }

        // element access
        real_type & operator[](std::size_t idx) { return _data[idx]; }

        // const element access
        const real_type & operator[](std::size_t idx) const { return _data[idx]; }

      private:
        // free the memory in the array
        void _free_memory()
        {
            if (!_data)
                return;

            // free the memory
            if (_memory_type == HostMemoryType::PINNED) {
                cudaError_t error = cudaFreeHost(_data);
                if (error != cudaSuccess) {
                    std::cerr << "[HostArray] Error: Failed to free pinned memory: "
                              << cudaGetErrorString(error) << "\n";
                }
            } else {
                delete[] _data;
            }

            // reset the state
            _data = nullptr;
            _size = 0;
            _memory_type = HostMemoryType::PINNED;

            // all done
            return;
        }

      private:
        // pointer to the data
        real_type * _data;
        // size of the array
        std::size_t _size;
        // type of memory
        HostMemoryType _memory_type;
    };

    // a utility class to manage device memory
    template <std::floating_point T>
    class DeviceArray {
      public:
        using real_type = T;

      public:
        // default constructor
        DeviceArray() : _data(nullptr), _size(0) {}

        // allocate on construction
        explicit DeviceArray(std::size_t size) : DeviceArray() { allocate(size); }

        // destructor
        ~DeviceArray() { _free_memory(); }

        // delete copy constructor
        DeviceArray(const DeviceArray &) = delete;

        // delete copy assignment operator
        DeviceArray & operator=(const DeviceArray &) = delete;

        // enable move semantics
        DeviceArray(DeviceArray && other) noexcept : _data(other._data), _size(other._size)
        {
            // reset the state of the moved-from object
            other._data = nullptr;
            other._size = 0;
        }

        DeviceArray & operator=(DeviceArray && other) noexcept
        {
            if (this != &other) {
                // free existing memory and acquire new memory
                _free_memory();
                _data = other._data;
                _size = other._size;
                // reset the state of the moved-from object
                other._data = nullptr;
                other._size = 0;
            }
            return *this;
        }

        // allocate or reallocate memory for the array
        void allocate(std::size_t size)
        {
            if (_size == size)
                return;

            // free old memory if any
            _free_memory();

            _size = size;
            if (_size == 0)
                return;

            // allocate device memory
            cudaError_t error = cudaMalloc((void **) &_data, size * sizeof(real_type));
            if (error != cudaSuccess) {
                throw std::runtime_error(
                    "[DeviceArray] Error: Failed to allocate device memory: "
                    + std::string(cudaGetErrorString(error)));
            }

            // all done
            return;
        }

        // zero out the device array
        void zero()
        {
            if (_data) {
                cudaError_t error = cudaMemset(_data, 0, _size * sizeof(real_type));
                if (error != cudaSuccess) {
                    throw std::runtime_error(
                        "[DeviceArray] Error: Failed to zero out device memory: "
                        + std::string(cudaGetErrorString(error)));
                }
            }
        }

        // data accessor
        real_type * data() { return _data; }

        // const data accessor
        const real_type * data() const { return _data; }

        // size accessor
        std::size_t size() const { return _size; }

      private:
        // free the memory in the array
        void _free_memory()
        {
            if (!_data)
                return;

            // free the memory
            cudaError_t error = cudaFree(_data);
            if (error != cudaSuccess) {
                std::cerr << "[DeviceArray] Error: Failed to free device memory: "
                          << cudaGetErrorString(error) << "\n";
            }

            // reset the state
            _data = nullptr;
            _size = 0;

            // all done
            return;
        }

      private:
        // pointer to the data
        real_type * _data;
        // size of the array
        std::size_t _size;
    };
}


// end of file
