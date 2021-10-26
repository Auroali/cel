#pragma once
#include <functional>
#include <string>
#include <cstddef>
#include <vector>

#define REFLECT() \
private: \
static cel::reflection::type reflect_type_intern; \
static void init_reflect(cel::reflection::type*); \
friend class cel::reflection::solver

#define RELFECT_DEFINE(Typename) \
cel::reflection::type Typename::reflect_type_intern(#Typename, Typename::init_reflect); \
void Typename::init_reflect(cel::reflection::type* r_type) { \
    using T = Typename; \
    r_type->size = sizeof(T); \
    r_type->members = {

#define REFLECT_MEMBER(member) \
        {#member, offsetof(T, member), sizeof(T::member)},
        
#define REFLECT_END() \
    }; \
    cel::reflection::solver::_register(r_type); \
}

namespace cel::reflection {
struct member {
    std::string name;
    size_t offset;
    size_t size;
    ~member() {}
    /**
     * Returns the member as type <T>
     * 
     * @param ptr the pointer of the object this is a member of
     * @return the member as type <T> 
     */
    template<typename T>
    T& as(void* ptr) {
        return *((T*)ptr+offset); 
    }
    /**
    * Returns the address of this member in memory
    * 
    * @param obj the pointer to the object this is a member of
    * @return a void* to the member in memory 
    */
    char* ptr(void* obj) {
        return (char*)obj+offset;
    }
};
struct type {
    type(std::string tname, std::function<void(type*)> func) {
        this->name = tname;
        func(this);
    }
    /**
    * Returns a pointer to the member entry specified by 'name'
    * 
    * @param name the name of the member
    * @return a pointer to the member entry, nullptr if it does not exist
    */
    member* get_member(const std::string& name);
    std::string name;
    size_t size;
    std::vector<member> members;
};

class solver {
private:
    static std::vector<type*> ref_types;
public:
    static void _register(type* type);
    static type* get_by_str(const std::string& name);
    template<typename T>
    static type* get_from() {
        return &T::reflect_type_intern;
    }
    template<typename T>
    static type* get_from(T* ptr) {
        return &T::reflect_type_intern;
    }
};
}