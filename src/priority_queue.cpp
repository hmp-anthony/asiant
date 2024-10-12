#include <asiant/priority_queue.hpp>

namespace asiant {

    void priority_queue::push(std::shared_ptr<node_record> nr) {
        heap_data_.push_back(nr);
    }

    std::shared_ptr<node_record> priority_queue::top() {
        return heap_data_[0];
    }
};
