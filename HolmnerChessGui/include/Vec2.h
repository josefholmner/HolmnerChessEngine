#pragma once

template<typename T>
class Vec2
{
public:
	Vec2() = default;
	Vec2(T inX, T inY) : x_{inX}, y_{inY} {}

	T x() const { return x_; }
	T y() const { return y_; }

private:
	T x_ = {};
	T y_ = {};
};
