/**
 * @file Deque.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Deque template class, implemented by linked list.
 * @version 1.0
 * @date 2023.01.10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DEQUE_H
#define DEQUE_H

#include <climits> // INT_MAX

#include "common/check_empty.hpp"
#include "common/check_full.hpp"

namespace mdspp
{

/**
 * @brief Deque template class, implemented by linked list.
 *
 * @tparam T the type of elements in the deque
 */
template <typename T>
class Deque
{
private:
    // Node of linked list.
    class Node
    {
        friend class Deque<T>;

    private:
        // Data stored in the node.
        T data_;

        // Predecessor.
        Node* pred_;

        // Successor.
        Node* succ_;

        // Create a node with given element.
        Node(const T& data, Node* pred = nullptr, Node* succ = nullptr)
            : data_(data)
            , pred_(pred)
            , succ_(succ)
        {
        }
    };

    // Number of elements.
    int size_;

    // Pointer to the header.
    Node* header_;

    // Pointer to the trailer.
    Node* trailer_;

    // Insert the given element at the given position.
    void insert(Node* pos, const T& e)
    {
        common::check_full(size_, INT_MAX);

        auto node = new Node(e, pos->pred_, pos);
        pos->pred_->succ_ = node;
        pos->pred_ = node;

        ++size_;
    }

    // Remove and return an element at the given position.
    T remove(Node* pos)
    {
        common::check_empty(size_);

        T data = pos->data_;

        pos->pred_->succ_ = pos->succ_;
        pos->succ_->pred_ = pos->pred_;
        delete pos;

        --size_;
        return data;
    }

    // Clear the stored node of data.
    void clear_data()
    {
        while (size_ > 0)
        {
            pop_back();
        }
    }

public:
    /**
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new deque object.
     */
    Deque()
        : size_(0)
        , header_(new Node(T(), nullptr, nullptr))
        , trailer_(new Node(T(), nullptr, nullptr))
    {
        header_->succ_ = trailer_;
        trailer_->pred_ = header_;
    }

    /**
     * @brief Destroy the deque object.
     *
     */
    ~Deque()
    {
        clear_data();

        delete header_;
        delete trailer_;
    }

    /**
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another deque
     * @return self reference
     */
    Deque& operator=(const Deque<T>& that)
    {
        if (this != &that)
        {
            clear_data();

            for (Node* cur = that.header_->succ_; cur != that.trailer_; cur = cur->succ_)
            {
                push_back(cur->data_);
            }
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another deque
     * @return self reference
     */
    Deque& operator=(Deque<T>&& that)
    {
        if (this != &that)
        {
            clear_data();

            size_ = that.size_;
            header_ = that.header_;
            trailer_ = that.trailer_;

            that.size_ = 0;
            that.header_ = new Node(T());
            that.trailer_ = new Node(T());
            that.header_->succ_ = that.trailer_;
            that.trailer_->pred_ = that.header_;
        }

        return *this;
    }

    /**
     * Examination (will not change the object itself)
     */

    /**
     * @brief Return the number of elements in the deque.
     *
     * @return the number of elements in the deque
     */
    int size() const
    {
        return size_;
    }

    /**
     * @brief Return true if the deque contains no elements.
     *
     * @return true if the deque contains no elements
     */
    bool is_empty() const
    {
        return size_ == 0;
    }

    /**
     * @brief Check whether two deques are equal.
     *
     * @param that another deque
     * @return true if two deques are equal
     */
    bool operator==(const Deque<T>& that) const
    {
        if (size_ != that.size_)
        {
            return false;
        }

        for (Node *this_cur = header_->succ_, *that_cur = that.header_->succ_; this_cur != trailer_; this_cur = this_cur->succ_, that_cur = that_cur->succ_)
        {
            if (this_cur->data_ != that_cur->data_)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Check whether two deques are not equal.
     *
     * @param that another deque
     * @return true if two deques are not equal
     */
    bool operator!=(const Deque<T>& that) const
    {
        return !(*this == that);
    }

    /**
     * @brief Peek the back element.
     *
     * @return the back element
     */
    const T& back() const
    {
        common::check_empty(size_);

        return trailer_->pred_->data_;
    }

    /**
     * @brief Peek the front element.
     *
     * @return the front element
     */
    const T& front() const
    {
        common::check_empty(size_);

        return header_->succ_->data_;
    }

    /**
     * Manipulation (will change the object itself)
     */

    /**
     * @brief Push an element to the end of the deque.
     *
     * @param element element to be pushed
     * @return self reference
     */
    Deque& push_back(const T& element)
    {
        insert(trailer_, element);
        return *this;
    }

    /**
     * @brief Push an element to the front of the deque.
     *
     * @param element element to be pushed
     * @return self reference
     */
    Deque& push_front(const T& element)
    {
        insert(header_->succ_, element);
        return *this;
    }

    /**
     * @brief Pop an element of the end of the deque.
     *
     * @return poped element
     */
    T pop_back()
    {
        return remove(trailer_->pred_);
    }

    /**
     * @brief Pop an element of the front of the deque.
     *
     * @return poped element
     */
    T pop_front()
    {
        return remove(header_->succ_);
    }

    /**
     * @brief Clear data.
     */
    void clear()
    {
        clear_data();
    }

    /**
     * @brief Rotate n elements to the right.
     *
     * @param n number of rotate elements
     * @return self reference
     */
    Deque& operator>>=(int n)
    {
        if (n > 0)
        {
            while (n--)
            {
                push_front(pop_back());
            }
        }
        else
        {
            while (n++)
            {
                push_back(pop_front());
            }
        }

        return *this;
    }

    /**
     * @brief Rotate n elements to the left.
     *
     * @param n number of rotate elements
     * @return self reference
     */
    Deque& operator<<=(int n)
    {
        if (n > 0)
        {
            while (n--)
            {
                push_back(pop_front());
            }
        }
        else
        {
            while (n++)
            {
                push_front(pop_back());
            }
        }

        return *this;
    }
};

} // namespace mdspp

#endif // DEQUE_H
