/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_PORT_UTILS_FPS_HPP
#define SIXENSE_PORT_UTILS_FPS_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense.h"
#include "sixense_math.hpp"

#include <vector>

using sixenseMath::Vector2;
using sixenseMath::Vector3;
using sixenseMath::Matrix3;
using sixenseMath::Matrix4;
using sixenseMath::Quat;

#include "export.hpp"

#include "sixense_utils/derivatives.hpp"
#include "sixense_utils/button_states.hpp"
#include "sixense_utils/sixense_utils_string.hpp"

namespace sixenseUtils {


	class SIXENSE_UTILS_EXPORT FPSViewAngles {

	public:
		FPSViewAngles();

		void setGame( const char* game_name );

		typedef enum {
			MOUSELOOK,
			FREE_AIM_TWO_CONTROLLER,
			FREE_AIM_ONE_CONTROLLER,
			DUAL_ANALOG
		} fps_mode;

		void setMode( fps_mode mode );
		fps_mode getMode();

		int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f );

		Vector3 getViewAngles(); // The final view angles, ie the feet plus the aiming from the controller
		Vector3 getViewAngleOffset(); // The negative controller aim, used to keep the camera stationary in metroid mode

		Vector3 getSpinSpeed();

		void forceViewAngles( fps_mode mode, Vector3 ); // Used to initialize the view direction when switching into stick spin mode

		void setFeetAnglesMetroid( Vector3 angles );
		Vector3 getFeetAnglesMetroid();

		float getTestVal();

		// Parameters to control the different modes
		typedef enum {
			CONTROLLER_ANGLE_MODE,

			AIM_1TO1_HEADING_MULTIPLIER,
			AIM_1TO1_PITCH_MULTIPLIER,
			AIM_1TO1_RATCHET_VERTICAL,

			AIM_METROID_HEADING_MULTIPLIER,
			AIM_METROID_PITCH_MULTIPLIER,
			AIM_METROID_DEAD_ZONE_RADIUS,
			AIM_METROID_ACCEL_BAND_SIZE,
			AIM_METROID_MAX_SPEED,
			AIM_METROID_AUTO_LEVEL_RATE,
			AIM_METROID_ACCEL_BAND_EXPONENT,
			AIM_METROID_SWITCH_BLEND_TIME_ENTER,
			AIM_METROID_SWITCH_BLEND_TIME_EXIT,

			FEET_ANGLES_OFFSET_STICK_SPIN_HORIZ_MULTIPLIER,
			FEET_ANGLES_OFFSET_STICK_SPIN_VERT_MULTIPLIER,
			FEET_ANGLES_OFFSET_STICK_SPIN_INVERT_PITCH,
			FEET_ANGLES_OFFSET_STICK_SPIN_EXPONENT,

			PITCH_CHANGE_BLEND_VAL,

			SPRING_VIEW_ENABLED,
			SPRING_VIEW_MIN_SPRING,
			SPRING_VIEW_MAX_SPRING,
			SPRING_VIEW_MIN_ANGLE,
			SPRING_VIEW_MAX_ANGLE,

			FEET_ANGLES_ALLOW_VERT_STICK_SPIN,

			AIM_METROID_ACCEL_BAND_POWER,

			HOLDING_TURN_SPEED,

			ROLL_CORRECTION_BLEND,
			EXIT_METROID_BLEND,

			LEFT_HANDED,

			LAST_FPS_VIEW_ANGLES_PARAM
		} fps_params;

		void setParameter( fps_params param, float val );
		float getParameter( fps_params param );

		void setFov( float hfov, float vfov );
		void getFov( float *hfov, float *vfov );

		void setHoldingTurnSpeed( float horiz, float vert );

		void setRatcheting( bool );
		bool isRatcheting();

		void reset();

		void forceMetroidBlend( float blend_fraction );

	protected:
		void computeAimOffset1to1( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms );
		void computeAimOffsetPointer( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms );

		void computeFeetAnglesOffsetStickSpin( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms );
		void computeFeetAnglesOffsetMetroid( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms );
		void computeFeetAnglesOffsetRatchet( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms );

		void computeSpringViewOffset();

		Vector3 clampViewAngles( Vector3 angles, Vector3 max_vals );

	private:
		fps_mode _mode;

		// Keep track of the game so we can do 
		std::string _game_name;

		// Different types of aim offsets
		Vector3 _aim_offset_1to1_metroid, _aim_offset_1to1_mouselook; // need two because they're scaled differently
		Vector3 _aim_offset_pointer;

		Vector3 _spring_view_offset;
		float _spring_vel;
		Quat _spring_view;

		// Different types of feet angles
		Vector3 _feet_angles_stick_spin;
		Vector3 _feet_angles_metroid;
		Vector3 _feet_angles_ratchet;

		// Parameters to control the different modes
		std::vector<float> _param_vals;

		ButtonStates _left_button_watcher, _right_button_watcher;

		Vector3 _ratchet_base_orientation;

		// Are we flicking? (0-1)
		float _flicking;

		bool _ratcheting;
		bool _just_started_ratcheting, _just_stopped_ratcheting;

		float _roll;

		// Keep track of the fov
		float _hfov, _vfov;

		// This is used to blend pitch smoothly between modes
		float _blend_pitch_val;

		Vector3 _blend_view_offset;
		Vector3 _prev_blend_view_offset;

		Vector3 _mode_switch_blend_angle_start, _mode_switch_blend_angle_end, _mode_switch_blend_angles;

		double _mode_switch_blend_duration_ms;
		double _mode_switch_blend_start_time;

		float _test_val;

		Vector3 _holding_turn_speed;

		// Allow metroid spinning to blend in over time. Useful when switching from 1-to-1 mode.
		float _metroid_blend_start_time;
		int _metroid_blend_in_or_out; // 0 == in, 1 == out

		Vector3 _metroid_spin_speed, _stick_spin_speed;

		float _force_blend_aim_metroid_start_time;
		float _force_blend_aim_metroid_pitch_mult;
		float _force_blend_aim_metroid_heading_mult;
	};

	class SIXENSE_UTILS_EXPORT FPSEvents {

	public:

		typedef enum {
			WALK_LEFT,
			WALK_RIGHT,
			WALK_FORWARD,
			WALK_BACK,
			JUMP,
			USE,
			PRIMARY_FIRE,
			SECONDARY_FIRE,
			ZOOM,
			MELEE,
			LEAN_LEFT,
			LEAN_RIGHT,
			CROUCH,
			SPRINT,
			THROW_GRENADE,
			FLASHLIGHT,
			NIGHTVISION,
			RELOAD,
			NEXT_WEAPON,
			PREV_WEAPON,
			ESC_KEY,
			EQUIP_GRENADE,
			MEDPACK_SWITCH,
			GIVE,
			NEXT_PRIMARY_WEAPON,
			ONE_TO_ONE_CARRY,
			LAST_FPS_EVENT
		} fps_event;

		FPSEvents(); 

		void setGame( const char* game_name );

		int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f );

		bool eventActive( fps_event event_query );
		bool eventStarted( fps_event event_query );
		bool eventStopped( fps_event event_query );

		Vector3 getGrenadeThrowVelocity();

		// Parameters to control the different modes
		typedef enum {
			MELEE_SENSITIVITY,
			WEAPON_SELECT_SENSITIVITY,
			CROUCH_SENSITIVITY,
			JUMP_SENSITIVITY,
			RELOAD_SENSITIVITY,
			THROW_SENSITIVITY,

			CONTROLLER_ANGLE_MODE,

			AUTO_ONE_TO_ONE_START_VEL,
			AUTO_ONE_TO_ONE_START_ACCEL,
			AUTO_ONE_TO_ONE_START_ACCEL_TIMER,
			AUTO_ONE_TO_ONE_START_ANGLE_THRESH,

			AUTO_ONE_TO_ONE_STOP_XY_DIST,
			AUTO_ONE_TO_ONE_STOP_Z_DIST,

			LEFT_HANDED,

			LAST_FPS_EVENTS_PARAM
		} fps_params;

		void setParameter( fps_params param, float val );
		float getParameter( fps_params param );

	private:
		std::vector<bool> _event_started, _event_stopped, _event_persistent_state;
		Derivatives _left_deriv, _right_deriv;
		Derivatives _left_deriv_offset, _right_deriv_offset;

		// Keep track of the velocity of the hand when a grenade is thrown
		Vector3 _grenade_throw_vel;

		// Keep track of the game so we can do 
		std::string _game_name;

		// Parameters to control the different modes
		std::vector<float> _param_vals;

		// The z depth at which one to one mode started. 
		Vector3 _one_to_one_start_pos;

	};


	class SIXENSE_UTILS_EXPORT FPSPlayerMovement {

	public:
		FPSPlayerMovement();

		void setGame( const char* game_name );

		int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f );

		Vector2 getWalkDir();

		// Parameters to control the different modes
		typedef enum {
			DEAD_ZONE_PERCENT,
			EXPONENTIAL,
			USE_RIGHT_HAND,

			LEFT_HANDED,

			LAST_FPS_MOVEMENT_PARAM
		} fps_movement_params;

		void setParameter( fps_movement_params param, float val );
		float getParameter( fps_movement_params param );

	private:
		Vector2 _walk_dir;
		std::string _game_name;

		// Parameters to control the different modes
		std::vector<float> _param_vals;

	};

	class SIXENSE_UTILS_EXPORT FPSMeleeWeapon {
	public:
		FPSMeleeWeapon();

		int update( sixenseControllerData *left_cd, sixenseControllerData *right_cd, float frametime_in_ms=0.0f );

		// Weapon movement
		Vector3 getMeleeWeaponPos();
		Vector3 getMeleeWeaponBladePos();
		Matrix3 getMeleeWeaponMatrix();

		// Attack
		bool swingAttackStarted(); // since last update
		bool isAttacking();
		
		// 1-to-1 mode (when the melee weapon should be moving with the controller and not animated
		bool OneToOneModeStarted();
		bool OneToOneModeStopped();
		bool isInOneToOneMode();

		Vector3 getSwingAttackStartPos();
		Vector3 getSwingAttackDir();

		void setToggleMode( bool mode );

		void forceOneToOneMode( bool mode );

		// Parameters to control the different modes
		typedef enum {
			SWING_START_VELOCITY,
			SWING_STOP_VELOCITY,
			SWING_REARM_VELOCITY,
			BLADE_LENGTH,
			CONTROLLER_ANGLE,

			LEFT_HANDED,

			LAST_MELEE_EVENTS_PARAM
		} melee_params;

		void setParameter( melee_params param, float val );
		float getParameter( melee_params param );


	private:
		Derivatives _left_deriv, _right_deriv;
		bool _armed; // Can we attack again?
		bool _just_started_attack, _attacking;
		bool _just_started_1to1, _just_stopped_1to1, _in_1to1_mode;
		int _swing_wait_count; // How many frames to wait until starting a swing
		int _swing_count; // How long have we been swinging?
		Matrix4 _start_swing_mat;
		Vector3 _swing_dir_vec; // Direction we are attacking in
		Vector3 _swing_start_pos; // Position swing started from

		float _last_trigger_pos;

		Vector3 _weap_pos, _last_weap_pos;
		Matrix3 _weap_mat, _last_weap_mat;

		Vector3 _weap_blade_pos;

		bool _toggle_mode;

		// Parameters to control the different modes
		std::vector<float> _param_vals;

	};

}

#endif

#pragma warning(pop)

