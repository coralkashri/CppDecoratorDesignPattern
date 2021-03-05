#ifndef CPPDECORATORDESIGNPATTERN_BASIC_INTERFACE_H
#define CPPDECORATORDESIGNPATTERN_BASIC_INTERFACE_H

#include <boost/property_tree/ptree.hpp>

class base {
public:
    explicit base() : base_element(0) {}
    explicit base(const boost::property_tree::ptree &json) {
        set_self_params(json);
    }

    virtual ~base() = default;

    virtual void func() = 0;
    virtual bool compare(const boost::property_tree::ptree&) = 0;
    virtual void set_params(const boost::property_tree::ptree &json) {
        set_self_params(json);
    };

protected:
    int base_element;

private:
    void set_self_params(const boost::property_tree::ptree &json) {
        base_element = json.get<int>("base_element", 0);
    }
};

#endif //CPPDECORATORDESIGNPATTERN_BASIC_INTERFACE_H