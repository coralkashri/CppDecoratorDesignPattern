#ifndef CPPDECORATORDESIGNPATTERN_SUB_STRUCTURE_DECORATOR_H
#define CPPDECORATORDESIGNPATTERN_SUB_STRUCTURE_DECORATOR_H

#include "../interface_core.h"
#include "../custom_concepts.h"
#include "sub_structures/custom_data.h"

template <Decorator ...Decorators>
class sub_structure_decorator : virtual public base_if_not_exists<base_core, Decorators...>::type, virtual public Decorators... {
public:
    sub_structure_decorator()
    requires (!base_if_not_exists<base_core, Decorators...>::value)
            : base_core(boost::property_tree::ptree()) {
        set_self_params(boost::property_tree::ptree());
    }

    explicit sub_structure_decorator()
    requires base_if_not_exists<base_core, Decorators...>::value
            : Decorators(boost::property_tree::ptree())..., base_core(boost::property_tree::ptree()) {
        set_self_params(boost::property_tree::ptree());
    }

    virtual ~sub_structure_decorator() = default;

    void func() override {
        (Decorators::func(), ...);
        std::cout << "BaseDecoration1Func" << std::endl;
    }

    bool compare(const boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration1Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = (Decorators::compare(json) && ...) &&
                    json.get<int>("sub_structure_decorator_a") == data.a &&
                    json.get<int>("sub_structure_decorator_b") == data.b &&
                    json.get<std::string>("sub_structure_decorator_str") == data.str;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(const boost::property_tree::ptree &json) override {
        (Decorators::set_params(json), ...);
        set_self_params(json);
    }

    operator custom_data&() {
        return data;
    }

    sub_structure_decorator& operator=(const custom_data &c_data) {
        data = c_data;
        return *this;
    }

private:
    void set_self_params(const boost::property_tree::ptree &json) {
        data.a = json.get<int>("sub_structure_decorator_a", 0);
        data.b = json.get<int>("sub_structure_decorator_b", 0);
        data.str = json.get<std::string>("sub_structure_decorator_str", "my_string");
    }

    custom_data data;
};

#endif //CPPDECORATORDESIGNPATTERN_SUB_STRUCTURE_DECORATOR_H