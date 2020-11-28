#ifndef CPPDECORATORDESIGNPATTERN_CUSTOM_CONCEPTS_H
#define CPPDECORATORDESIGNPATTERN_CUSTOM_CONCEPTS_H

#include <type_traits>

template <class T>
concept Decorator = std::is_base_of_v<base_core, T>;

#endif //CPPDECORATORDESIGNPATTERN_CUSTOM_CONCEPTS_H