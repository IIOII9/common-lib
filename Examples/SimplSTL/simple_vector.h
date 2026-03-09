#pragma once

template <typename T>
class SimpleVector
{
public:
    SimpleVector(size_t size);
    void PushBack(const T& value);
    void PopBack();
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    size_t Size() const
    {
        return m_size;
    }

private:
    void Grow();

private:
    T* m_pData{nullptr};
    size_t m_size{0};
    size_t m_capacity{0};
};

template <typename T>
inline SimpleVector<T>::SimpleVector(size_t size):
    m_pData(new T[size]),
    m_size(size), m_capacity(size)
{
}

template <typename T>
inline void SimpleVector<T>::PushBack(const T& value)
{
    Grow();
    m_pData[m_size++] = value;
}

template <typename T>
inline void SimpleVector<T>::PopBack()
{
    if (m_size > 0)
    {
        // new 创建的对象会调用构造函数，delete
        // 删除对象会调用析构函数，所以在这里不需要手动调用析构函数。
        //m_pData[m_size - 1].~T();
        m_size--;
    }
}

template <typename T>
inline void SimpleVector<T>::Grow()
{
    if (m_size + 1 >= m_capacity)
    {
        size_t newCapacity = m_capacity == 0 ? 1 : m_capacity * 2;
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; i++) {
            newData[i] = m_pData[i];
        }
        
        delete[] m_pData;
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