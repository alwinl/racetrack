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

#include "system_input.h"

#include <GLFW/glfw3.h>

void InputSystem::init( GLFWwindow* win )
{
    window = win;

    glfwSetWindowUserPointer( window, this );

    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods){
        auto* self = static_cast<InputSystem*>(glfwGetWindowUserPointer(win));
        self->process_key(key, action);
    });
}

void InputSystem::process()
{
    glfwPollEvents();

}

void InputSystem::shutdown()
{

}

void InputSystem::process_key( int key, int action )
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose( window, GLFW_TRUE );
}
