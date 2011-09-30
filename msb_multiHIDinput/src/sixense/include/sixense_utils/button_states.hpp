/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_BUTTON_STATES_HPP
#define SIXENSE_UTILS_BUTTON_STATES_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"

#include <sixense.h>

namespace sixenseUtils {

	// LaserPointer computes a ray that shoots from the controller and intersects with the screen. 
	class SIXENSE_UTILS_EXPORT ButtonStates {

	public:
		ButtonStates();
		void update( sixenseControllerData *cd );
		bool justPressed( unsigned short which_button );
		bool justReleased( unsigned short which_button );

		void setTriggerThreshold( unsigned char thresh );
		bool triggerJustPressed();
		bool triggerJustReleased();

		sixenseControllerData _cd_copy; // Keep a copy of the last full cd for reference as the current state
		unsigned char _trigger_threshold;

	private:
		unsigned short _just_pressed;
		unsigned short _just_released;
		unsigned short _last_buttons;

		bool _trigger_just_pressed;
		bool _trigger_just_released;
		unsigned char _last_trigger_state;

	};

}

#pragma warning(pop)

#endif

