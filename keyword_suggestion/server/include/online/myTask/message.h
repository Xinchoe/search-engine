#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

namespace keyword_suggestion
{
struct Message
{
    int _id;
    int _len;
    /* std::string _content; */
    char _content[1024];
};
} //end of namespace keyword_suggestion
#endif
