/*
 * Common.h
 *
 *  Created on: Oct 11, 2017
 *      Author: YNG
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <iostream>
#include <ctype.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include "error_functions.h"
#include <pwd.h>
#include <dirent.h>

#include<iterator>
#include <type_traits>
#include <string>
#include <algorithm>


/*
** A C++ Version of LINQ Where()
** Return a new container picking up only elements from a container that satisfy a certain criteria.
**
** This could run not as member function but as a wrapper function.
**
** You call this like
** std::vector<int> myVec{1,2,3,4,5};
** std::vector<int> myOddVec = Where(myVec, [](int i){return i%2 == 1;});
**
** No template parameters are needed in function call. All will be deducted.
*/
template<typename TContainer, typename TPred>
static inline TContainer Where(
        const TContainer& _container,
        TPred _pred // if _pred(element) is true, then this element will be selected
        )
{
    TContainer selected;
    std::copy_if(std::begin(_container), std::end(_container), std::inserter(selected, selected.end()), _pred);
    return selected;
}


/*
** A C++ Version of LINQ OrderBy()
** Return a new container which is ordered by the key from _func(element)
** This could run not as member function but as a wrapper function.
**
** You call this like
** std::vector<int> myVec{1,3,4,2,2};
** std::vector<int> myNegativeFloatVec = OrderBy(myVec, [](int i){return float(-i);});
**
** No template parameters are needed in function call. All will be deducted.
*/
template<typename TOriginContainer, typename TOrderFunc>
static inline TOriginContainer OrderBy
(
        const TOriginContainer& _origin,
        TOrderFunc _func //transform the element to get the key
)
{

    auto comparor = [&_func](
            typename TOriginContainer::value_type elem1,
            typename TOriginContainer::value_type elem2)
    {
        return _func(elem1) < _func(elem2);
    };

    auto ordered = _origin;

    std::sort(std::begin(ordered), std::end(ordered), comparor);

    return ordered;
}

/*
** A C++ Version of LINQ OrderByDescending()
** Return a new container which is ordered descending by the key calculated by _func(element)
** This could run not as member function but as a wrapper function.
**
** You call this like
** std::vector<int> myVec{1,3,4,2,2};
** std::vector<int> myInvFloatVec = OrderByDescending(myVec, [](int i){return float(-i);});
**
** No template parameters are needed in function call. All will be deducted.
*/
template<typename TOriginContainer, typename TSelectFunc>
static inline TOriginContainer OrderByDescending
(
        const TOriginContainer& _origin,
        TSelectFunc _func
)
{

    auto comparor = [&_func](
            typename TOriginContainer::value_type elem1,
            typename TOriginContainer::value_type elem2)
    {
        return _func(elem2) < _func(elem1);
    };

    auto ordered = _origin;

    std::sort(std::begin(ordered), std::end(ordered), comparor);

    return ordered;
}


/*
** A C++ Version of LINQ Select<TSelectedElement>()
**
** Return a new container with the elements transformed by TSelectFunc _func
** This could run not as member function but as a wrapper function.
**
** You call this like
** std::vector<int> myVec{1,3,4,2,2};
** std::vector<float> myInvFloatVec = Select<float>(myVec, [](int i){return 1.0f/i;});
**
** You need to specify the underlining element type in the type argument.
*/
template<
            typename TSelected,
            template<typename, typename> class TOriginContainer,
            typename TOriginElement,
            typename TOriginAllocator,
            typename TSelectFunc
        >
static inline TOriginContainer<TSelected, std::allocator<TSelected>> Select
(
        const TOriginContainer<TOriginElement, TOriginAllocator>& _origin,
        TSelectFunc _func
)
{
    TOriginContainer<TSelected, std::allocator<TSelected>> selected;
    auto inserter = std::inserter(selected, selected.end());
    std::transform(std::begin(_origin), std::end(_origin), inserter, _func);
    return selected;
}



//verifies if a class contains an element
template <class TContainer, class TVal>
static inline bool Contains(TContainer& container, const TVal& element)
{
    return std::find(std::begin(container), std::end(container), element) != container.end();
}

//for a string containing string test, specialize the template
template<>
inline bool Contains(std::string& parent, const std::string& sub_string)
{
    return parent.find(sub_string) != std::string::npos;
}

//for a string containing string test, specialize the template
inline bool Contains(std::string& parent, const char* sub_string)
{
    return parent.find(sub_string) != std::string::npos;
}


template <typename TContainer, typename TPre>
static bool All(TContainer& container, TPre pred)
{
    bool match = true;
    for(auto & elem : container)
    {
        if(!pred(elem))
            match = false;
    }
    return match;
}

template <typename TContainer, typename TPre>
static bool None(TContainer &container, TPre pred)
{
    bool match = true;
    for(auto & elem : container)
    {
        if(pred(elem))
            match = false;
    }
    return match;
}


#endif /* COMMON_H_ */
