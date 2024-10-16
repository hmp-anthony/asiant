#include <asiant/priority_queue.hpp>

#include <iostream>
#include <string>

namespace asiant {

    void priority_queue::push(std::shared_ptr<node_record> nr) {
        heap_data_.push_back(nr);
        //set_data_.insert(nr);
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
        
        //set_data_.remove(heap_data_[0]);
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

    void priority_queue::print() {
        std::cout << "heap_data" << std::endl;
        for(auto & hd : heap_data_) {
            std::cout << hd->node_.get_value() << " ";
        }
        std::cout << std::endl;
    }
};
