#include <asiant/asiant.hpp>

namespace asiant {
    std::shared_ptr<steering> seek::get_steering(){
        auto output = std::make_shared<steering>();
        auto linear = *target - character->get_position();
        output->set_linear(linear);
        return output;
    }

    std::shared_ptr<steering> flee::get_steering(){
        auto output = std::make_shared<steering>();
        auto linear = character->get_position() - *target;
        output->set_linear(linear);
        return output;
    }
}
