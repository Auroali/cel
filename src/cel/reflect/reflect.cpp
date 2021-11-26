#include "reflect.h"

std::vector<cel::reflection::type*> cel::reflection::solver::ref_types;

void cel::reflection::solver::_register(cel::reflection::type* type) {
    ref_types.push_back(type);
}
cel::reflection::type* cel::reflection::solver::get_by_str(const std::string& name) {
    for(cel::reflection::type* t : ref_types) {
        if(t->name == name)
            return t;
    }
    return nullptr;
}
std::optional<cel::reflection::member> cel::reflection::type::get_member(const std::string& name) {
    for(cel::reflection::member& member : members)
        if(member.name == name)
            return member;
    return {};
}