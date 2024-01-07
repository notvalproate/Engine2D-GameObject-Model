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
    GameObject tempCounter("Counter");
    tempCounter.AddComponent<CounterComponent>();

    for(int i = 0; i < 10; i++) {
        tempCounter.HandleEvents();
        tempCounter.Update();
        tempCounter.Render();
    }

    auto tempCountersCountComponent = tempCounter.GetComponent<CounterComponent>();

    std::cout << tempCountersCountComponent->count << std::endl;

    auto objects = GameObject::FindObjectsByTag("Counter");

    for(const auto& object : objects) {
        std::cout << object->tag << std::endl;
    }

    auto counterObject = GameObject::FindObjectByName("Counters");

    if(counterObject) {
        std::cout << counterObject.value()->name << std::endl;
    } else {
        std::cout << "Could not find object" << std::endl;
    }

    return 0;
}