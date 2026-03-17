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
        :ListNodeBase(),  m_value(std::forward<Args>(args)...)
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
    using Node = SimpleListNode<T>;
    SimpleListIterator(ListNodeBase* node = nullptr) : m_pNode(node)
    {
    }

    T& operator*()
    {
        return static_cast<SimpleListNode<T>*>(m_pNode)->m_value;
    }
    const T& operator*() const
    {
        return m_pNode->m_value;
    }

    SimpleListIterator& operator++()
    {
        m_pNode = m_pNode->m_pNext;
        return *this;
    }
    SimpleListIterator& operator--()
    {
        m_pNode = m_pNode->m_pPrev;
        return *this;
    }
    bool operator!=(const SimpleListIterator& other) const
    {
        return m_pNode != other.m_pNode;
    }

    bool operator==(const SimpleListIterator& other) const
    {
        return m_pNode == other.m_pNode;
    }

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
    SimpleList<T>::Iterator Emplace(Iterator pos, Args&&... args);
    template <typename... Args>
    SimpleList<T>::Iterator EmplaceBack(Args&&... args);
    size_t Size() const;
    void Clear();

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
    // SimpleListNode<T>* m_pHead;
    // SimpleListNode<T>* m_pTail;
    Node* m_pSentinel;
    size_t m_size;

private:
    // 禁止拷贝构造和赋值操作
    SimpleList(const SimpleList&) = delete;
    SimpleList& operator=(const SimpleList&) = delete;
};

template <typename T>
inline SimpleList<T>::SimpleList() : m_pSentinel(nullptr), m_size(0)
{
    m_pSentinel = new Node(T());         // 创建哨兵节点
    m_pSentinel->m_pNext = m_pSentinel;  // 哨兵节点的下一个指向自己
    m_pSentinel->m_pPrev = m_pSentinel;  // 哨兵节点的上一个指向自己
}

template <typename T>
inline SimpleList<T>::~SimpleList()
{
    Clear();
    delete m_pSentinel;
}

template <typename T>
template <typename U>
inline void SimpleList<T>::PushBack(U&& value)
{
    Insert(End(), std::forward<U>(value));
}

template <typename T>
template <typename U>
inline void SimpleList<T>::PushFront(U&& value)
{
    Insert(Begin(), std::forward<U>(value));
}

template <typename T>
template <typename U>
inline SimpleListIterator<T> SimpleList<T>::Insert(Iterator pos, U&& value)
{
    auto* pNode = pos.m_pNode;
    auto* pNewNode = new SimpleListNode<T>(std::forward<U>(value));

    auto* pPrev = pNode->m_pPrev;
    pPrev->m_pNext = pNewNode;
    pNewNode->m_pPrev = pPrev;

    pNewNode->m_pNext = pNode;
    pNode->m_pPrev = pNewNode;

    ++m_size;
    return Iterator(pNewNode);
}

template <typename T>
template <typename... Args>
inline SimpleList<T>::Iterator SimpleList<T>::Emplace(Iterator pos,
                                                      Args&&... args)
{
    auto* pNode = pos.m_pNode;
    // 使用完美转发构造新节点
    auto* pNewNode = new Node(std::forward<Args>(args)...);

    auto* pPrev = pNode->m_pPrev;
    pPrev->m_pNext = pNewNode;
    pNewNode->m_pPrev = pPrev;

    pNewNode->m_pNext = pNode;
    pNode->m_pPrev = pNewNode;

    ++m_size;
    return Iterator(pNewNode);
    // return SimpleList<T>::Iterator();
}

template <typename T>
template <typename... Args>
inline SimpleList<T>::Iterator SimpleList<T>::EmplaceBack(Args&&... args)
{
    return Emplace(End(), std::forward<Args>(args)...);
}

template <typename T>
inline SimpleListIterator<T> SimpleList<T>::Erase(Iterator pos)
{
    auto* pNode = pos.m_pNode;
    if (pNode == m_pSentinel) {
        return End();  // 不允许删除哨兵节点，直接返回原位置
    }
    auto* pPrev = pNode->m_pPrev;
    auto* pNext = pNode->m_pNext;

    pPrev->m_pNext = pNext;
    pNext->m_pPrev = pPrev;

    delete pNode;
    --m_size;
    return Iterator(pNext);
}

template <typename T>
inline size_t SimpleList<T>::Size() const
{
    return m_size;
}

template <typename T>
inline void SimpleList<T>::Clear()
{
    SimpleListNode<T>* current = m_pSentinel->m_pNext;
    while (current != m_pSentinel) {
        SimpleListNode<T>* next = current->m_pNext;
        delete current;
        current = next;
    }
    m_pSentinel->m_pNext = m_pSentinel;
    m_pSentinel->m_pPrev = m_pSentinel;
    m_size = 0;
}

template <typename T>
inline void SimpleList<T>::Splice(SimpleListIterator<T> pos, SimpleList& other,
                                  SimpleListIterator<T> it)
{
    auto* pOtherNode = it.m_pNode;
    if (pOtherNode == other.m_pSentinel) {
        return;  // it 指向的是 other 的哨兵节点，不进行操作
    }

    if (pos == it)
        return;  // pos 和 it 指向同一个位置，不进行操作
    if (this == &other && pos.m_pNode == pOtherNode->m_pNext)
        return;

    // 从 other 中移除 pOtherNode
    auto* pOtherPrev = pOtherNode->m_pPrev;
    auto* pOtherNext = pOtherNode->m_pNext;
    pOtherNext->m_pPrev = pOtherPrev;
    pOtherPrev->m_pNext = pOtherNext;

    // 将 pOtherNode 插入到当前链表 pos 位置之前
    auto* pPosNode = pos.m_pNode;
    auto* pPosPrev = pPosNode->m_pPrev;
    pPosPrev->m_pNext = pOtherNode;
    pOtherNode->m_pPrev = pPosPrev;
    pOtherNode->m_pNext = pPosNode;
    pPosNode->m_pPrev = pOtherNode;

    if (this != &other) {
        --other.m_size;
        ++m_size;
    }
}

template <typename T>
inline void SimpleList<T>::PopFront()
{
    if (m_size == 0)
        return;  // 链表为空，不进行操作
    Erase(Begin());
}

template <typename T>
inline void SimpleList<T>::PopBack()
{
    if (m_size == 0)
        return;  // 链表为空，不进行操作
    Erase(--End());
}