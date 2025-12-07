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

#include "../engine.h"
#include "render_system.h"

#include "../components/load_request_component.h"

void InputSystem::init()
{
    RenderSystem * renderer = engine->get_system<RenderSystem>();
    if( !renderer )
        return;

    window = renderer->get_window();

    glfwSetKeyCallback( window, [](GLFWwindow* win, int key, int scancode, int action, int mods)
    {
        Engine* eng = static_cast<Engine*>(glfwGetWindowUserPointer(win));
        InputSystem* system = eng->get_system<InputSystem>();
        system->process_key(key, action);
    });
}

void InputSystem::update( World &world, double dt )
{
    glfwPollEvents();

    if( load_level_0 ) {
        Entity event = world.create_entity();

        LoadRequestComponent comp;
        comp.filename = "/home/alwin/Documents/Programming/Graphics/racetrack/src/data.json";

        world.add_component( event, comp );

        load_level_0 = false;
    }
}

void InputSystem::process_key( int key, int action )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose( window, GLFW_TRUE );

    if( key == GLFW_KEY_0 && action == GLFW_PRESS ) {
        load_level_0 = true;
    }
}
