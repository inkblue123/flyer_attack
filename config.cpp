#include "config.h"

//去除str字符串中的c字符
void removeAll(std::string &str,char c)
{
    std::string::iterator new_end = remove(str.begin(), str.end(),c);
    str.erase(new_end, str.end());
}
