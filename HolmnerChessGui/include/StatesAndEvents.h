#pragma once

#include <cstdint>

class Window;

namespace statesAndEvents
{
	enum class GameState
	{
		Menu,
		Play
	};

	enum class DifficultyLevel
	{
		Hard,
		Medium,
		Easy,
		Silly
	};

	enum class PlayingSide
	{
		White,
		Black,
		None
	};

	enum class EventType
	{
		None,
		Close,
		MouseDown,
		MouseRelease
	};

	struct Event
	{
		EventType type = EventType::None;

		// Only has valid values for mouse EventTypes such as MouseDown and MouseRelease.
		// Magnitude is pixels along X and Y axis with origin at top left window corner.
		// Y axis points downwards and the X axis points towards the right.
		int32_t mouseX;
		int32_t mouseY;
	};
}
