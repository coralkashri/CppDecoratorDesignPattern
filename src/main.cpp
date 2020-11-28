#include <iostream>
#include <memory>
#include "actual_decorators/advanced_decorator.h"
#include "actual_decorators/base_decorator_1.h"
#include "actual_decorators/base_decorator_2.h"

class individual_class {};

int main() {
    {
        boost::property_tree::ptree json;
        json.put("base_element", 5); // base
        json.put("decorator_param", 8); // base_decorator_1
        json.put("another_param", 56.3f); // base_decorator_1
        json.put("my_special_param", "value"); // base_decoration_2
        json.put("another_special_param", 5.23); // base_decoration_2
        json.put("advanced_param", "value"); // advanced_core

        std::shared_ptr<base> b = std::make_shared<advanced_core<base_decoration_2<>, base_decorator_1<>>>(json);
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
        json.put("my_special_param", "value1"); // base_decoration_2
        result = b->compare(json);
        std::cout << "\nComparison result: " << std::boolalpha << result << "\n\n";
        assert(!result);
    }
    return 0;
}