#pragma once
#include "Criteria.h"

template <typename O, typename T>
class Filter {
public:
	virtual bool meetsFilter(O o, T t) = 0;
};