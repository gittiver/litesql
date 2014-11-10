#ifndef litesql_gen_protobuf_hpp
#define litesql_gen_protobuf_hpp

#include "generator.hpp"

namespace litesql
{
    class ProtoBufClassGenerator : public CodeGenerator
    {
    public:
        static const char* NAME;
        ProtoBufClassGenerator() :
                CodeGenerator(NAME)
        {
        }
        ;
        bool generateCode(const ObjectModel* model);
    protected:
        bool generate(const xml::ObjectPtr& object)
        {
            return false;
        }
        ;
    };
}

#endif

