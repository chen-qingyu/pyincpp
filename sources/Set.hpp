/**
 * @file Set.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Set template class, implemented by red-black tree.
 * @date 2023.01.15
 *
 * @copyright Copyright (C) 2023
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SET_HPP
#define SET_HPP

#include "utility.hpp"

#include "Deque.hpp"

namespace pyincpp
{

// TODO: use red-black tree.

/**
 * @brief Set template class, implemented by red-black tree.
 *
 * @tparam T the type of elements in the set, require support operator<
 */
template <typename T>
class Set
{
    template <typename K, typename V>
    friend class Map;

private:
    // Tree node class.
    struct Node
    {
        // Data stored in the node.
        T data_;

        // Pointer to the left child.
        Node* left_;

        // Pointer to the right child.
        Node* right_;

        // Pointer to the parent.
        Node* parent_;

        // Constructor.
        Node(const T& data, Node* left = nullptr, Node* right = nullptr, Node* parent = nullptr)
            : data_(data)
            , left_(left)
            , right_(right)
            , parent_(parent)
        {
        }

        // Link left child.
        void link_left(Node* child)
        {
            left_ = child;
            if (child != nullptr)
            {
                child->parent_ = this;
            }
        }

        // Link right child.
        void link_right(Node* child)
        {
            right_ = child;
            if (child != nullptr)
            {
                child->parent_ = this;
            }
        }
    };

public:
    /**
     * @brief Set iterator class.
     *
     * Walk the set in ascending order. This means that begin() is the smallest element.
     *
     * Because the internal elements of the set have a fixed order,
     * thus the iterator of the set only supports access and does not support modification.
     */
    class Iterator
    {
        friend class Set;

        template <typename K, typename V>
        friend class Map;

    private:
        // Current node pointer.
        Node* current_;

        // Constructor.
        Iterator(Node* current)
            : current_(current)
        {
        }

        // Iterator to next ascending node.
        void next()
        {
            if (current_->right_) // have right sub tree
            {
                current_ = current_->right_;
                while (current_->left_) // find min in right sub tree
                {
                    current_ = current_->left_;
                }
            }
            else // back to the next ascending node
            {
                while (current_->parent_->right_ == current_)
                {
                    current_ = current_->parent_;
                }
                current_ = current_->parent_;
            }
        }

        // Iterator to previous ascending node.
        void previous()
        {
            if (current_->left_) // have left sub tree
            {
                current_ = current_->left_;
                while (current_->right_) // find max in left sub tree
                {
                    current_ = current_->right_;
                }
            }
            else // back to the previous ascending node
            {
                while (current_->parent_->left_ == current_)
                {
                    current_ = current_->parent_;
                }
                current_ = current_->parent_;
            }
        }

    public:
        /**
         * @brief Dereference.
         *
         * @return reference of the data
         */
        const T& operator*() const
        {
            return current_->data_;
        }

        /**
         * @brief Get current pointer.
         *
         * @return current pointer
         */
        const T* operator->() const
        {
            return &current_->data_;
        }

        /**
         * @brief Check if two iterators are same.
         *
         * @param that another iterator
         * @return ture if two iterators are same, false otherwise.
         */
        bool operator==(const Iterator& that) const
        {
            return current_ == that.current_;
        }

        /**
         * @brief Check if two iterators are different.
         *
         * @param that another iterator
         * @return ture if two iterators are different, false otherwise.
         */
        bool operator!=(const Iterator& that) const
        {
            return !(*this == that);
        }

        /**
         * @brief Increment the iterator: ++it.
         *
         * @return reference of this iterator that point to next data
         */
        Iterator& operator++()
        {
            next();
            return *this;
        }

        /**
         * @brief Increment the iterator: it++.
         *
         * @return const reference of this iterator that point to current data
         */
        Iterator operator++(int)
        {
            auto tmp = *this;
            next();
            return tmp;
        }

        /**
         * @brief Decrement the iterator: --it.
         *
         * @return reference of this iterator that point to previous data
         */
        Iterator& operator--()
        {
            previous();
            return *this;
        }

        /**
         * @brief Decrement the iterator: it--.
         *
         * @return const reference of this iterator that point to current data
         */
        Iterator operator--(int)
        {
            auto tmp = *this;
            previous();
            return tmp;
        }
    };

private:
    // Number of elements.
    int size_;

    // Pointer to the root.
    Node* root_;

    // Virtual maximum node.
    Node* end_;

    // Find subtree minimum node.
    Node* find_min(Node* pos) const
    {
        if (pos)
        {
            while (pos->left_)
            {
                pos = pos->left_;
            }
        }

        return pos == nullptr ? end_ : pos;
    }

    // Find subtree maximum node.
    Node* find_max(Node* pos) const
    {
        if (pos)
        {
            while (pos->right_)
            {
                pos = pos->right_;
            }
        }

        return pos == nullptr ? end_ : pos;
    }

    // Insert node.
    Node* insert(Node* pos, const T& element)
    {
        if (pos == nullptr)
        {
            pos = new Node(element);
            size_++;
        }
        else
        {
            if (element < pos->data_)
            {
                pos->link_left(insert(pos->left_, element));
            }
            else if (pos->data_ < element)
            {
                pos->link_right(insert(pos->right_, element));
            }
        }

        return pos;
    }

    // Remove node.
    Node* remove(Node* pos, const T& element)
    {
        if (pos)
        {
            if (element < pos->data_)
            {
                pos->link_left(remove(pos->left_, element));
            }
            else if (pos->data_ < element)
            {
                pos->link_right(remove(pos->right_, element));
            }
            else // element == pos->data_
            {
                if (pos->left_ && pos->right_)
                {
                    Node* tmp = find_min(pos->right_);
                    pos->data_ = tmp->data_;
                    pos->link_right(remove(pos->right_, tmp->data_));
                }
                else
                {
                    Node* tmp = pos;
                    pos = pos->left_ ? pos->left_ : pos->right_;
                    delete tmp;
                    size_--;
                }
            }
        }

        return pos;
    }

    // Destroy the subtree rooted at the specified node.
    static void destroy(Node* node)
    {
        if (node)
        {
            destroy(node->left_);
            destroy(node->right_);
            delete node;
        }
    }

    // Traverse the subtree rooted at the specified node.
    template <typename F>
    static void level_action(Node* node, const F& action)
    {
        // level order
        if (node != nullptr)
        {
            Deque<Node*> queue;
            queue.push_back(node);
            while (!queue.is_empty())
            {
                node = queue.pop_front();
                action(node->data_);
                if (node->left_)
                {
                    queue.push_back(node->left_);
                }
                if (node->right_)
                {
                    queue.push_back(node->right_);
                }
            }
        }
    }

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new empty set object.
     */
    Set()
        : size_(0)
        , root_(nullptr)
        , end_(new Node(T()))
    {
    }

    /**
     * @brief Create a set based on the given initializer list.
     *
     * @param il initializer list
     */
    Set(const std::initializer_list<T>& il)
        : Set()
    {
        for (auto it = il.begin(); it != il.end(); ++it)
        {
            *this += *it;
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param that another set
     */
    Set(const Set& that)
        : Set()
    {
        // level copy
        level_action(that.root_, [&](const T& e)
                     { *this += e; });
    }

    /**
     * @brief Move constructor.
     *
     * @param that another set
     */
    Set(Set&& that)
        : size_(that.size_)
        , root_(that.root_)
        , end_(that.end_)
    {
        that.size_ = 0;
        that.root_ = nullptr;
        that.end_ = new Node(T());
    }

    /**
     * @brief Destroy the set object.
     */
    ~Set()
    {
        destroy(end_);
    }

    /*
     * Comparison
     */

    /**
     * @brief Check whether two sets are equal.
     *
     * @param that another set
     * @return true if two sets are equal
     */
    bool operator==(const Set& that) const
    {
        if (size_ != that.size_)
        {
            return false;
        }

        for (auto this_it = begin(), that_it = that.begin(); this_it != end(); ++this_it, ++that_it)
        {
            if (*this_it != *that_it)
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Check whether two sets are not equal.
     *
     * @param that another set
     * @return true if two sets are not equal
     */
    bool operator!=(const Set& that) const
    {
        return !(*this == that);
    }

    /**
     * @brief Test whether the set is a proper subset of another set.
     *
     * @param that another set
     * @return true if the set is a proper subset of another set
     */
    bool operator<(const Set& that) const
    {
        for (auto it = begin(); it != end(); ++it)
        {
            if (!that.contains(*it))
            {
                return false;
            }
        }

        return size_ < that.size_;
    }

    /**
     * @brief Test whether every element in the set is in another set.
     *
     * @param that another set
     * @return true if every element in the set is in another set
     */
    bool operator<=(const Set& that) const
    {
        return *this < that || *this == that;
    }

    /**
     * @brief Test whether the set is a proper superset of another set.
     *
     * @param that another set
     * @return true if the set is a proper superset of another set
     */
    bool operator>(const Set& that) const
    {
        return that < *this;
    }

    /**
     * @brief Test whether every element in another set is in the set.
     *
     * @param that another set
     * @return true if every element in another set is in the set
     */
    bool operator>=(const Set& that) const
    {
        return *this > that || *this == that;
    }

    /*
     * Assignment
     */

    /**
     * @brief Copy assignment operator.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator=(const Set& that)
    {
        if (this != &that)
        {
            destroy(root_);

            size_ = 0;
            root_ = nullptr;

            // level copy
            level_action(that.root_, [&](const T& e)
                         { *this += e; });
        }

        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator=(Set&& that)
    {
        if (this != &that)
        {
            destroy(end_);

            size_ = that.size_;
            root_ = that.root_;
            end_ = that.end_;

            that.size_ = 0;
            that.root_ = nullptr;
            that.end_ = new Node(T());
        }

        return *this;
    }

    /*
     * Iterator
     */

    /**
     * @brief Return an iterator to the first element of the set.
     *
     * If the set is empty, the returned iterator will be equal to end().
     *
     * @return iterator to the first element
     */
    Iterator begin() const
    {
        return Iterator(find_min(root_));
    }

    /**
     * @brief Return an iterator to the element following the last element of the set.
     *
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     *
     * @return iterator to the element following the last element
     */
    Iterator end() const
    {
        return Iterator(end_);
    }

    /*
     * Examination
     */

    /**
     * @brief Return the number of elements in the set.
     *
     * @return the number of elements in the set
     */
    int size() const
    {
        return size_;
    }

    /**
     * @brief Return true if the set contains no elements.
     *
     * @return true if the set contains no elements
     */
    bool is_empty() const
    {
        return size_ == 0;
    }

    /**
     * @brief Return the iterator of the specified element in the set.
     *
     * Or end() if the set does not contain the element.
     *
     * @param element element to search for
     * @return the iterator of the specified element in the set, or end() if the set does not contain the element
     */
    Iterator find(const T& element) const
    {
        Node* current = root_;

        while (current)
        {
            if (current->data_ < element)
            {
                current = current->right_;
            }
            else if (element < current->data_)
            {
                current = current->left_;
            }
            else // element == current->data_
            {
                return Iterator(current);
            }
        }

        return end();
    }

    /**
     * @brief Return true if the set contains the specified element.
     *
     * @param element element whose presence in the set is to be tested
     * @return true if the set contains the specified element
     */
    bool contains(const T& element) const
    {
        return find(element) != end();
    }

    /**
     * @brief Get the smallest item of the set.
     *
     * @return the smallest item
     */
    T min() const
    {
        internal::check_empty(size_);

        return find_min(root_)->data_;
    }

    /**
     * @brief Get the largest item of the set.
     *
     * @return the largest item
     */
    T max() const
    {
        internal::check_empty(size_);

        return find_max(root_)->data_;
    }

    /*
     * Manipulation
     */

    /**
     * @brief Add the specified element to the set.
     *
     * @param element element to be added to the set
     * @return self reference
     */
    Set& operator+=(const T& element)
    {
        internal::check_full(size_, INT_MAX);

        root_ = insert(root_, element);
        end_->link_left(root_);

        return *this;
    }

    /**
     * @brief Remove the specified element from the set, if it is present.
     *
     * If the set does not contain the element, it is unchanged.
     *
     * @param element element to be removed
     * @return self reference
     */
    Set& operator-=(const T& element)
    {
        root_ = remove(root_, element);
        end_->link_left(root_);

        return *this;
    }

    /**
     * @brief Adds a value to the set. Returns whether the value was newly inserted.
     *
     * @param element element to be added to the set
     * @return true if the value was newly inserted
     */
    bool insert(const T& element)
    {
        int old_size = size_;
        *this += element;
        return old_size != size_;
    }

    /**
     * @brief Removes a value from the set. Returns whether such an element was present.
     *
     * @param element element to be removed
     * @return true if such an element was present
     */
    bool remove(const T& element)
    {
        int old_size = size_;
        *this -= element;
        return old_size != size_;
    }

    /**
     * @brief Remove all of the elements from the set.
     *
     * @return self reference
     */
    Set& clear()
    {
        if (size_ != 0)
        {
            destroy(root_);

            size_ = 0;
            root_ = nullptr;
            end_->link_left(root_);
        }

        return *this;
    }

    /**
     * @brief Intersect with another set.
     *
     * Update the set, keeping only elements found in it and all others.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator&=(const Set& that)
    {
        return *this = std::move(*this & that);
    }

    /**
     * @brief Union with another set.
     *
     * Update the set, adding elements from all others.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator|=(const Set& that)
    {
        level_action(that.root_, [&](const T& e)
                     { *this += e; });

        return *this;
    }

    /**
     * @brief Difference with another set.
     *
     * Update the set, removing elements found in others.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator-=(const Set& that)
    {
        return *this = std::move(*this - that);
    }

    /**
     * @brief Symmetric difference with another set.
     *
     * Update the set, keeping only elements found in either set, but not in both.
     *
     * @param that another set
     * @return self reference
     */
    Set& operator^=(const Set& that)
    {
        return *this = std::move(*this ^ that);
    }

    /*
     * Production (will produce new object)
     */

    /**
     * @brief Intersect with another set.
     *
     * @param that another set
     * @return a new set with elements common to the set and all others
     */
    Set operator&(const Set& that) const
    {
        Set new_set;
        level_action(root_, [&](const T& e)
                     { if (that.contains(e)) { new_set += e; } });

        return new_set;
    }

    /**
     * @brief Union with another set.
     *
     * @param that another set
     * @return a new set with elements from the set and all others
     */
    Set operator|(const Set& that) const
    {
        Set new_set = *this;
        return new_set |= that;
    }

    /**
     * @brief Difference with another set.
     *
     * @param that another set
     * @return a new set with elements in the set that are not in the others
     */
    Set operator-(const Set& that) const
    {
        Set new_set;
        level_action(root_, [&](const T& e)
                     { if (!that.contains(e)) { new_set += e; } });

        return new_set;
    }

    /**
     * @brief Symmetric difference with another set.
     *
     * @param that another set
     * @return a new set with elements in either the set or other but not both
     */
    Set operator^(const Set& that) const
    {
        return (*this | that) - (*this & that);
    }

    /*
     * Print
     */

    /**
     * @brief Output the set to the specified output stream.
     *
     * @param os an output stream
     * @param set the set to be printed to the output stream
     * @return self reference of the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Set& set)
    {
        if (set.is_empty())
        {
            return os << "{}";
        }

        os << "{";
        auto it = set.begin();
        while (true)
        {
            os << *it++;
            if (it == set.end())
            {
                return os << "}";
            }
            os << ", ";
        }
    }
};

} // namespace pyincpp

#endif // SET_HPP
