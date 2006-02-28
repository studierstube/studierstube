/*
** string.h
** 
** Made by Bernhard Reitinger
** Login   <breiting@localhost.localdomain>
** 
** Started on  Thu Feb  9 11:33:07 2006 Bernhard Reitinger
** Last update Thu Feb  9 11:33:07 2006 Bernhard Reitinger
*/

/**
   \brief This file encapsulates basic data structures like string,
   list, and so on. For the PDA-based setup, STL cannot be used.
*/
#include <stb/base/string.h>

BEGIN_NAMESPACE_STB

int stricasecmp(const stb::string str1, const stb::string str2)
{
#ifdef WIN32
    return stricmp(str1.c_str(),str2.c_str());
#else //LINUX
    #include <strings.h>
    return strcasecmp(str1.c_str(),str2.c_str());
#endif
    return -1;
}
///////////////// std::strcasecmp

END_NAMESPACE_STB


//========================================================================
// End of string.cxx
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================