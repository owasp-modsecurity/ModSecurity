/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#ifdef __cplusplus
#include <stddef.h>
#include <cstddef>
#include <cassert>
#include <cstdio>
#include <iostream>
#endif

#ifndef SRC_UTILS_MODSEC_STACK_ALLOCATOR_H_
#define SRC_UTILS_MODSEC_STACK_ALLOCATOR_H_


#ifdef __cplusplus
namespace modsecurity {


template <std::size_t N, std::size_t alignment>
class arena
{
    alignas(alignment) char buf_[N];
    char* ptr_;

public:
    ~arena() { ptr_ = nullptr; }
    arena() noexcept : ptr_(buf_) {}
    arena(const arena&) = delete;
    arena& operator=(const arena&) = delete;

    template <std::size_t ReqAlign> char* allocate(std::size_t n);
    void deallocate(char* p, std::size_t n) noexcept;

    static constexpr std::size_t size() noexcept { return N; }
    std::size_t used() const noexcept { return static_cast<std::size_t>(ptr_ - buf_); }
    void reset() noexcept { ptr_ = buf_; }

private:
    static
        std::size_t
        align_up(std::size_t n) noexcept
    {
        return (n + (alignment - 1)) & ~(alignment - 1);
    }

    bool
        pointer_in_buffer(char* p) noexcept
    {
        return buf_ <= p && p <= buf_ + N;
    }
};

template <std::size_t N, std::size_t alignment>
template <std::size_t ReqAlign>
char*
arena<N, alignment>::allocate(std::size_t n)
{
    #if 1
    static_assert(ReqAlign <= alignment, "alignment is too small for this arena");
    assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
    auto const aligned_n = align_up(n);
    if (static_cast<decltype(aligned_n)>(buf_ + N - ptr_) >= aligned_n)
    {
        char* r = ptr_;
        ptr_ += aligned_n;
        return r;
    }

    //static_assert(alignment <= alignof(std::max_align_t), "you've chosen an "
    //    "alignment that is larger than alignof(std::max_align_t), and "
    //    "cannot be guaranteed by normal operator new");
    return static_cast<char*>(::operator new(n));
    #else
    return ptr_;
    #endif
}

template <std::size_t N, std::size_t alignment>
void
arena<N, alignment>::deallocate(char* p, std::size_t n) noexcept
{
    #if 1
    assert(pointer_in_buffer(ptr_) && "short_alloc has outlived arena");
    if (pointer_in_buffer(p))
    {
        n = align_up(n);
        if (p + n == ptr_)
            ptr_ = p;
    }
    else
        ::operator delete(p);
    #endif
}

template <class T, std::size_t N, std::size_t Align>
class short_alloc
{
public:
    using value_type = T;
    static auto constexpr alignment = Align;
    static auto constexpr size = N;
    using arena_type = arena<size, alignment>;

private:
    arena_type& a_;

public:
    short_alloc(const short_alloc&) = default;
    short_alloc& operator=(const short_alloc&) = delete;

    short_alloc(arena_type& a) noexcept : a_(a)
    {
        static_assert(size % alignment == 0,
            "size N needs to be a multiple of alignment Align");
    }
    template <class U>
    short_alloc(const short_alloc<U, N, alignment>& a) noexcept
        : a_(a.a_) {}

    template <class _Up> struct rebind { using other = short_alloc<_Up, N, alignment>; };

    T* allocate(std::size_t n)
    {
        return reinterpret_cast<T*>(a_.template allocate<alignof(T)>(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) noexcept
    {
        a_.deallocate(reinterpret_cast<char*>(p), n * sizeof(T));
    }

    template <class T1, std::size_t N1, std::size_t A1,
        class U, std::size_t M, std::size_t A2>
        friend
        bool
        operator==(const short_alloc<T1, N1, A1>& x, const short_alloc<U, M, A2>& y) noexcept;

    template <class U, std::size_t M, std::size_t A> friend class short_alloc;
};

template <class T, std::size_t N, std::size_t A1, class U, std::size_t M, std::size_t A2>
inline
bool
operator==(const short_alloc<T, N, A1>& x, const short_alloc<U, M, A2>& y) noexcept
{
    return N == M && A1 == A2 && &x.a_ == &y.a_;
}

template <class T, std::size_t N, std::size_t A1, class U, std::size_t M, std::size_t A2>
inline
bool
operator!=(const short_alloc<T, N, A1>& x, const short_alloc<U, M, A2>& y) noexcept
{
    return !(x == y);
}

}  // namespace modsecurity

#endif

#endif  // SRC_UTILS_MODSEC_STACK_ALLOCATOR_H_
