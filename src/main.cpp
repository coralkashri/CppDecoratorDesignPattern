#include <iostream>
#include <memory>
#include <boost/property_tree/ptree.hpp>

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

template <class Decorator = base_core, typename = typename std::enable_if<std::is_base_of<base_core, Decorator>::value>::type>
class base_decoration_1 : virtual public Decorator {
public:
    virtual ~base_decoration_1() = default;

    void func() override {
        std::cout << "BaseDecoration1Func" << std::endl;
    }

    bool compare(boost::property_tree::ptree &json) override {
        std::cout << "BaseDecoration1Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = Decorator::compare(json) && json.get<int>("decorator_param") == decorator_param && json.get<float>("another_param") == another_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        Decorator::set_params(json);
        decorator_param = json.get<int>("decorator_param", 0);
        another_param = json.get<float>("another_param", 0.0f);
    }

private:
    int decorator_param;
    float another_param;
};

template <class Decorator = base_core, typename = typename std::enable_if<std::is_base_of<base_core, Decorator>::value>::type>
class base_decoration_2 : virtual public Decorator {
public:
    virtual ~base_decoration_2() = default;

    void func() override {
        std::cout << "BaseDecoration2Func" << std::endl;
    }

    bool compare(boost::property_tree::ptree & json) override {
        std::cout << "BaseDecoration2Compare" << std::endl;
        bool is_equal;
        try {
            is_equal = Decorator::compare(json) && json.get<std::string>("my_special_param") == my_special_param && json.get<double>("another_special_param") == another_special_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        Decorator::set_params(json);
        my_special_param = json.get<std::string>("my_special_param", "default");
        another_special_param = json.get<double>("another_special_param", 0.0);
    }

private:
    std::string my_special_param;
    double another_special_param;
};

template <class Decorator = base_core, typename = typename std::enable_if<std::is_base_of<base_core, Decorator>::value>::type>
class advanced_core : virtual public base_decoration_2<>, virtual public Decorator {
public:
    virtual ~advanced_core() = default;

    void func() override {
        std::cout << "AdvancedCoreFunc" << std::endl;
    }

    bool compare(boost::property_tree::ptree &json) override {
        std::cout << "AdvancedCoreCompare" << std::endl;
        bool is_equal;
        try {
            is_equal = base_decoration_2::compare(json) && Decorator::compare(json) && json.get<std::string>("advanced_param") == advanced_param;
        } catch (boost::property_tree::ptree_bad_path &e) {
            is_equal = false;
        }
        return is_equal;
    }

    void set_params(boost::property_tree::ptree &json) override {
        Decorator::set_params(json);
        base_decoration_2::set_params(json);
        advanced_param = json.get<std::string>("advanced_param", "default");
    }

private:
    std::string advanced_param;
};

class individual_class {

};

int main() {
    {
        std::shared_ptr<base> base = std::make_shared<advanced_core<base_decoration_1<>>>();
        boost::property_tree::ptree json;
        json.put("base_element", 5);
        json.put("decorator_param", 8);
        json.put("another_param", 56.3f);
        json.put("my_special_param", "value");
        json.put("another_special_param", 5.23);
        json.put("advanced_param", "value");
        base->set_params(json);
//        json.put("another_param", 51.3f);
        //json.put("my_special_param", "value1");
        // std::shared_ptr<base> base = std::make_shared<advanced_core<>>(json);
        // std::shared_ptr<base> base = std::make_shared<advanced_core<individual_class>>(json); // Compilation error
        bool result = base->compare(json);
        std::cout << "\nComparison result: " << std::boolalpha << result << "\n\n";
        base->func();
    }
    return 0;
}