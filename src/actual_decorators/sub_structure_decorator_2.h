#ifndef CPPDECORATORDESIGNPATTERN_SUB_STRUCTURE_DECORATOR_2_H
#define CPPDECORATORDESIGNPATTERN_SUB_STRUCTURE_DECORATOR_2_H

#include "../interface_core.h"
#include "../custom_concepts.h"
#include "sub_structures/custom_data_2.h"

template <Decorator D = base_core>
class sub_structure_decorator_2 : virtual public D {
public:
    sub_structure_decorator_2() : D(boost::property_tree::ptree()) {
        set_self_params(boost::property_tree::ptree());
    }

    explicit sub_structure_decorator_2(const boost::property_tree::ptree &json) : D(json) {
        set_self_params(json);
    }

    virtual ~sub_structure_decorator_2() = default;

    void func() override {
        D::func();
        std::cout << "BaseDecoration1Func" << std::endl;
    }

    bool compare(const boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration1Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = D::compare(json) &&
                    json.get<std::string>("sub_structure_decorator_2_str") == data.my_str;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(const boost::property_tree::ptree &json) override {
        D::set_params(json);
        set_self_params(json);
    }

    operator custom_data_2&() {
        return data;
    }

    sub_structure_decorator_2& operator=(const custom_data_2 &c_data) {
        data = c_data;
        return *this;
    }

private:
    void set_self_params(const boost::property_tree::ptree &json) {
        data.my_str = json.get<std::string>("sub_structure_decorator_2_str", "my_string");
    }

    custom_data_2 data;
};

#endif //CPPDECORATORDESIGNPATTERN_SUB_STRUCTURE_DECORATOR_2_H