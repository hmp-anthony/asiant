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
    for(auto& boid : boids) {
        in_neighbourhood[i++] = false;
        if(boid == of) continue;
        if(boid->get_position().distance(of->get_position()) > size) continue;
        if(min_dot_product > -1.0) {
            vector offset = boid->get_position() - of->get_position();
            offset.normalize();
            if(look * offset < min_dot_product) continue;
        }
        in_neighbourhood[i-1] = true;
        count++;
    }
    return count;
};

vector flock::get_neighbourhood_center() {
    vector center;
    unsigned i = 0, count = 0;
    for(auto& boid : boids) {
        if(in_neighbourhood[i++]) {
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
    for(auto& boid : boids) {
        if(in_neighbourhood[i++]) {
            center += boid->get_velocity();
            count++;
        }
    }
    center *= 1 / (real) count;
    return center;
}

std::shared_ptr<steering> separation::get_steering() {
    auto no_steering = std::make_shared<steering>(); 
    auto count = the_flock->prepare_neighbourhood(character, 
                                                  neighbourhood_size,
                                                  neighbourhood_minimum_dot_product);
    if(count <= 0) return no_steering;

    auto center_of_mass = std::make_shared<vector>(the_flock->get_neighbourhood_center());
    
    flee_.set_max_acceleration(max_acceleration);
    flee_.set_character(character);
    flee_.set_target(center_of_mass);
    return flee_.get_steering();
}

std::shared_ptr<steering> cohesion::get_steering() {
    auto no_steering = std::make_shared<steering>(); 
    auto count = the_flock->prepare_neighbourhood(character, 
                                                  neighbourhood_size,
                                                  neighbourhood_minimum_dot_product);
    if(count <= 0) return no_steering;

    auto center_of_mass = std::make_shared<vector>(the_flock->get_neighbourhood_center());
    
    seek_.set_max_acceleration(max_acceleration);
    seek_.set_character(character);
    seek_.set_target(center_of_mass);
    return seek_.get_steering();
}

std::shared_ptr<steering> velocity_match_and_align::get_steering() {
    auto no_steering = std::make_shared<steering>(); 
    auto count = the_flock->prepare_neighbourhood(character, 
                                                  neighbourhood_size,
                                                  neighbourhood_minimum_dot_product);
    if(count <= 0) return no_steering;

    vector vel = the_flock->get_neighbourhood_average_velocity();

    auto steering_output = std::make_shared<steering>();

    auto output = vel - character->get_velocity();
    if(output.magnitude() > max_acceleration) {
        output.normalize();
        output *= max_acceleration;
    }

    steering_output->set_linear(output);
    return steering_output;
}

