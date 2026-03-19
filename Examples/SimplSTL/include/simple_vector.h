#pragma once
#include <new>
#include <utility>
#include <cstddef>
#include <initializer_list>

template <typename T>
class SimpleVector
{
public:
    SimpleVector(std::initializer_list<T> list);
    SimpleVector(size_t size);
    SimpleVector();
    ~SimpleVector();

    void PushBack(const T& value);
    void PopBack();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t size() const
    {
        return m_size;
    }
    size_t Capacity() const
    {
        return m_capacity;
    }

private:
    void Grow();

private:
    T* m_pData{nullptr};
    size_t m_size{0};
    size_t m_capacity{0};

private:
    SimpleVector(const SimpleVector&) = delete;
    SimpleVector& operator=(const SimpleVector&) = delete;
};

template <typename T>
inline SimpleVector<T>::SimpleVector(std::initializer_list<T> list)
{
    for (const auto& item : list) {
        PushBack(item);
    }
}

template <typename T>
inline SimpleVector<T>::SimpleVector(size_t size)
    : m_size(size),
      m_capacity(size),
      m_pData(static_cast<T*>(::operator new(sizeof(T) * m_capacity)))
{
    for (size_t i = 0; i < m_size; i++) {
        new (&m_pData[i]) T();
    }
}

template <typename T>
inline SimpleVector<T>::SimpleVector()
    : m_size(0), m_capacity(0), m_pData(nullptr)
{
}

template <typename T>
inline SimpleVector<T>::~SimpleVector()
{
    for (auto i = 0; i < m_size; i++) {
        m_pData[i].~T();
    }
    ::operator delete(m_pData);
}

template <typename T>
inline void SimpleVector<T>::PushBack(const T& value)
{
    Grow();
    new (&m_pData[m_size]) T(value);
    m_size++;
}

template <typename T>
inline void SimpleVector<T>::PopBack()
{
    if (m_size > 0) {
        m_pData[m_size - 1].~T();
        m_size--;
    }
}

template <typename T>
inline void SimpleVector<T>::Grow()
{
    if (m_size >= m_capacity) {
        size_t newCapacity = m_capacity == 0 ? 1 : m_capacity * 2;
        T* newData = static_cast<T*>(::operator new(sizeof(T) * newCapacity));
        for (size_t i = 0; i < m_size; i++) {
            new (&newData[i]) T(std::move(m_pData[i]));
            m_pData[i].~T();
        }

       ::operator delete(m_pData);
        m_pData = newData;
        m_capacity = newCapacity;
    }
}

template <typename T>
inline T& SimpleVector<T>::operator[](size_t index)
{
    return m_pData[index];
}

template <typename T>
inline const T& SimpleVector<T>::operator[](size_t index) const
{
    return m_pData[index];
}