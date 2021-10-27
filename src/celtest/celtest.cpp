#include "celtest.h"
#include <cel/reflect/reflect.h>

cel::project_builder<proj> proj::builder = cel::project_builder<proj>("Test Project");

REFLECT_COMPONENT_DEFINE(test_comp)
REFLECT_MEMBER(test)
REFLECT_MEMBER(test2)
REFLECT_END()
// proj::builder 