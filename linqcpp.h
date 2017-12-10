/*
 * linqcpp.h
 *
 *  Created on: Dec 10, 2017
 *      Author: yng
 */

#ifndef LINQCPP_H_
#define LINQCPP_H_



#include <iterator>
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
TContainer Where(
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
TOriginContainer OrderBy
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
TOriginContainer OrderByDescending
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
** A C++ Version of LINQ Select(), more precisely called Transform here since select normally is used for selecting columns
**
** Return a new container with the elements transformed by TSelectFunc _func
** This could run not as member function but as a wrapper function.
**
** You call this like
** std::vector<int> myVec{1,3,4,2,2};
** auto myInvFloatVec = Select(myVec, [](int i){return 1.0f/i;});
**
** You don't need to specify the underlining element type in the type argument.
*/
template<
            template<typename, typename> class TOriginContainer,
            typename TOriginElement,
            typename TOriginAllocator,
            typename TTransformFunc,
            typename TTransformed = typename std::result_of<TTransformFunc(TOriginElement)>::type,
            typename TTransformedAlloc = std::allocator<TTransformed>
        >
auto Transform
(
    const TOriginContainer<TOriginElement, TOriginAllocator>& _origin,
    TTransformFunc _func
)
->TOriginContainer<TTransformed, TTransformedAlloc>
{
    TOriginContainer<TTransformed, TTransformedAlloc> transformed;
    auto inserter = std::inserter(transformed, transformed.end());
    std::transform(std::begin(_origin), std::end(_origin), inserter, _func);
    return transformed;
}



//verifies if a class contains an element
template <class TContainer, class TVal>
bool Contains(TContainer& container, const TVal& element)
{
    return std::find(std::begin(container), std::end(container), element) != container.end();
}

//for a string containing string test, specialize the template
inline bool Contains(std::string& parent, std::string sub_string)
{
    return parent.find(sub_string) != std::string::npos;
}


template <typename TContainer, typename TPre>
bool All(const TContainer& container, TPre pred)
{
	return std::all_of(std::begin(container), std::end(container), pred);
}

template <typename TContainer, typename TPre>
bool None(const TContainer &container, TPre pred)
{
	return std::none_of(std::begin(container), std::end(container), pred);
}

template <typename TContainer, typename TPre>
bool Any(const TContainer &container, TPre pred)
{
	return std::any_of(std::begin(container), std::end(container), pred);
}



template <typename TTarget = double, typename TContainer>
TTarget Mean(const TContainer &container)
{
	TTarget mean{};
	for (auto &elem : container)
	{
		mean += elem;
	}

	mean /= container.size();
	return mean;
}


template <typename TTarget = double, typename TContainer>
TTarget Variant(const TContainer &container)
{
	auto mean = Mean<TTarget>(container);
	TTarget variant{};
	for (auto &elem : container)
	{
		variant += (elem - mean) * (elem - mean);
	}

	variant /= container.size();
	return variant;
}


template <typename TTarget = double, typename TContainer>
TTarget Standard_Deviation(const TContainer &container)
{
	auto variant = Variant<TTarget>(container);
	TTarget standard_deviation = sqrt(variant);
	return standard_deviation;
}

#endif /* LINQCPP_H_ */
