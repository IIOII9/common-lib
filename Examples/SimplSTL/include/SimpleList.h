#pragma once
#include <utility>
#include <cstddef>

template <typename T>
class SimpleList;

template <typename T>
class SimpleListIterator;

class ListNodeBase
{
protected:
    ListNodeBase* m_pNext;
    ListNodeBase* m_pPrev;
    ListNodeBase() : m_pNext(nullptr), m_pPrev(nullptr)
    {
    }
    template <typename>
    friend class SimpleList;
    template <typename>
    friend class SimpleListIterator;
};

template <typename T>
class SimpleListNode : public ListNodeBase
{
public:
    template <typename... Args>
    SimpleListNode(Args&&... args)
        : ListNodeBase(), m_value(std::forward<Args>(args)...)
    {
    }

private:
    T m_value;
    template <typename>
    friend class SimpleList;
    template <typename>
    friend class SimpleListIterator;
};

template <typename T>
class SimpleListIterator
{
public:
    SimpleListIterator(ListNodeBase* node = nullptr) : m_pNode(node)
    {
    }

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;
    // 前置++，++it
    SimpleListIterator& operator++();
    // 后置++，it++
    SimpleListIterator operator++(int);

    SimpleListIterator& operator--();
    SimpleListIterator operator--(int);

    bool operator!=(const SimpleListIterator& other) const;
    bool operator==(const SimpleListIterator& other) const;

private:
    ListNodeBase* m_pNode;
    template <typename>
    friend class SimpleList;
};


template <typename T>
class SimpleList
{
public:
    using Iterator = SimpleListIterator<T>;
    using Node = SimpleListNode<T>;

public:
    SimpleList();
    ~SimpleList();
    template <typename U>
    void PushBack(U&& value);
    template <typename U>
    void PushFront(U&& value);

    template <typename U>
    SimpleListIterator<T> Insert(Iterator pos, U&& value);
    SimpleListIterator<T> Erase(Iterator pos);

    void PopBack();
    void PopFront();
    template <typename... Args>
    Iterator Emplace(Iterator pos, Args&&... args);
    template <typename... Args>
    Iterator EmplaceBack(Args&&... args);
    size_t Size() const;
    void Clear();
    bool Empty() const
    {
        return m_size == 0;
    }
    Iterator Begin() const
    {
        return Iterator(m_pSentinel->m_pNext);
    }

    Iterator End() const
    {
        return Iterator(m_pSentinel);
    }
    /*把 other 中 it 指向的节点
    移动到当前链表 pos 位置之前*/
    void Splice(SimpleListIterator<T> pos, SimpleList& other,
                SimpleListIterator<T> it);

private:
    void LinkBefore(ListNodeBase* pNode, ListNodeBase* pNewNode);
    void Unlink(ListNodeBase* node);

private:
    ListNodeBase* m_pSentinel;
    size_t m_size;

private:
    // 禁止拷贝构造和赋值操作
    SimpleList(const SimpleList&) = delete;
    SimpleList& operator=(const SimpleList&) = delete;
};


#include "SimpleList.inl"