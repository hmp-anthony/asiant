#include <asiant/priority_queue.hpp>

#include<iostream>

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

    bool priority_queue::contains(int node_value) {
        return find(node_value) != nullptr;
    }

    std::shared_ptr<node_record> priority_queue::find(int node_value) {
        for(auto & node_rec : heap_data_) {
            if(node_rec->node_.get_value() == node_value) {
                return node_rec;
            }
        }
        return nullptr;
    }
};
