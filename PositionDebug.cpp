#include "Core.hpp"

class PositionDebug : public Behaviour {
public:
    using Behaviour::Behaviour;

    void Update() override {
        std::cout << tag << " " << transform.position.x << std::endl;
    }
};