#pragma once
#include "../object.h"
#include <memory>
#include "../../reflect/reflect.h"
#include <iostream>

#define REFLECT_COMPONENT_DEFINE(Typename) \
REFLECT_DEFINE(Typename) \
REFLECT_MEMBER_WITH(name, 0) \
REFLECT_MEMBER_WITH(trans, SERIALIZE)

namespace cel
{

class component {
private:
    std::weak_ptr<object> parent;
    friend class object;
public:
    transform trans;
    std::string name;
	virtual cel::reflection::type* get_type() { return nullptr; }
    component();
    virtual ~component() {}
};

} 
