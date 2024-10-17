#include <asiant/level.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>

namespace asiant {
    level::level(std::string file) {
        std::ifstream f(file);
        f >> rows_ >> cols_;

        data_.assign(rows_ * cols_, 0);
        std::vector<int> cost(rows_ * cols_);

        for(int j = 0; j < rows_; ++j) {
            for(int i = 0; i < cols_; ++i) {
                f >> data_[j * cols_ + i];
            }
        }
    
        // we also update the data_ vector to take negative 1
        // for a non attainable square.
        int index = 0;
        for(int j = 0; j < rows_; ++j) {
            for(int i = 0; i < cols_; ++i) {
                if(data_[j * cols_ + i] > 0) {
                    locations_.push_back(std::make_pair(j, i));
                    cost[j * cols_ + i] = data_[j * cols_ + i]; 
                    data_[j * cols_ + i] = index++;
                } else {
                    data_[j * cols_ + i] = -1;
                }
            }
        }

        begin_ = 0;
        end_ = locations_.size() - 1;

        // now we assemble the graph
        graph_ = std::make_shared<graph>(locations_.size(), false);
        for(int j = 1; j < rows_ - 1; ++j) {
            for(int i = 1; i < cols_ - 1; ++i) {
                auto c = j * cols_ + i;
                auto u = (j - 1) * cols_ + i;
                auto d = (j + 1) * cols_ + i;
                auto r = j * cols_ + i + 1;
                auto l = j * cols_ + i - 1;
                if(data_[c] == -1) continue; 
                
                if(data_[u] != -1) {
                    graph_->insert(std::make_shared<connection>(data_[c], data_[u], cost[u]));
                }
                if(data_[d] != -1) {
                    graph_->insert(std::make_shared<connection>(data_[c], data_[d], cost[d]));
                }
                if(data_[r] != -1) {
                    graph_->insert(std::make_shared<connection>(data_[c], data_[r], cost[r]));
                }
                if(data_[l] != -1) {
                    graph_->insert(std::make_shared<connection>(data_[c], data_[l], cost[l]));
                }
            }
        }
    }

    

    std::shared_ptr<graph> level::get_graph() {
        return graph_;
    }

    std::vector<std::pair<int, int>> level::get_locations() {
        return locations_;
    }

    void level::print_path(std::vector<int> path) { 
        for(int j = 0; j < rows_; ++j) {
            for(int i = 0; i < cols_; ++i) {
                if(data_[j * cols_ + i] < 0) {
                    data_[j * cols_ + i] = 0; 
                    continue;
                }
                if(std::find(path.begin(), path.end(), data_[j * cols_ + i]) != path.end() ) {
                    data_[j * cols_ + i] = 2; 
                    continue;
                } 
                data_[j * cols_ + i] = 1; 
            }
        }
        print();
    }

    void level::print() {
        std::cout << rows_ << " " << cols_ << std::endl;

        for(int j = 0; j < rows_; ++j) {
            for(int i = 0; i < cols_; ++i) {
                std::cout << data_[j * cols_ + i] << " ";
            }
            std::cout << std::endl;
        }
    }

    int level::get_begin() {
        return begin_;
    }

    int level::get_end() {
        return end_;
    }
};
