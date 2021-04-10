#pragma once

#include <cstdint>
#include <cstddef>

struct ImageData
{
	ImageData(const uint8_t* inData, size_t inLength) : data{inData}, length{inLength}
	{
	}

	const uint8_t* data;
	size_t length;
};
