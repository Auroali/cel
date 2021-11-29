#pragma once
#include <functional>
#include <string>
#include <cstddef>
#include <vector>
#include <memory>
#include <optional>

#define REFLECT() \
protected: \
virtual cel::reflection::type* get_type() { return &reflect_type_intern; } \
private: \
static cel::reflection::type reflect_type_intern; \
static void init_reflect(cel::reflection::type*); \
friend class cel::reflection::solver

#define REFLECT_DEFINE(Typename) \
cel::reflection::type Typename::reflect_type_intern(#Typename, Typename::init_reflect); \
void Typename::init_reflect(cel::reflection::type* r_type) { \
    using T = Typename; \
    r_type->size = sizeof(T); \
    r_type->fact = new cel::reflection::__factory<T>(); \
    r_type->members = {

#define REFLECT_MEMBER(member) \
        { #member, offsetof(T, member), sizeof(T::member), SERIALIZE },

#define REFLECT_MEMBER_WITH(member, attrib) \
        { #member, offsetof(T, member), sizeof(T::member), attrib },
        
#define REFLECT_END() \
    }; \
    cel::reflection::solver::_register(r_type); \
}

/**
 * Reflection Attributes
 */
#define SERIALIZE 0x01UL

/**
 * Namespace containing structures used for reflection
 */
namespace cel::reflection {
    class factory {
    public:
        virtual void* make() { return nullptr; }
        virtual std::shared_ptr<void> make_shared() { return std::shared_ptr<void>(); }
        virtual ~factory() {}
    };
    template<typename T>
    class __factory : public factory {
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
         * @brief Returns the member as type <T>
         * 
         * @tparam T the type of the member
         * 
         * @param ptr the pointer of the object this is a member of
         * @return T& the member as type <T> 
         */
        template<typename T>
        T& as(void* ptr) {
            return *((T*)ptr+offset); 
        }

        /**
         * @brief Sets the member to the value 'val'
         * 
         * @tparam T the type of the value
         * @param ptr the pointer of the object this is a member of
         * @param val the value to set this member to
         */
        template<typename T>
        void set(void* ptr, T&& val) {
            *((T*)ptr+offset) = val;
        }
        /**
        * @brief Returns the address of this member in memory
        * 
        * @param obj the pointer to the object this is a member of
        * @return void* a pointer to the member in memory 
        */
        char* ptr(void* obj) {
            return (char*)obj+offset;
        }
        /**
         * @brief Checks whether or not this member has the specified attribute(s)
         * 
         * @param attrib the attribute(s) to check
         * 
         * @return true if the member has the attribute(s)
         * @return false if the member does not have the attribute(s)
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
        * @brief Returns an optional holding the member entry specified by 'name'
        * 
        * @param name the name of the member
        * @return std::optional<member> an optional holding the member, if it exists
        */
        std::optional<member> get_member(const std::string& name);
        std::string name;
        size_t size;
        std::vector<member> members;
        factory* fact;
        ~type() {
            delete fact;
        }
    };
    /**
     * Utilities for getting reflection info
     * from various sources
     */
    class solver {
    private:
        static std::vector<type*> ref_types;
    public:
        /**
         * @brief Registers reflection type info
         * 
         * @note This is only intented to be used in the REFLECT_END macro
         * 
         * @param type pointer to the type info to register
         */
        static void _register(type* type);
        /**
         * @brief Gets reflection type info by class name
         * 
         * @param name the name of the class
         * @return cel::reflection::type* a pointer to the type info 
         */
        static type* get_by_str(const std::string& name);
        template<typename T>
        static type* get_from() {
            return &T::reflect_type_intern;
        }
        /**
         * @brief Gets the reflection type info from a pointer
         * 
         * @note The value pointed to by ptr must have reflection implemented
         *
         * @param ptr the pointer to get the type info from 
         * @return cel::reflection::type* a pointer to the type info 
         */
        template<typename T>
        static type* get_from(T* ptr) {
            return ptr->get_type();
        }
    };
}
