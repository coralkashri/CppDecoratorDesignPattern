#ifndef CPPDECORATORDESIGNPATTERN_INTERFACE_CORE_H
#define CPPDECORATORDESIGNPATTERN_INTERFACE_CORE_H

#include <iostream>
#include "interface.h"

class base_core : virtual public base {
public:
    explicit base_core(const boost::property_tree::ptree &json) : base(json) {}

    ~base_core() override = default;

    void func() override {
        std::cout << "BaseCoreFunc" << std::endl;
    }

    bool compare(const boost::property_tree::ptree &json) override {
        std::cout << "BaseCoreCompare" << std::endl;
        bool is_equal;
        try {
            is_equal = json.get<int>("base_element") == base_element;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(const boost::property_tree::ptree &json) override {
        base::set_params(json);
    }
};

#endif //CPPDECORATORDESIGNPATTERN_INTERFACE_CORE_H