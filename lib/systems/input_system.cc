/*
 * input.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "input_system.h"

#include <GLFW/glfw3.h>

#include "../core/engine.h"

#include "../commands/load_request.h"

void InputSystem::input()
{
	InputQueue::KeyEvent event;

	while( engine->input().poll( event ) ) {
		if( event.action == GLFW_PRESS ) {
			switch (event.key)
			{
			case GLFW_KEY_ESCAPE:
				engine->stop_running();
				break;
			case GLFW_KEY_0:
				engine->command_list().push( std::make_unique<LoadRequest>(
					"../data/data.json"
				) );
				break;
			case GLFW_KEY_1:
				engine->command_list().push( std::make_unique<LoadRequest>(
					"../data/data simple.json"
				) );
				break;

			case GLFW_KEY_2:
				engine->command_list().push( std::make_unique<LoadRequest>(
					"../data/data copy.json"
				) );
				break;
			default:
				break;
			}
		}
	}
}
