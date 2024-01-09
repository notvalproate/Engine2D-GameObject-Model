#include "Core.hpp"

class CounterComponent : public Behavior {
public:
    using Behavior::Behavior;

    void Start() override {
        std::cout << "Called Counter Start" << std::endl;
    }

    void Update() override {
        count++;
        std::cout << "Count is " << count << std::endl;
    }

    uint64_t count{};
};

class CounterDisabler : public Behavior {
public:
    using Behavior::Behavior;

    void Start() override {
        m_CounterComponent = gameObject.GetComponent<CounterComponent>();
    }

    void Update() override {
        if(m_CounterComponent->enabled && m_CounterComponent->count == 5) {
            m_CounterComponent->enabled = false;
        } 
        
        if(!m_CounterComponent->enabled){
            localCount++;
            std::cout << "Local Count is " << localCount << std::endl; 
        }

        if(localCount == 3) {
            m_CounterComponent->enabled = true;
        }
    }

private:
    CounterComponent* m_CounterComponent;
    int localCount{0};
};

int main() {
    GameObject tempCounter("Counter");
    tempCounter.AddComponent<CounterComponent>();
    tempCounter.AddComponent<CounterDisabler>();
    tempCounter.tag = "Counter";

    for(int i = 0; i < 10; i++) {
        tempCounter.HandleEvents();
        tempCounter.Update();
        tempCounter.Render();
    }

    auto tempCountersCountComponent = tempCounter.GetComponent<CounterComponent>();

    if(tempCountersCountComponent) {
        std::cout << tempCountersCountComponent->count << std::endl;
    }

    auto counterObject = GameObject::FindObjectByName("Counter");

    std::cout << counterObject->name << std::endl;

    delete counterObject;

    auto objects = GameObject::FindObjectsByTag("Counter");

    for(const auto& object : objects) {
        std::cout << object->name << std::endl;
    }

    return 0;
}