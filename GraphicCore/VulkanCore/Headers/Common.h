#ifndef COMMON_H
#define COMMON_H

#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> mGraphicsFamily;
	std::optional<uint32_t> mPresentFamily;

	bool isComplete()
	{
		return mGraphicsFamily.has_value() && mPresentFamily.has_value();
	}
};

#endif // COMMON_H

