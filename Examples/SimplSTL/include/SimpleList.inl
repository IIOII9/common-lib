#pragma once
#include <cassert>
template <typename T>
inline T& SimpleListIterator<T>::operator*()
{
    return static_cast<SimpleListNode<T>*>(m_pNode)->m_value;
}

template <typename T>
inline const T& SimpleListIterator<T>::operator*() const
{
    return static_cast<SimpleListNode<T>*>(m_pNode)->m_value;
}

template <typename T>
inline T* SimpleListIterator<T>::operator->()
{
    return &(static_cast<SimpleListNode<T>*>(m_pNode)->m_value);
}

template <typename T>
inline const T* SimpleListIterator<T>::operator->() const
{
    return &(static_cast<SimpleListNode<T>*>(m_pNode)->m_value);
}

template <typename T>
inline SimpleListIterator<T>& SimpleListIterator<T>::operator++()
{
    m_pNode = m_pNode->m_pNext;
    return *this;
}

template <typename T>
inline SimpleListIterator<T> SimpleListIterator<T>::operator++(int)
{
    SimpleListIterator tmp = *this;
    ++(*this);
    return *tmp;
}

template <typename T>
inline SimpleListIterator<T>& SimpleListIterator<T>::operator--()
{
    m_pNode = m_pNode->m_pPrev;
    return *this;
}

template <typename T>
inline SimpleListIterator<T> SimpleListIterator<T>::operator--(int)
{
    SimpleListIterator tmp = *this;
    --(*this);
    return *tmp;
}

template <typename T>
inline bool SimpleListIterator<T>::operator!=(
    const SimpleListIterator& other) const
{
    return m_pNode != other.m_pNode;
}

template <typename T>
inline bool SimpleListIterator<T>::operator==(
    const SimpleListIterator& other) const
{
    return m_pNode == other.m_pNode;
}

template <typename T>
inline SimpleList<T>::SimpleList() : m_pSentinel(nullptr), m_size(0)
{
    m_pSentinel = new ListNodeBase();    // 创建哨兵节点
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
    Insert(end(), std::forward<U>(value));
}

template <typename T>
template <typename U>
inline void SimpleList<T>::PushFront(U&& value)
{
    Insert(begin(), std::forward<U>(value));
}

template <typename T>
inline void SimpleList<T>::LinkBefore(ListNodeBase* pPosNode,
                                      ListNodeBase* pNode)
{
    assert(pPosNode != nullptr);
    assert(pNode != nullptr);

    // pos 必须已经在链里
    assert(pPosNode->m_pPrev != nullptr);
    assert(pPosNode->m_pNext != nullptr);

    // node 应该是“未挂链”的新节点
    assert(pNode->m_pPrev == nullptr);
    assert(pNode->m_pNext == nullptr);

    // 不能把自己插到自己前面
    if (pNode == pPosNode)
        return;

    auto* pPrev = pPosNode->m_pPrev;

    pPosNode->m_pPrev = pNode;
    pNode->m_pNext = pPosNode;

    pPrev->m_pNext = pNode;
    pNode->m_pPrev = pPrev;
}

template <typename T>
inline void SimpleList<T>::Unlink(ListNodeBase* node)
{
    auto* pPrev = node->m_pPrev;
    auto* pNext = node->m_pNext;
    pPrev->m_pNext = pNext;
    pNext->m_pPrev = pPrev;
    // 将 node 的前后指针置空，表示它已经脱离链表
    node->m_pPrev = nullptr;
    node->m_pNext = nullptr;
}

template <typename T>
template <typename U>
inline SimpleListIterator<T> SimpleList<T>::Insert(Iterator pos, U&& value)
{
    auto* pNode = pos.m_pNode;
    auto* pNewNode = new Node(std::forward<U>(value));

    LinkBefore(pNode, pNewNode);
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
    LinkBefore(pNode, pNewNode);
    ++m_size;
    return Iterator(pNewNode);
}

template <typename T>
template <typename... Args>
inline SimpleList<T>::Iterator SimpleList<T>::EmplaceBack(Args&&... args)
{
    return Emplace(end(), std::forward<Args>(args)...);
}

template <typename T>
inline SimpleListIterator<T> SimpleList<T>::Erase(Iterator pos)
{
    auto* pNode = pos.m_pNode;
    if (pNode == m_pSentinel) {
        return end();  // 不允许删除哨兵节点，直接返回原位置
    }
    auto* pNext = pNode->m_pNext;
    Unlink(pNode);
    delete static_cast<Node*>(pNode);
    --m_size;
    return Iterator(pNext);
}

template <typename T>
inline size_t SimpleList<T>::size() const
{
    return m_size;
}

template <typename T>
inline void SimpleList<T>::Clear()
{
    // 批量销毁整条链,故不需要每次调用 Unlink 来断开节点，直接遍历删除即可
    auto* current = m_pSentinel->m_pNext;
    while (current != m_pSentinel) {
        auto* next = current->m_pNext;
        delete static_cast<Node*>(current);
        current = next;
    }
    m_pSentinel->m_pNext = m_pSentinel;
    m_pSentinel->m_pPrev = m_pSentinel;
    m_size = 0;
}

template <typename T>
inline void SimpleList<T>::Splice(Iterator pos, SimpleList& other, Iterator it)
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
    other.Unlink(pOtherNode);

    // 将 pOtherNode 插入到当前链表 pos 位置之前
    LinkBefore(pos.m_pNode, pOtherNode);

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
    Erase(begin());
}

template <typename T>
inline void SimpleList<T>::PopBack()
{
    if (m_size == 0)
        return;  // 链表为空，不进行操作
    Iterator it = end();
    --it;
    Erase(it);
}
