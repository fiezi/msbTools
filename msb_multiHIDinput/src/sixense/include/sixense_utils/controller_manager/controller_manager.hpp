/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef CONTROLLER_MANAGER_HPP
#define CONTROLLER_MANAGER_HPP

#pragma warning(push)
#pragma warning( disable:4251 )

#include "sixense_utils/export.hpp"
#include "sixense_utils/sixense_utils_string.hpp"

#include <string>
#include <vector>
#include <map>

#include <sixense.h>

namespace sixenseUtils {

	class SIXENSE_UTILS_EXPORT controller_manager {

	public:
		// Define the steps the manager goes through for the different modes
		typedef enum {

			SETUP_COMPLETE,

			// P1C1 = One player, 1 controller

			// One player one controller
			P1C1_START, //1
			P1C1_POWER_UP_0,
			P1C1_POWER_UP_DONE,
			P1C1_AIM_P1L,
			P1C1_DONE,
			P1C1_OUT_OF_RANGE,
			P1C1_IDLE, // 7

			// One player two controllers
			P1C2_START, // 8 
			P1C2_POWER_UP_0,
			P1C2_POWER_UP_1,
			P1C2_POWER_UP_DONE,
			P1C2_AIM_P1L, // 12
			P1C2_AIM_P1R,
			P1C2_DONE,
			P1C2_OUT_OF_RANGE,
			P1C2_IDLE // 16

		} setup_step;


		typedef enum {
			NO_SOUND,
			SUCCESS_BEEP,
			FAIL_BEEP
		} sound_type;

		typedef void (*setup_callback)( setup_step );

		typedef enum {
			ONE_PLAYER_ONE_CONTROLLER,
			ONE_PLAYER_TWO_CONTROLLER,
			TWO_PLAYER_ONE_CONTROLLER,
			TWO_PLAYER_TWO_CONTROLLER,
			THREE_PLAYER_ONE_CONTROLLER,
			THREE_PLAYER_TWO_CONTROLLER,
			FOUR_PLAYER_ONE_CONTROLLER,
			FOUR_PLAYER_TWO_CONTROLLER
		} game_type;

		typedef enum {
			P1L,
			P1R,
			P2L,
			P2R,
			P3L,
			P3R,
			P4L,
			P4R, 
			LAST_CONTROLLER_DESC
		} controller_desc;

		~controller_manager();

		void setGameType( game_type gt );
		game_type getGameType();

		// Update the controller_manager. Should be called each frame.
		void update( sixenseAllControllerData * );

		// Get the controller index for the given description, ie player 1's left hand is controller index 3
		int getIndex( controller_desc );

		// Force the user to rebind the controllers to the player slots.
		void rebind();

		// Register a callback that will get called when the mode changes
		void registerSetupCallback( setup_callback );

		// Returns true if the application should show the menu system
		bool isMenuVisible();

		// Returns the filename of the recommended texture for this step
		const char* getTextureFileName();

		// Returns a string describing this step
		const char* getStepString();


		controller_manager::sound_type shouldPlaySound();

		static controller_manager *getTheControllerManager();

		static controller_desc controllerDescFromString( sixense_utils_string str );

		setup_step getCurrentStep();

	protected:
		controller_manager();

		game_type _game_type;

		// A pointer to the current step
		setup_step _current_step;

		// A list of maps, one map of steps for each of the game_types.
		std::vector< std::map< setup_step, class base_step*> >_steps;

		int _controller_map[LAST_CONTROLLER_DESC];

		setup_callback _callback;

	};

	// Singleton
	SIXENSE_UTILS_EXPORT controller_manager *getTheControllerManager();

}

#pragma warning(pop)

#endif

