#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <iostream>

namespace rice
{

    template <typename T>
    class vector
    {
    public:
        // Type definitions
        typedef T value_type;
        typedef std::allocator<T> allocator_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ptrdiff_t difference_type;
        typedef unsigned int size_type;

        // Constructors
        vector();
        vector(size_type n, const T &val = T{});
        vector(vector<T>::iterator first, vector<T>::iterator last);
        vector(std::initializer_list<T>);
        vector(const vector<T> &);
        vector(vector<T> &&);
        ~vector();

        // Copy initializers
        vector<T> &operator=(const vector<T> &);
        vector<T> &operator=(vector<T> &&);
        vector<T> &operator=(std::initializer_list<T>);

        void assign(size_type, const T &value);
        void assign(vector<T>::iterator, vector<T>::iterator);
        void assign(std::initializer_list<T>);

        // Iterators
        iterator begin();
        const_iterator cbegin() const;
        iterator end();
        const_iterator cend() const;
        reverse_iterator rbegin();
        const_reverse_iterator crbegin() const;
        reverse_iterator rend();
        const_reverse_iterator crend() const;

        bool empty() const;
        size_type size() const;
        size_type max_size() const;
        size_type capacity() const;
        size_type getReallocateCnt() const;
        void resize(size_type);
        void resize(size_type, const T &);
        void reserve(size_type);
        void shrink_to_fit();

        // Operator overloading
        reference operator[](size_type);
        const_reference operator[](size_type) const;
        reference at(size_type);
        const_reference at(size_type) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        T *data();
        const T *data() const;

        allocator_type getAllocator() const;

        template <class... Args>
        void emplace_back(Args &&... args);
        void push_back(const T &);
        void push_back(T &&);
        void pop_back();

        template <class... Args>
        iterator emplace(const_iterator, Args &&...);
        iterator insert(const_iterator, const T &el = T());
        iterator insert(const_iterator, T &&el = T());
        iterator insert(const_iterator, size_type, const T &);
        template <class InputIt>
        iterator insert(const_iterator, InputIt, InputIt);
        iterator insert(const_iterator, std::initializer_list<T>);
        iterator erase(const_iterator);
        iterator erase(const_iterator, const_iterator);
        void swap(vector<T> &);
        void clear();

        // Operator overloading
        bool operator==(const vector<T> &) const;
        bool operator!=(const vector<T> &) const;

    private:
        size_type vec_sz = 0, cap_sz = 6, reallocateCnt = 0;
        T *buffer;
        allocator_type allocator;

        // Helper functions
        inline void reallocate();
        inline void grow_allocate(size_type);
        inline void grow_reallocate(size_type);
    };

    template <typename T>
    vector<T>::vector()
    {
        try
        {
            buffer = allocator.allocate(cap_sz);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Allocation Error: " << e.what() << '\n';
        }
    }

    template <typename T>
    vector<T>::vector(vector<T>::size_type n, const T &value)
    {
        grow_allocate(n);
        std::uninitialized_fill(buffer, buffer + n, value);
        vec_sz = n;
    }

    template <typename T>
    vector<T>::vector(vector<T>::iterator first, vector<T>::iterator last)
    {
        size_type cnt = last - first;
        grow_allocate(cnt);
        std::uninitialized_fill(buffer, buffer + cnt, *first);
        vec_sz = cnt;
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list)
    {
        grow_allocate(list.size());
        for (auto &item : list)
            buffer[vec_sz++] = item;
    }

    template <typename T>
    vector<T>::vector(const vector<T> &other)
    {
        cap_sz = other.cap_sz, vec_sz = other.vec_sz;
        buffer = allocator.allocate(cap_sz);
        std::uninitialized_copy(other.buffer, other.buffer + other.vec_sz, buffer);
        /* for (size_type i = 0; i < other.vec_sz; ++i)
            buffer[i] = other.buffer[i]; */
    }

    template <typename T>
    vector<T>::vector(vector<T> &&other)
    {
        cap_sz = other.cap_sz, vec_sz = other.vec_sz;
        buffer = allocator.allocate(cap_sz);
        std::uninitialized_copy(other.buffer, other.buffer + other.vec_sz, buffer);
        /*         for (size_type i = 0; i < other.vec_sz; ++i)
            buffer[i] = std::move(other.buffer[i]); */
    }

    template <typename T>
    vector<T>::~vector()
    {
        allocator.deallocate(buffer, cap_sz);
    }

    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &other)
    {
        if (cap_sz < other.vec_sz)
        {
            cap_sz = other.vec_sz << 2;
            reallocate();
        }
        std::uninitialized_copy(other.buffer, other.buffer + other.vec_sz, buffer);
        /*         for (size_type i = 0; i < other.vec_sz; ++i)
            buffer[i] = other.buffer[i]; */
        vec_sz = other.vec_sz;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(vector<T> &&other)
    {
        std::cout << "REALLOCATED" << std::endl;
        if (cap_sz < other.cap_sz)
        {
            cap_sz = other.cap_sz << 2;
            reallocate();
        }
        std::uninitialized_copy(other.buffer, other.buffer + other.vec_sz, buffer);
        /*         for (size_type i = 0; i < other.vec_sz; ++i)
            buffer[i] = std::move(other.buffer[i]); */
        vec_sz = other.vec_sz;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(std::initializer_list<T> list)
    {
        if (cap_sz < list.size())
            grow_reallocate(list.size());
        vec_sz = 0;
        /* for (auto &item : list)
            buffer[vec_sz++] = item; */
        std::uninitialized_copy(list.begin(), list.end(), buffer);
    }

    template <typename T>
    void vector<T>::assign(vector<T>::size_type count, const T &value)
    {
        if (count > cap_sz)
            grow_reallocate(count);
        for (size_type i = 0; i < count; ++i)
            buffer[i] = value;
        vec_sz = count;
    }

    template <typename T>
    void vector<T>::assign(vector<T>::iterator first, vector<T>::iterator last)
    {
        size_type count = last - first;
        if (count > cap_sz)
            grow_reallocate(count);
        for (size_type i = 0; i < count; ++i, ++first)
            buffer[i] = *first;
        vec_sz = count;
    }

    template <typename T>
    void vector<T>::assign(std::initializer_list<T> list)
    {
        size_type i, count = list.size();
        if (count > cap_sz)
            grow_reallocate(count);
        i = 0;
        for (auto &item : list)
            buffer[i++] = item;
        vec_sz = list.size(); //change here?
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::begin()
    {
        return buffer;
    }

    template <typename T>
    typename vector<T>::const_iterator vector<T>::cbegin() const
    {
        return buffer;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::end()
    {
        return buffer + vec_sz;
    }

    template <typename T>
    typename vector<T>::const_iterator vector<T>::cend() const
    {
        return buffer + vec_sz;
    }

    template <typename T>
    typename vector<T>::reverse_iterator vector<T>::rbegin()
    {
        return reverse_iterator(buffer + vec_sz);
    }

    template <typename T>
    typename vector<T>::const_reverse_iterator vector<T>::crbegin() const
    {
        return reverse_iterator(buffer + vec_sz);
    }

    template <typename T>
    typename vector<T>::reverse_iterator vector<T>::rend()
    {
        return reverse_iterator(buffer);
    }

    template <typename T>
    typename vector<T>::const_reverse_iterator vector<T>::crend() const
    {
        return reverse_iterator(buffer);
    }

    template <typename T>
    inline void vector<T>::reallocate()
    {
        ++reallocateCnt;
        try
        {
            T *temp_buffer = allocator.allocate(cap_sz);
            std::uninitialized_copy(buffer, buffer + vec_sz, temp_buffer);
            allocator.deallocate(buffer, cap_sz);
            buffer = temp_buffer;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Allocation Error: " << e.what() << '\n';
        }
    }

    template <typename T>
    inline void vector<T>::grow_allocate(vector<T>::size_type n)
    {
        cap_sz = n << 2;
        try
        {
            buffer = allocator.allocate(cap_sz);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Allocation Error: " << e.what() << '\n';
        }
    }

    template <typename T>
    inline typename vector<T>::size_type vector<T>::getReallocateCnt() const
    {
        return reallocateCnt;
    }

    template <typename T>
    inline void vector<T>::grow_reallocate(vector<T>::size_type n)
    {
        cap_sz = n << 2;
        reallocate();
    }

    template <typename T>
    inline bool vector<T>::empty() const
    {
        return vec_sz == 0;
    }

    template <typename T>
    inline typename vector<T>::size_type vector<T>::size() const
    {
        return vec_sz;
    }

    template <typename T>
    inline typename vector<T>::size_type vector<T>::max_size() const
    {
        return allocator.max_size();
    }

    template <typename T>
    inline typename vector<T>::size_type vector<T>::capacity() const
    {
        return cap_sz;
    }

    template <typename T>
    void vector<T>::resize(typename vector<T>::size_type size)
    {
        if (size > vec_sz)
        {
            if (size > vec_sz)
            {
                cap_sz = size;
                reallocate();
            }
        }
        else
        {
            for (size_type i = vec_sz; i < size; ++i)
                allocator.destroy(buffer + i);
        }
        vec_sz = size;
    }

    template <typename T>
    void vector<T>::resize(typename vector<T>::size_type size, const T &c)
    {
        if (size > vec_sz)
        {
            if (size > cap_sz)
            {
                cap_sz = size;
                reallocate();
            }
            for (size_type i = vec_sz; i < size; ++i)
                buffer[i] = c;
        }
        else
        {
            for (size_type i = vec_sz; i < size; ++i)
                allocator.destroy(buffer + i);
        }
        vec_sz = size;
    }

    template <typename T>
    void vector<T>::reserve(typename vector<T>::size_type size)
    {
        if (size > cap_sz)
        {
            cap_sz = size;
            reallocate();
        }
    }

    template <typename T>
    void vector<T>::shrink_to_fit()
    {
        cap_sz = vec_sz;
        reallocate();
    }

    template <typename T>
    typename vector<T>::reference vector<T>::operator[](typename vector<T>::size_type index)
    {
        return buffer[index];
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::operator[](typename vector<T>::size_type index) const
    {
        return buffer[index];
    }

    template <typename T>
    typename vector<T>::reference vector<T>::at(size_type pos)
    {
        if (pos < vec_sz)
            return buffer[pos];
        else
            throw std::out_of_range("Position not in element range");
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::at(size_type pos) const
    {
        if (pos < vec_sz)
            return buffer[pos];
        else
            throw std::out_of_range("Position not in element range");
    }

    template <typename T>
    typename vector<T>::reference vector<T>::front()
    {
        return buffer[0];
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::front() const
    {
        return buffer[0];
    }

    template <typename T>
    typename vector<T>::reference vector<T>::back()
    {
        return buffer[vec_sz - 1];
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::back() const
    {
        return buffer[vec_sz - 1];
    }

    template <typename T>
    T *vector<T>::data()
    {
        return buffer;
    }

    template <typename T>
    const T *vector<T>::data() const
    {
        return buffer;
    }

    template <typename T>
    typename vector<T>::allocator_type vector<T>::getAllocator() const
    {
        return allocator;
    }

    template <typename T>
    template <class... Args>
    void vector<T>::emplace_back(Args &&... args)
    {
        if (vec_sz == cap_sz)
            grow_reallocate(cap_sz);
        buffer[vec_sz++] = std::move(T(std::forward<Args>(args)...));
    }

    template <typename T>
    void vector<T>::push_back(const T &val)
    {
        if (vec_sz == cap_sz)
            grow_reallocate(cap_sz);
        allocator.construct(buffer + vec_sz++, val);
    }

    template <typename T>
    void vector<T>::push_back(T &&val)
    {
        if (vec_sz == cap_sz)
            grow_reallocate(cap_sz);
        allocator.construct(buffer + vec_sz++, val);
    }

    template <typename T>
    void vector<T>::pop_back()
    {
        allocator.destroy(buffer + vec_sz--);
    }

    template <typename T>
    template <class... Args>
    typename vector<T>::iterator vector<T>::emplace(vector<T>::const_iterator pos, Args &&... args)
    {
        if (vec_sz == cap_sz)
            grow_reallocate(cap_sz);

        iterator it = &buffer[pos - buffer];
        std::uninitialized_move(it, buffer + vec_sz, it + 1);

        (*it) = std::move(T(std::forward<Args>(args)...));
        ++vec_sz;

        return it;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, const T &val)
    {
        size_type index = pos - buffer;
        if (vec_sz == cap_sz)
            grow_reallocate(cap_sz);

        iterator it = &buffer[index];
        std::uninitialized_move(it, buffer + vec_sz, it + 1);
        (*it) = val;
        ++vec_sz;

        return it;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, T &&val)
    {
        size_type index = pos - buffer;
        if (vec_sz == cap_sz)
            grow_reallocate(cap_sz);

        iterator it = &buffer[index];
        std::uninitialized_move(it, buffer + vec_sz, it + 1);
        (*it) = val;
        ++vec_sz;
        return it;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator pos, vector<T>::size_type cnt, const T &val)
    {
        iterator f = &buffer[pos - buffer];
        if (!cnt)
            return f;
        if (vec_sz + cnt > cap_sz)
            grow_reallocate(vec_sz + cnt);
        std::uninitialized_move(f, buffer + vec_sz, f + cnt);
        vec_sz += cnt;
        for (iterator pos = f; cnt--; ++pos)
            (*pos) = val;
        return f;
    }

    template <typename T>
    template <class InputIt>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator pos, InputIt first, InputIt last)
    {
        iterator f = &buffer[pos - buffer];
        size_type cnt = last - first;
        if (!cnt)
            return f;
        if (vec_sz + cnt > cap_sz)
            grow_reallocate(vec_sz + cnt);
        std::uninitialized_move(f, buffer + vec_sz, f + cnt);
        for (iterator pos = f; first != last; ++pos, ++first)
            (*pos) = *first;
        vec_sz += cnt;
        return f;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator pos, std::initializer_list<T> list)
    {
        size_type cnt = list.size();

        if (vec_sz + cnt > cap_sz)
            grow_reallocate(vec_sz + cnt);

        iterator f = &buffer[pos - buffer];

        if (!cnt)
            return f;
        std::uninitialized_move(f, buffer + vec_sz, f + cnt);
        iterator it = f;
        for (auto &item : list)
        {
            (*it) = item;
            ++it;
        }
        vec_sz += cnt;
        return f;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::erase(vector<T>::const_iterator pos)
    {
        iterator it = &buffer[pos - buffer];
        allocator.destroy(it);
        std::uninitialized_move(it + 1, buffer + vec_sz, it);

        --vec_sz;
        return it;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
    {
        iterator f = &buffer[first - buffer], l = &buffer[last - buffer], d = f;
        if (first == last)
            return f;
        for (; d != l; ++d)
            allocator.destroy(d);
        std::uninitialized_move(l, buffer + vec_sz, f);

        vec_sz -= last - first;
        return f;
    }

    template <typename T>
    void vector<T>::swap(vector<T> &rhs)
    {
        size_t temp_vec_sz = vec_sz,
               temp_cap_sz = cap_sz;
        T *temp_buffer = buffer;

        vec_sz = rhs.vec_sz;
        vec_sz = rhs.vec_sz;
        buffer = rhs.buffer;

        rhs.vec_sz = temp_vec_sz;
        rhs.vec_sz = temp_cap_sz;
        rhs.buffer = temp_buffer;
    }

    template <typename T>
    void vector<T>::clear()
    {
        for (size_type i = 0; i < vec_sz; ++i)
            allocator.destroy(buffer + i);
        vec_sz = 0;
    }

    template <typename T>
    bool vector<T>::operator==(const vector<T> &rhs) const
    {
        if (vec_sz != rhs.vec_sz)
            return false;
        for (size_type i = 0; i < vec_sz; ++i)
            if (buffer[i] != rhs.buffer[i])
                return false;
        return true;
    }

    template <typename T>
    bool vector<T>::operator!=(const vector<T> &rhs) const
    {
        if (vec_sz != rhs.vec_sz)
            return true;
        for (size_type i = 0; i < vec_sz; ++i)
            if (buffer[i] != rhs.buffer[i])
                return true;
        return false;
    }

} // namespace rice
