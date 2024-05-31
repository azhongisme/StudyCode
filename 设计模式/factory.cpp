#include <iostream>
#include <memory>

class AbstractSmile {
public:
    virtual void transform(){}
    virtual void ability(){}
    virtual ~AbstractSmile(){}
};

class Sheep : public AbstractSmile {
public:
    void transform() override {
        std::cout<< "transform to sheep\n";
    }
    void ability() override {
        std::cout<< "the ability of sheep\n";
    }
};

class Lion : public AbstractSmile {
public:
    void transform() override {
        std::cout<< "transform to lion \n";
    }
    void ability() override {
        std::cout<< "the ability of lion \n";
    }
};

class Bat : public AbstractSmile {
public:
    void transform() override {
        std::cout<< "transform to bat \n";
    }
    void ability() override {
        std::cout<< "the ability of bat \n";
    }
};

enum class Type: char{SHEEP, LION, BAT};

class SmileFactory {
public:
    SmileFactory(){}
    ~SmileFactory(){}
    std::unique_ptr<AbstractSmile> createSmile(Type type) {
        std::unique_ptr<AbstractSmile>ptr;
        switch (type)
        {
            case Type::SHEEP:
                ptr = std::make_unique<Sheep>();
                break;
            case Type::BAT:
                ptr = std::make_unique<Bat>();
                break;
            case Type::LION:
                ptr = std::make_unique<Lion>();
                break;
            default:
                break;
        }
        return ptr;
    }
};

int main()
{
    SmileFactory* factory = new SmileFactory;
    std::unique_ptr<AbstractSmile> obj = factory->createSmile(Type::BAT);
    obj->transform();
    obj->ability();
    return 0;
}
