#pragma once
#include "../object.h"
#include <memory>
#include "cel/reflect/reflect.h"
#include <iostream>

#define REFLECT_COMPONENT() \
public: \
	virtual cel::reflection::type* get_type() override; \
private: \
REFLECT()


#define REFLECT_COMPONENT_DEFINE(Typename) \
cel::reflection::type* Typename::get_type() { \
	return &Typename::reflect_type_intern; \
} \
REFLECT_DEFINE(Typename) \
REFLECT_MEMBER(name) \
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
