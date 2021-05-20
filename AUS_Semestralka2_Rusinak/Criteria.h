#pragma once

template <typename R, typename T>
class Criteria {
public:
	virtual T rate(R) const = 0;
};