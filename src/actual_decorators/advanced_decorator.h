#ifndef CPPDECORATORDESIGNPATTERN_ADVANCED_DECORATOR_H
#define CPPDECORATORDESIGNPATTERN_ADVANCED_DECORATOR_H

#include "../interface_core.h"
#include "../custom_concepts.h"
#include "../helper_base_if_not_exists.h"

template <Decorator ...Decorators>
class advanced_core : virtual public base_if_not_exists<base_core, Decorators...>::type, virtual public Decorators... {
public:
    std::string advanced_param;

    explicit advanced_core(const boost::property_tree::ptree &json)
    requires (!base_if_not_exists<base_core, Decorators...>::value)
            : base_core(json) {
        set_self_params(json);
    }

    explicit advanced_core(const boost::property_tree::ptree &json)
    requires base_if_not_exists<base_core, Decorators...>::value
            : Decorators(json)..., base_core(json) {
        set_self_params(json);
    }

    ~advanced_core() override = default;

    void func() override {
        (Decorators::func(), ...);
        std::cout << "AdvancedCoreFunc" << std::endl;
        if constexpr (!base_if_not_exists<base_core, Decorators...>::value) {
            base_core::func();
        }
    }

    bool compare(const boost::property_tree::ptree &json) override {
        std::cout << "AdvancedCoreCompare" << std::endl;
        bool is_equal = true;
        try {
            if constexpr (!base_if_not_exists<base_core, Decorators...>::value) {
                is_equal = base_core::compare(json);
            }
            is_equal = is_equal && (Decorators::compare(json) && ...) && json.get<std::string>("advanced_param") == advanced_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(const boost::property_tree::ptree &json) override {
        if constexpr (!base_if_not_exists<base_core, Decorators...>::value) {
            base_core::set_params(json);
        }
        (Decorators::set_params(json), ...);
        set_self_params(json);
    }

private:
    void set_self_params(const boost::property_tree::ptree &json) {
        advanced_param = json.get<std::string>("advanced_param", "default");
    }
};

#endif //CPPDECORATORDESIGNPATTERN_ADVANCED_DECORATOR_H