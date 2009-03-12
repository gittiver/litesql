/* LiteSQL 
 * 
 * By Tero Laitinen 
 * 
 * See LICENSE for copyright information. */

/** \file split.hpp
    contains class Split */
#ifndef _litesql_split_hpp
#define _litesql_split_hpp
#include <string>
#include <vector>
namespace litesql {
/** splits and joins strings. Modelled with Python's strings in mind. */
class Split : public std::vector<std::string> {
public:
    /** empty split */
    Split() {}
    /** from string vector */
    Split(std::vector<std::string> data) 
        : std::vector<std::string>(data) {}
    /** from string. Split to parts using delimeter */
    Split(const std::string& s, const std::string& delim=" ");
    /** returns a part of strings 
     \param start starting index
     \param end ending index
     Indexes can be negative; actual index is calculated from the end of 
     Split then.*/
    Split slice(int start, int end) const;
    /** returns strings joined with delimiter */
    std::string join(const std::string& delim) const;
    /** adds contents of another split to the end */
    Split & extend(const Split & s);
};
}
#endif
