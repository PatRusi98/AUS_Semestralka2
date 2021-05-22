#pragma once

template <typename R, typename T>
class Criteria {
public:
	virtual T rate(R, wstring) = 0;
};