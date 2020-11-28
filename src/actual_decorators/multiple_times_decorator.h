#ifndef CPPDECORATORDESIGNPATTERN_MULTIPLE_TIMES_DECORATOR_H
#define CPPDECORATORDESIGNPATTERN_MULTIPLE_TIMES_DECORATOR_H

#include "../interface_core.h"
#include "../custom_concepts.h"

template <size_t ID, Decorator D = base_core>
class multiple_times_decorator : virtual public D {
public:
    int mtd_param;

    explicit multiple_times_decorator(boost::property_tree::ptree &json) : D(json) {
        set_self_params(json);
    }

    virtual ~multiple_times_decorator() = default;

    void func() override {
        D::func();
        std::cout << "BaseDecoration1Func" << std::endl;
    }

    bool compare(boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration1Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = D::compare(json) && self_compare(json);
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        D::set_params(json);
        set_self_params(json);
    }

private:
    void set_self_params(boost::property_tree::ptree &json) {
        mtd_param = json.get<int>("mtd_" + std::to_string(ID) + "_param", 0);
    }

    bool self_compare(boost::property_tree::ptree &json) {
        return json.get<int>("mtd_" + std::to_string(ID) + "_param") == mtd_param;
    }
};

#endif //CPPDECORATORDESIGNPATTERN_MULTIPLE_TIMES_DECORATOR_H