#ifndef CPPDECORATORDESIGNPATTERN_BASE_DECORATOR_2_H
#define CPPDECORATORDESIGNPATTERN_BASE_DECORATOR_2_H

#include "../interface_core.h"
#include "../custom_concepts.h"

template <Decorator D = base_core>
class base_decorator_2 : virtual public D {
public:
    std::string my_special_param;
    double another_special_param;

    explicit base_decorator_2(const boost::property_tree::ptree &json) : D(json) {
        set_self_params(json);
    }

    virtual ~base_decorator_2() = default;

    void func() override {
        D::func();
        std::cout << "BaseDecoration2Func" << std::endl;
    }

    bool compare(const boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration2Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = D::compare(json) && json.get<std::string>("my_special_param") == my_special_param && json.get<double>("another_special_param") == another_special_param;
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
        my_special_param = json.get<std::string>("my_special_param", "default");
        another_special_param = json.get<double>("another_special_param", 0.0);
    }
};

#endif //CPPDECORATORDESIGNPATTERN_BASE_DECORATOR_2_H