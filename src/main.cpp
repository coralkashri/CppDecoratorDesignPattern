#include <iostream>
#include <tuple>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <type_traits>

class base {
public:
    virtual ~base() = default;

    virtual void func() = 0;
    virtual bool compare(boost::property_tree::ptree&) = 0;
    virtual void set_params(boost::property_tree::ptree &json) = 0;

protected:
    int base_element;
};

class base_core : virtual public base {
public:
    virtual ~base_core() = default;

    void func() override {
        std::cout << "BaseCoreFunc" << std::endl;
    }

    bool compare(boost::property_tree::ptree &json) override {
        std::cout << "BaseCoreCompare" << std::endl;
        bool is_equal;
        try {
            is_equal = json.get<int>("base_element") == base_element;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        base_element = json.get<int>("base_element", 0);
    }
};

/// <BEGIN><ENABLE_USING_ADVANCED_CORE_WITHOUT_SPECIFIED_BASE_CORE>
class empty_inheritance {};

template<typename Base = base_core, typename ...Decorators>
struct base_if_not_exists {
    static constexpr bool value = sizeof...(Decorators);
    using type = typename std::conditional<value, empty_inheritance, Base>::type;
};
/// <ENABLE_USING_ADVANCED_CORE_WITHOUT_SPECIFIED_BASE_CORE><END>

template <class T>
        concept Decorator = std::is_base_of_v<base_core, T>;

template <Decorator D = base_core>
class base_decoration_1 : virtual public D {
public:
    virtual ~base_decoration_1() = default;

    void func() override {
        std::cout << "BaseDecoration1Func" << std::endl;
    }

    bool compare(boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration1Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = D::compare(json) && json.get<int>("decorator_param") == decorator_param && json.get<float>("another_param") == another_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        D::set_params(json);
        decorator_param = json.get<int>("decorator_param", 0);
        another_param = json.get<float>("another_param", 0.0f);
    }

private:
    int decorator_param;
    float another_param;
};

template <Decorator D = base_core>
class base_decoration_2 : virtual public D {
public:
    virtual ~base_decoration_2() = default;

    void func() override {
        std::cout << "BaseDecoration2Func" << std::endl;
    }

    bool compare(boost::property_tree::ptree & json) override {
        std::cout << "BaseDecoration2Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = D::compare(json) && json.get<std::string>("my_special_param") == my_special_param && json.get<double>("another_special_param") == another_special_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        D::set_params(json);
        my_special_param = json.get<std::string>("my_special_param", "default");
        another_special_param = json.get<double>("another_special_param", 0.0);
    }

private:
    std::string my_special_param;
    double another_special_param;
};

template <Decorator ...Decorators>
class advanced_core : virtual public base_if_not_exists<base_core, Decorators...>::type, virtual public Decorators... {
public:
    virtual ~advanced_core() = default;

    void func() override {
        std::cout << "AdvancedCoreFunc" << std::endl;
    }

    bool compare(boost::property_tree::ptree &json) override {
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

    void set_params(boost::property_tree::ptree &json) override {
        if constexpr (!base_if_not_exists<base_core, Decorators...>::value) {
            base_core::set_params(json);
        }
        (Decorators::set_params(json), ...);
        advanced_param = json.get<std::string>("advanced_param", "default");
    }

private:
    std::string advanced_param;
};

class individual_class {

};

int main() {
    {
        std::shared_ptr<base> base = std::make_shared<advanced_core<>>();
        boost::property_tree::ptree json;
        json.put("base_element", 5);
        json.put("decorator_param", 8);
        json.put("another_param", 56.3f);
        json.put("my_special_param", "value");
        json.put("another_special_param", 5.23);
        json.put("advanced_param", "value");
        base->set_params(json);
//        json.put("my_special_param", "aa");
        //json.put("my_special_param", "value1");
        // std::shared_ptr<base> base = std::make_shared<advanced_core<>>(json);
        // std::shared_ptr<base> base = std::make_shared<advanced_core<individual_class>>(json); // Compilation error
        bool result = base->compare(json);
        std::cout << "\nComparison result: " << std::boolalpha << result << "\n\n";
        base->func();
    }
    return 0;
}