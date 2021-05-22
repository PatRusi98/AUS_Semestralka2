#pragma once
#include "Criteria.h"

template <typename O, typename T, typename B>
class Filter {
public:
	virtual bool meetsFilter(O o, T t, B b) = 0;
};