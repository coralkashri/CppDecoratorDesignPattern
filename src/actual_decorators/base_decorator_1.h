#ifndef CPPDECORATORDESIGNPATTERN_BASE_DECORATOR_1_H
#define CPPDECORATORDESIGNPATTERN_BASE_DECORATOR_1_H

#include "../interface_core.h"
#include "../custom_concepts.h"

template <Decorator D = base_core>
class base_decorator_1 : virtual public D {
public:
    int decorator_param;
    float another_param;

    explicit base_decorator_1(const boost::property_tree::ptree &json) : D(json) {
        set_self_params(json);
    }

    virtual ~base_decorator_1() = default;

    void func() override {
        D::func();
        std::cout << "BaseDecoration1Func" << std::endl;
    }

    bool compare(const boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration1Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = D::compare(json) && json.get<int>("decorator_param") == decorator_param && json.get<float>("another_param") == another_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(const boost::property_tree::ptree &json) override {
        D::set_params(json);
        set_self_params(json);
    }

private:
    void set_self_params(const boost::property_tree::ptree &json) {
        decorator_param = json.get<int>("decorator_param", 0);
        another_param = json.get<float>("another_param", 0.0f);
    }
};

#endif //CPPDECORATORDESIGNPATTERN_BASE_DECORATOR_1_H