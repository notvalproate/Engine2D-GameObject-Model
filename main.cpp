#include "Core.hpp"

class CounterComponent : public Component {
public:
    using Component::Component;

    void Start() override {
        std::cout << "Called Counter Start" << std::endl;
    }

    void Update() override {
        count++;
        std::cout << "Count is " << count << std::endl;
    }

    uint64_t count{};
};

int main() {
   

    return 0;
}