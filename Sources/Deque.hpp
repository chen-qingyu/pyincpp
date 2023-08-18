/**
 * @file Deque.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Deque template class, implemented by doubly linked list.
 * @version 1.0
 * @date 2023.01.10
 *
 * @copyright Copyright (c) 2023
 */

#ifndef DEQUE_HPP
#define DEQUE_HPP

#include <climits>          // INT_MAX
#include <initializer_list> // std::initializer_list
#include <ostream>          // std::ostream

#include "utility.hpp"

#include "List.hpp"
#include "Set.hpp"

namespace mdspp
{

template <typename T>
class List;

template <typename T>
class Set;

/**
 * @brief Deque template class, implemented by doubly linked list.
 *
 * @tparam T the type of elements in the deque
 */
template <typename T>
class Deque
{
    template <typename X>
    friend std::ostream& operator<<(std::ostream& os, const Deque<X>& deque);

private:
    // Node of linked list.
    class Node
    {
        friend class Deque<T>;

        template <typename X>
        friend std::ostream& operator<<(std::ostream& os, const Deque<X>& deque);

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

private:
    // Number of elements.
    int size_;

    // Pointer to the header.
    Node* header_;

    // Pointer to the trailer.
    Node* trailer_;

    // Insert the given element at the given position.
    void insert(Node* pos, const T& element)
    {
        utility::check_full(size_, INT_MAX);

        auto node = new Node(element, pos->pred_, pos);
        pos->pred_->succ_ = node;
        pos->pred_ = node;

        ++size_;
    }

    // Remove and return an element at the given position.
    T remove(Node* pos)
    {
        utility::check_empty(size_);

        T element = pos->data_;

        pos->pred_->succ_ = pos->succ_;
        pos->succ_->pred_ = pos->pred_;
        delete pos;

        --size_;

        return element;
    }

    // Clear the stored node of data.
    void clear_data()
    {
        while (header_->succ_ != trailer_)
        {
            auto node = header_->succ_->succ_;
            delete header_->succ_;
            header_->succ_ = node;
        }

        size_ = 0;
        header_->succ_ = trailer_;
        trailer_->pred_ = header_;
    }

    // Compare two deques.
    int compare(const Deque& that) const
    {
        for (Node *it1 = header_->succ_, *it2 = that.header_->succ_; it1 != trailer_ && it2 != that.trailer_; it1 = it1->succ_, it2 = it2->succ_)
        {
            if (it1->data_ > it2->data_)
            {
                return 1;
            }
            else if (it1->data_ < it2->data_)
            {
                return -1;
            }
        }

        if (size_ > that.size_)
        {
            return 1;
        }
        else if (size_ < that.size_)
        {
            return -1;
        }
        else // size_ == that.size_
        {
            return 0;
        }
    }

    // Set the node to the first visible node.
    void set_first(Node* node)
    {
        if (node == header_->succ_)
        {
            return;
        }

        // link origin last node and origin first node
        trailer_->pred_->succ_ = header_->succ_;
        header_->succ_->pred_ = trailer_->pred_;

        // link new last node and trailer
        node->pred_->succ_ = trailer_;
        trailer_->pred_ = node->pred_;

        // then, link new first node and header
        node->pred_ = header_;
        header_->succ_ = node;
    }

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new deque object.
     */
    Deque()
        : size_(0)
        , header_(new Node(T()))
        , trailer_(new Node(T()))
    {
        header_->succ_ = trailer_;
        trailer_->pred_ = header_;
    }

    /**
     * @brief Create a deque based on the given initializer list.
     *
     * @param il initializer list
     */
    Deque(const std::initializer_list<T>& il)
        : Deque()
    {
        for (auto it = il.begin(); it != il.end(); ++it)
        {
            push_back(*it);
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another deque
     */
    Deque(const Deque<T>& that)
        : Deque()
    {
        for (Node* it = that.header_->succ_; it != that.trailer_; it = it->succ_)
        {
            push_back(it->data_);
        }
    }

    /**
     * @brief Move constructor.
     *
     * @param that another deque
     */
    Deque(Deque<T>&& that)
        : size_(that.size_)
        , header_(that.header_)
        , trailer_(that.trailer_)
    {
        that.size_ = 0;
        that.header_ = new Node(T());
        that.trailer_ = new Node(T());
        that.header_->succ_ = that.trailer_;
        that.trailer_->pred_ = that.header_;
    }

    /**
     * @brief Destroy the deque object.
     */
    ~Deque()
    {
        clear_data();

        delete header_;
        delete trailer_;
    }

    /*
     * Comparison
     */

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

        for (Node *it1 = header_->succ_, *it2 = that.header_->succ_; it1 != trailer_; it1 = it1->succ_, it2 = it2->succ_)
        {
            if (it1->data_ != it2->data_)
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
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this < that
     */
    bool operator<(const Deque<T>& that) const
    {
        return compare(that) < 0;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this <= that
     */
    bool operator<=(const Deque<T>& that) const
    {
        return compare(that) <= 0;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this > that
     */
    bool operator>(const Deque<T>& that) const
    {
        return compare(that) > 0;
    }

    /**
     * @brief Compare two deques.
     *
     * @param that another deque
     * @return true if this >= that
     */
    bool operator>=(const Deque<T>& that) const
    {
        return compare(that) >= 0;
    }

    /*
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

            for (Node* it = that.header_->succ_; it != that.trailer_; it = it->succ_)
            {
                push_back(it->data_);
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

    /*
     * Access
     */

    /**
     * @brief Peek the back element.
     *
     * @return the back element
     */
    const T& back() const
    {
        utility::check_empty(size_);

        return trailer_->pred_->data_;
    }

    /**
     * @brief Peek the front element.
     *
     * @return the front element
     */
    const T& front() const
    {
        utility::check_empty(size_);

        return header_->succ_->data_;
    }

    /**
     * @brief Get the back element.
     *
     * @return the back element
     */
    T& back()
    {
        utility::check_empty(size_);

        return trailer_->pred_->data_;
    }

    /**
     * @brief Get the front element.
     *
     * @return the front element
     */
    T& front()
    {
        utility::check_empty(size_);

        return header_->succ_->data_;
    }

    /*
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

    /*
     * Manipulation (will change the object itself)
     */

    /**
     * @brief Push an element to the end of the deque.
     *
     * @param element element to be pushed
     */
    void push_back(const T& element)
    {
        insert(trailer_, element);
    }

    /**
     * @brief Push an element to the front of the deque.
     *
     * @param element element to be pushed
     */
    void push_front(const T& element)
    {
        insert(header_->succ_, element);
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
     *
     * @return self reference
     */
    Deque& clear()
    {
        if (size_ != 0)
        {
            clear_data();
        }

        return *this;
    }

    /**
     * @brief Rotate n elements to the right.
     *
     * @param n number of rotate elements
     * @return self reference
     */
    Deque& operator>>=(int n)
    {
        if (size_ <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this <<= -n;
        }

        n %= size_;
        if (n > size_ / 2)
        {
            return *this <<= (size_ - n);
        }

        Node* it = trailer_;
        while (n--)
        {
            it = it->pred_;
        }
        set_first(it);

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
        if (size_ <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this >>= -n;
        }

        n %= size_;
        if (n > size_ / 2)
        {
            return *this >>= (size_ - n);
        }

        Node* it = header_->succ_;
        while (n--)
        {
            it = it->succ_;
        }
        set_first(it);

        return *this;
    }

    /**
     * @brief Reverse the deque in place.
     *
     * @return self reference
     */
    Deque& reverse()
    {
        utility::swap(header_, trailer_);
        for (Node* cur = trailer_; cur != nullptr; cur = cur->pred_)
        {
            utility::swap(cur->pred_, cur->succ_);
        }

        return *this;
    }

    /*
     * Production (will produce new object)
     */

    /**
     * @brief Generate a list with the same elements.
     *
     * @return a list with the same elements
     */
    List<T> to_list() const
    {
        List<T> list;
        for (Node* it = header_->succ_; it != trailer_; it = it->succ_)
        {
            list += it->data_;
        }
        return list;
    }

    /**
     * @brief Generate a set with the same elements.
     *
     * @return a set with the same elements
     */
    Set<T> to_set() const
    {
        Set<T> set;
        for (Node* it = header_->succ_; it != trailer_; it = it->succ_)
        {
            set += it->data_;
        }
        return set;
    }
};

/*
 * Non-member functions
 */

/**
 * @brief Output deque data to the specified output stream.
 *
 * @tparam T the type of elements in the deque, must be printable
 * @param os an output stream
 * @param deque the deque to be printed to the output stream
 * @return self reference of the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const Deque<T>& deque)
{
    if (deque.is_empty())
    {
        return os << "<>";
    }

    auto it = deque.header_->succ_;
    os << "<";
    while (true)
    {
        os << it->data_;
        it = it->succ_;
        if (it == deque.trailer_)
        {
            return os << ">";
        }
        os << ", ";
    }
}

} // namespace mdspp

#endif // DEQUE_HPP
