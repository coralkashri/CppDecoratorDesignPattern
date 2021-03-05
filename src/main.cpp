#include <iostream>
#include <memory>
#include "actual_decorators/advanced_decorator.h"
#include "actual_decorators/base_decorator_1.h"
#include "actual_decorators/base_decorator_2.h"
#include "actual_decorators/multiple_times_decorator.h"
#include "actual_decorators/sub_structure_decorator.h"
#include "actual_decorators/sub_structure_decorator_2.h"
#include "actual_decorators/sub_structure_decorator_3.h"

class individual_class {};

void sub_structure_decorator_test() {
    sub_structure_decorator<sub_structure_decorator_2<>, sub_structure_decorator_3<>> decorator;
    boost::property_tree::ptree json;
    json.put("sub_structure_decorator_a", 16); // base
    json.put("sub_structure_decorator_b", 50); // base_decorator_1
    json.put("sub_structure_decorator_str", "My Custom String for decorator"); // base_decorator_1
    decorator.set_params(json);
    ((custom_data_2&)(sub_structure_decorator_2<>&)(decorator)).my_str = "My str";
    std::cout << ((custom_data_2&)(sub_structure_decorator_2<>&)(decorator)).my_str << std::endl; // Prints "My str"
    std::cout << ((custom_data_2&)(sub_structure_decorator_3<>&)(decorator)).my_str << std::endl; // Prints deault value ("my_string")
    std::cout << ((custom_data&)(decorator)).str << std::endl; // Prints "My Custom String for decorator"
}

void comparison_test(boost::property_tree::ptree json) {
    std::shared_ptr<base> b = std::make_shared<advanced_core<base_decorator_2<>, base_decorator_1<>>>(json);
    //std::shared_ptr<base> b2 = std::make_shared<advanced_core<individual_class>>(json); // Compilation error
    std::shared_ptr<base> b1 = std::make_shared<advanced_core<>>(json);

    std::cout << "~~~~~~~~~~~~~~~~~~~~ b->set_params(json): ~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    b->set_params(json);
    std::cout << "~~~~~~~~~~~~~~~~~~~~~ b->compare(json): ~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    bool result = b->compare(json);
    std::cout << "\nComparison result: " << std::boolalpha << result << "\n\n";
    assert(result);

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~ b->func(): ~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    b->func();
    std::cout << "\n" << std::endl;

    std::cout << "~~~~~~~~~~~~~~ modify json && b->compare(json): ~~~~~~~~~~~~~~~~~" << std::endl;
    json.put("my_special_param", "value1"); // base_decorator_2
    result = b->compare(json);
    std::cout << "\nComparison result: " << std::boolalpha << result << "\n\n";
    assert(!result);
}

void access_fields(boost::property_tree::ptree json) {
    std::shared_ptr<base> b = std::make_shared<advanced_core<base_decorator_2<>, base_decorator_1<>>>(json);
    auto access1 = dynamic_cast<base_decorator_1<>*>(b.get());
    auto access2 = dynamic_cast<base_decorator_2<>*>(b.get());
    auto access3 = dynamic_cast<advanced_core<base_decorator_2<>, base_decorator_1<>>*>(b.get());
    std::cout << access1->decorator_param << " " << access1->another_param << std::endl;
    std::cout << access2->another_special_param << " " << access2->my_special_param << std::endl;
    std::cout << access3->decorator_param << " " << access3->another_param << " " << access3->another_special_param << " " << access3->my_special_param << " " << access3->advanced_param << std::endl;

    boost::property_tree::ptree json1;
    json1.put("advanced_param", "advanced_core_value");
    json1.put("mtd_1_param", 5);
    json1.put("mtd_2_param", 10);
    std::shared_ptr<base> c = std::make_shared<advanced_core<multiple_times_decorator<1>, multiple_times_decorator<2>>>(json1);
    auto access11 = dynamic_cast<multiple_times_decorator<1>*>(c.get());
    auto access12 = dynamic_cast<multiple_times_decorator<2>*>(c.get());
    auto access13 = dynamic_cast<advanced_core<multiple_times_decorator<1>, multiple_times_decorator<2>>*>(c.get());
    std::cout << access11->mtd_param << std::endl;
    std::cout << access12->mtd_param << std::endl;
    std::cout << access13->multiple_times_decorator<1>::mtd_param << " " << access13->multiple_times_decorator<2>::mtd_param << " " << access13->advanced_param << std::endl;
}

int main() {
    {
        boost::property_tree::ptree json;
        json.put("base_element", 5); // base
        json.put("decorator_param", 8); // base_decorator_1
        json.put("another_param", 56.3f); // base_decorator_1
        json.put("my_special_param", "value"); // base_decorator_2
        json.put("another_special_param", 5.23); // base_decorator_2
        json.put("advanced_param", "value"); // advanced_core

        // comparison_test(json);

        // access_fields(json);

        sub_structure_decorator_test();

    }
    return 0;
}