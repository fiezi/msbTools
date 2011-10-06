/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_UTILS_MOUSE_POINTER_HPP
#define SIXENSE_UTILS_MOUSE_POINTER_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"

#include <sixense.h>
#include <sixense_math.hpp>

using sixenseMath::Vector2;
using sixenseMath::Vector3;
using sixenseMath::Matrix3;

namespace sixenseUtils {

	// LaserPointer computes a ray that shoots from the controller and intersects with the screen.
	class SIXENSE_UTILS_EXPORT MousePointer {

	public:
		MousePointer();
		sixenseMath::Vector2 update( sixenseControllerData *cd );

		void setSensitivity( float sensitivity );
		void setAcceleration( float acceleration );
		void setSlideEnabled( bool slide_enabled );
		void setAspectRatio( float aspect_ratio );

		float getRollAngle();

		void setCenter();

	private:
		bool _slide_enabled;
		float _aspect_ratio;
		float _sensitivity;
		float _screen_width_in_mm;

		// velocity params
		float _min_vel, _max_vel;
		float _acceleration;

		// This offset is the position of the center of the virtual screen relative to the base
		sixenseMath::Vector2 mouse_offset;

		// Keep track of the previous mouse pos so we can compute velocity
		sixenseMath::Vector2 _last_mouse_pos;

		// Keep track of the last accel so we can filter it
		float _last_accel;

		float _roll_angle;

		bool _center_mouse_requested;
	};
}

#pragma warning(pop)

#endif
