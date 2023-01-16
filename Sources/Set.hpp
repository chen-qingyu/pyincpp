/**
 * @file Set.hpp
 * @author 青羽 (chen_qingyu@qq.com, https://chen-qingyu.github.io/)
 * @brief Set template class, implemented by AVL tree.
 * @version 0.1
 * @date 2023.01.15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SET_HPP
#define SET_HPP

namespace mdspp
{

template <typename T>
class Set
{
private:
    // Tree node.
    class Node
    {
        friend class Set<T>;

    private:
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

    // Number of elements.
    int size_;

    // Pointer to the root.
    Node* root_;

public:
    /*
     * Constructor / Destructor
     */

    /**
     * @brief Construct a new set object.
     */
    Set()
        : size_(0)
        , root_(nullptr)
    {
    }

    /*
     * Examination (will not change the object itself)
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
};

} // namespace mdspp

#endif // SET_HPP
