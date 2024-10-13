#include <asiant/priority_queue.hpp>

#include <iostream>
#include <string>

namespace asiant {

    void set::insert(std::shared_ptr<node_record> val) {
        auto new_node = std::make_shared<rb_node>(val);
        // this node will become the parent
        std::shared_ptr<rb_node> parent = nullptr;
        // we start at the root and work our way down
        std::shared_ptr<rb_node> current = root_;
        while(current != nullptr) {
            parent = current;
            if(new_node->data_->cost_so_far_ < current->data_->cost_so_far_)
                current = current->left_;
            else
                current = current->right_;
        }
        // at this point current is nullptr.
        new_node->parent_ = parent;
        if(parent == nullptr)
            root_ = new_node;
        else if (new_node->data_->cost_so_far_ < parent->data_->cost_so_far_)
            parent->left_ = new_node;
        else
            parent->right_ = new_node;
        
        fix_insert(new_node);
    }

    void set::remove(std::shared_ptr<node_record> val) {
        auto node = root_;
        std::shared_ptr<rb_node> z = nullptr;
        while(node != nullptr) {
            if(std::abs(node->data_->cost_so_far_ - val->cost_so_far_) < 0.001) {
                z = node; break;
            }
            else if (val->cost_so_far_ < node->data_->cost_so_far_)
                node = node->left_;
            else 
                node = node->right_;
        }
        if(z == nullptr) return;

        std::shared_ptr<rb_node> x = nullptr;
        std::shared_ptr<rb_node> y = nullptr;

        y = z;
        color y_original_color = y->color_;
        if(z->left_ == nullptr) {
            x = z->right_;
            transplant(root_, z, z->right_);        
        } else if (z->right_ == nullptr) {
            x = z->left_;
            transplant(root_, z, z->left_);
        }
        else {
            y = min_value_node(z->right_);
            y_original_color = y->color_;
            x = y->right_;
            if (y->parent_ == z) {
                if (x != nullptr)
                    x->parent_ = y;
            }
            else {
                transplant(root_, y, y->right_);
                y->right_ = z->right_;
                y->right_->parent_ = y;
            }
            transplant(root_, z, y);
            y->left_ = z->left_;
            y->left_->parent_ = y;
            y->color_ = z->color_;
        }
        if (y_original_color == BLACK) {
            fix_delete(x);
        }
    }

    void set::print()
    {
        if (root_ == nullptr)
            std::cout << "Tree is empty." << std::endl;
        else {
            std::cout << "Red-Black Tree:" << std::endl;
            print_helper(root_, "", true);
        }
    }


    void set::rotate_left(std::shared_ptr<rb_node> x) {
        if (x == nullptr || x->right_ == nullptr)
            return;

        auto child = x->right_;
        x->right_ = child->left_;
        if (child->left_ != nullptr)
            child->left_->parent_ = x;
        child->parent_ = x->parent_;
        if (x->parent_ == nullptr)
            root_ = child;
        else if (x == x->parent_->left_)
            x->parent_->left_ = child;
        else
            x->parent_->right_ = child;
        child->left_ = x;
        x->parent_ = child;
    }

    void set::rotate_right(std::shared_ptr<rb_node> x) {
        if (x == nullptr || x->left_ == nullptr)
            return;

        auto child = x->left_;
        x->left_ = child->right_;
        if (x->left_ != nullptr)
            x->left_->parent_ = x;
        child->parent_ = x->parent_;
        if (x->parent_ == nullptr)
            root_ = child;
        else if (x == x->parent_->left_)
            x->parent_->left_ = child;
        else
            x->parent_->right_ = child;
        child->right_ = x;
        x->parent_ = child;
    }

    void set::fix_insert(std::shared_ptr<rb_node> x) {
        while(x != root_ && x->parent_->color_ == RED) {
            // p - parent, gp - grandparent
            auto p = x->parent_; auto gp = x->parent_->parent_;
            if(p == gp->left_) {
                // u - uncle
                auto u = gp->right_;
                if(u != nullptr && u->color_ == RED) {
                    gp->color_ = RED;
                    p->color_ = BLACK;
                    u->color_ = BLACK;
                    x = gp;
                } else {
                    if(x == p->right_) {
                        rotate_left(p);
                        x = p; p = x->parent_;
                    }
                    rotate_right(gp);
                    std::swap(p->color_, gp->color_);
                    x = p;
                }
            } else {
                auto u = gp->left_;
                if(u != nullptr && u->color_ == RED) {
                    gp->color_ = RED;
                    p->color_ = BLACK;
                    u->color_ = BLACK;
                    x = gp;
                } else {
                    if(x == p->left_) {
                        rotate_right(p);
                        x = p; p = x->parent_;
                    }
                    rotate_left(gp);
                    std::swap(p->color_, gp->color_);
                    x = p;
                }
            }
        }
        root_->color_ = BLACK;
    }   

    std::shared_ptr<set::rb_node> set::min_value_node(std::shared_ptr<rb_node> node) {
        std::shared_ptr<rb_node> current = node;
        while (current->left_ != nullptr)
            current = current->left_;
        return current;
    }
    
    void set::transplant(std::shared_ptr<rb_node> root,
                         std::shared_ptr<rb_node> u, 
                         std::shared_ptr<rb_node> v) {
        if (u->parent_ == nullptr)
            root = v;
        else if (u == u->parent_->left_)
            u->parent_->left_ = v;
        else
            u->parent_->right_ = v;
        if (v != nullptr)
            v->parent_ = u->parent_;
    }
       
    void set::fix_delete(std::shared_ptr<rb_node> node) {
        while (node != root_ && node->color_ == BLACK) {
            if (node == node->parent_->left_) {
                auto sibling = node->parent_->right_;
                if (sibling->color_ == RED) {
                    sibling->color_ = BLACK;
                    node->parent_->color_ = RED;
                    rotate_left(node->parent_);
                    sibling = node->parent_->right_;
                }
                if ((sibling->left_ == nullptr || sibling->left_->color_ == BLACK)
                 && (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
                    sibling->color_ = RED;
                    node = node->parent_;
                } else {
                    if (sibling->right_ == nullptr || sibling->right_->color_ == BLACK) {
                        if (sibling->left_ != nullptr)
                            sibling->left_->color_ = BLACK;
                        sibling->color_ = RED;
                        rotate_right(sibling);
                        sibling = node->parent_->right_;
                    }
                    sibling->color_ = node->parent_->color_;
                    node->parent_->color_ = BLACK;
                    if (sibling->right_ != nullptr)
                        sibling->right_->color_ = BLACK;
                    rotate_left(node->parent_);
                    node = root_;
                }
            }
            else {
                auto sibling = node->parent_->left_;
                if (sibling->color_ == RED) {
                    sibling->color_ = BLACK;
                    node->parent_->color_ = RED;
                    rotate_right(node->parent_);
                    sibling = node->parent_->left_;
                }
                if ((sibling->left_ == nullptr || sibling->left_->color_ == BLACK)
                 && (sibling->right_ == nullptr|| sibling->right_->color_ == BLACK)) {
                    sibling->color_ = RED;
                    node = node->parent_;
                }
                else {
                    if (sibling->left_ == nullptr || sibling->left_->color_ == BLACK) {
                        if (sibling->right_ != nullptr)
                            sibling->right_->color_ = BLACK;
                        sibling->color_ = RED;
                        rotate_left(sibling);
                        sibling = node->parent_->left_;
                    }
                    sibling->color_ = node->parent_->color_;
                    node->parent_->color_ = BLACK;
                    if (sibling->left_ != nullptr)
                        sibling->left_->color_ = BLACK;
                    rotate_right(node->parent_);
                    node = root_;
                }
            }
        }
        node->color_ = BLACK;
    }

   void set::print_helper(std::shared_ptr<rb_node> root, std::string indent, bool last)
    {
        if (root != nullptr) {
            std::cout << indent;
            if (last) {
                std::cout << "R----";
                indent += "   ";
            }
            else {
                std::cout << "L----";
                indent += "|  ";
            }
            std::string sColor
                = (root->color_ == RED) ? "RED" : "BLACK";
            std::cout << root->data_ << "(" << sColor << ")"
                 << std::endl;
            print_helper(root->left_, indent, false);
            print_helper(root->right_, indent, true);
        }
    }

    void priority_queue::push(std::shared_ptr<node_record> nr) {
        heap_data_.push_back(nr);
        int child_index = heap_data_.size() - 1;
        while(child_index > 0) {
            int parent_index = (child_index - 1) / 2;
            // our child must be greater than our parent in min heap
            if(heap_data_[child_index]->cost_so_far_ 
                    >= heap_data_[parent_index]->cost_so_far_) break;
            std::swap(heap_data_[child_index], heap_data_[parent_index]);
            child_index = parent_index;
        }
       
    }

    std::shared_ptr<node_record> priority_queue::pop() {
        int last_index = heap_data_.size() - 1;
        auto front = heap_data_[0];
        heap_data_[0] = heap_data_[last_index];
        heap_data_.pop_back();

        --last_index;
        int parent_index = 0;
        while(true) {
            // this line and the next make this operation ln(n) in time.
            // that is, same as the cost of pushing. 
            int child_index = parent_index * 2 + 1;
            if(child_index > last_index) break;
            int next_child_index = child_index + 1;
            if(next_child_index <= last_index && heap_data_[next_child_index]->cost_so_far_ < 
                                                 heap_data_[child_index]->cost_so_far_)
                child_index = next_child_index;
            if(heap_data_[parent_index]->cost_so_far_ <= heap_data_[child_index]->cost_so_far_)
                break;
            
            std::swap(heap_data_[child_index], heap_data_[parent_index]);
            parent_index = child_index;

        }
        return front;
    }

    std::shared_ptr<node_record> priority_queue::top() {
        return heap_data_[0];
    }

    int priority_queue::size() {
        return heap_data_.size();
    }

    bool priority_queue::contains(real node_value) {
        return find(node_value) != nullptr;
    }

    std::shared_ptr<node_record> priority_queue::find(real node_value) {
        for(auto & node_rec : heap_data_) {
            if(node_rec->node_.get_value() == node_value) {
                return node_rec;
            }
        }
        return nullptr;
    }
};
