#ifndef EMBGEN_PARSER_EXCEPTIONS_HPP
#define EMBGEN_PARSER_EXCEPTIONS_HPP

#include <stdexcept>

#define NEW_EMB_EX(name)                                         \
    class name : public emb::gen::parser::ParserException          \
    {                                                            \
    public:                                                      \
        name(const std::string& message = "") :                  \
            emb::gen::parser::ParserException(#name ": " + message) \
        {                                                        \
        }                                                        \
    };                                                           \

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            class ParserException : public std::runtime_error
            {
            public:
                ParserException(const std::string& message) :
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