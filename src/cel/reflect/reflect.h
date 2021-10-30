#pragma once
#include <functional>
#include <string>
#include <cstddef>
#include <vector>
#include <memory>

#define REFLECT() \
private: \
static cel::reflection::type reflect_type_intern; \
static void init_reflect(cel::reflection::type*); \
friend class cel::reflection::solver

#define REFLECT_DEFINE(Typename) \
cel::reflection::type Typename::reflect_type_intern(#Typename, Typename::init_reflect); \
void Typename::init_reflect(cel::reflection::type* r_type) { \
    using T = Typename; \
    r_type->size = sizeof(T); \
    r_type->factory = new cel::reflection::factory<T>(); \
    r_type->members = {

#define REFLECT_MEMBER(member) \
        {#member, offsetof(T, member), sizeof(T::member), size_t(0)},

#define REFLECT_MEMBER_WITH(member, attrib) \
        {#member, offsetof(T, member), sizeof(T::member), size_t(attrib)},
        
#define REFLECT_END() \
    }; \
    cel::reflection::solver::_register(r_type); \
}

/**
 * Reflection Attributes
 */
#define NOSAVE 0x01UL

/**
 * Namespace containing structures used for reflection
 */
namespace cel::reflection {
class _factory {
public:
    virtual void* make() { return nullptr; }
    virtual std::shared_ptr<void> make_shared() { return std::shared_ptr<void>(); }
    virtual ~_factory() {}
};
template<typename T>
class factory : public _factory {
public:
    // It's assumed you will know a base type to cast this to
    virtual void* make() override { return new T; }
    virtual std::shared_ptr<void> make_shared() override {
        return std::static_pointer_cast<void>(std::make_shared<T>());
    }
};
struct member {
    std::string name;
    size_t offset;
    size_t size;
    size_t attribs;
    
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
    /**
     * Checks whether or not this member has the specified attribute
     * 
     * @return whether or not it contains the attribute
     */
    bool has_attrib(size_t attrib) {
        return attribs & attrib;
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
    _factory* factory;
    ~type() {
        delete factory;
    }
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
