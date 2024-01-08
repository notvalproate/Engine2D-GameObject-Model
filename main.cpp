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

    if(tempCountersCountComponent) {
        std::cout << tempCountersCountComponent.value()->count << std::endl;
    }

    // auto objects = GameObject::FindObjectsByTag("Counter");

    // for(const auto& object : objects) {
    //     std::cout << object.get().tag << std::endl;
    // }
    
    // auto counterObject = GameObject::FindObjectByName("Counter");

    // std::cout << counterObject.name << std::endl;

    return 0;
}