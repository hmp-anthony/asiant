#include "flock_steer.hpp"

unsigned flock::prepare_neighbourhood(const std::shared_ptr<kinematic> of,
                                      real size,
                                      real min_dot_product) {
    vector look;
    if(min_dot_product > -1.0f) {
        look = of->get_orientation_as_vector();
    }

    flock result;
    unsigned i = 0, count = 0;
    for(auto& boid : boids_) {
        in_neighbourhood_[i++] = false;
        if(boid == of) continue;
        if(boid->get_position().distance(of->get_position()) > size) continue;
        if(min_dot_product > -1.0) {
            vector offset = boid->get_position() - of->get_position();
            offset.normalize();
            if(look * offset < min_dot_product) continue;
        }
        in_neighbourhood_[i-1] = true;
        count++;
    }
    return count;
};

vector flock::get_neighbourhood_center() {
    vector center;
    unsigned i = 0, count = 0;
    for(auto& boid : boids_) {
        if(in_neighbourhood_[i++]) {
            center += boid->get_position();
            count++;
        }
    }
    center *= 1 / (real) count;
    return center;
}

vector flock::get_neighbourhood_average_velocity() {
    vector center;
    unsigned i = 0, count = 0;
    for(auto& boid : boids_) {
        if(in_neighbourhood_[i++]) {
            center += boid->get_velocity();
            count++;
        }
    }
    center *= 1 / (real) count;
    return center;
}

void separation::get_steering(std::shared_ptr<steering> steer) {
    auto count = the_flock_->prepare_neighbourhood(character_, 
                                                  neighbourhood_size_,
                                                  neighbourhood_minimum_dot_product_);
    if(count <= 0) return;
    auto center_of_mass = std::make_shared<vector>(the_flock_->get_neighbourhood_center());
    
    flee_.set_max_acceleration(max_acceleration_);
    flee_.set_character(character_);
    flee_.set_target(center_of_mass);
    flee_.get_steering(steer);
}

void cohesion::get_steering(std::shared_ptr<steering> steer) {
    auto count = the_flock_->prepare_neighbourhood(character_, 
                                                  neighbourhood_size_,
                                                  neighbourhood_minimum_dot_product_);
    if(count <= 0) return;

    auto center_of_mass = std::make_shared<vector>(the_flock_->get_neighbourhood_center());
    
    seek_.set_max_acceleration(max_acceleration_);
    seek_.set_character(character_);
    seek_.set_target(center_of_mass);
    seek_.get_steering(steer);
}

void velocity_match_and_align::get_steering(std::shared_ptr<steering> steer) {
    auto count = the_flock_->prepare_neighbourhood(character_, 
                                                  neighbourhood_size_,
                                                  neighbourhood_minimum_dot_product_);
    if(count <= 0) return;

    vector vel = the_flock_->get_neighbourhood_average_velocity();
    auto output = vel - character_->get_velocity();
    if(output.magnitude() > max_acceleration_) {
        output.normalize();
        output *= max_acceleration_;
    }
    steer->set_linear(output);
}

