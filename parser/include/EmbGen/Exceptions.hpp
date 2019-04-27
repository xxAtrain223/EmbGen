#ifndef EMBGEN_PARSER_EXCEPTIONS_HPP
#define EMBGEN_PARSER_EXCEPTIONS_HPP

#include <stdexcept>

#define NEW_EMB_EX(name)                                         \
    class name : public emb::gen::parser::BaseException          \
    {                                                            \
    public:                                                      \
        name(const std::string& message = "") :                  \
            emb::gen::parser::BaseException(#name ":" + message) \
        {                                                        \
        }                                                        \
    };                                                           \

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class BaseException : public std::runtime_error
            {
            public:
                BaseException(const std::string& message) :
                    std::runtime_error(message)
                {
                }
            };

            NEW_EMB_EX(AttributeException);
            NEW_EMB_EX(ElementException);
        }
    }
}

#endif // EMBGEN_PARSER_EXCEPTIONS_HPP