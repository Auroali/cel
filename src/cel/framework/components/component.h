#pragma once
#include "../object.h"
#include <memory>

namespace cel
{

class component {
private:
    std::weak_ptr<object> parent;
    friend class object;
public:
    transform tans;
    std::string name;

    component();
};

} 