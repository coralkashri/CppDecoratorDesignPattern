#ifndef CPPDECORATORDESIGNPATTERN_HELPER_BASE_IF_NOT_EXISTS_H
#define CPPDECORATORDESIGNPATTERN_HELPER_BASE_IF_NOT_EXISTS_H

#include <type_traits>
#include "interface_core.h"

class empty_inheritance {};

template<typename Base = base_core, typename ...Decorators>
struct base_if_not_exists {
    static constexpr bool value = sizeof...(Decorators);
    using type = typename std::conditional<value, empty_inheritance, Base>::type;
};

#endif //CPPDECORATORDESIGNPATTERN_HELPER_BASE_IF_NOT_EXISTS_H