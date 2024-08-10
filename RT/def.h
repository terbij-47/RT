#include "mth/mth.h"

#ifndef __def_h_
#define __def_h_

/* Debug memory allocation support */ 
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy 
{ 
public: 
  /* Class constructor */
  __Dummy( VOID ) 
  { 
    SetDbgMemHooks(); 
  } /* End of '__Dummy' constructor */
} __ooppss;

#endif /* _DEBUG */ 
// #include <vector>
// #include <algorithm>

template<typename Type>
class stock : public std::vector<Type>
{
public:

  /* Push back to stock function.
   * ARGUMENTS:
   *   - element to add:
   *       const Type &x;
   * RETURNS:
   *   (stock &) reference to stock.
   */
  stock & operator<<(const Type &x)
  {
    (*this).push_back(x);
    return *this;
  } /* End of 'operator<<' function. */

  /* Walk around stock and use every element.
   * ARGUMENTS:
   *   - type of function:
   *       Walk_type Walk;
   * RETURNS: None.
   */
  template<typename Walk_type>
  VOID Walk( Walk_type Walk )
  {
    for (auto &x : *this)
      Walk(x);
  } /* End of 'Walk' function. */

};

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

#endif /* __def_h_ */

