/*
 * glfw_platform.h Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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

#pragma once

#include "../core/platform.h"

struct GLFWwindow;
class InputQueue;

class GLFWPlatform : public IPlatform
{
	bool create_window( InputQueue& sink ) override;
	void destroy_window() override;

	void begin_render() override;
	void present_frame() override;

	double get_time() override;
	void poll_events() override;
	bool should_close() override;

private:
    GLFWwindow* window = nullptr;
	InputQueue * event_sink = nullptr;

	void handle_key( int key, int scancode, int action, int mods );
	void handle_mouse_move( double xpos, double ypos );
	void handle_mouse_button( int button, int action, int mods );
};

