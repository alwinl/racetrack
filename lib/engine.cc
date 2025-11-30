/*
 * engine.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "engine.h"

#include <GLFW/glfw3.h>

void Engine::init()
{
    running = renderer.init();
    if( !running )
        return;

    inputter.init( renderer.get_window() );
    world.init();
}

void Engine::run()
{
    double last = glfwGetTime();

    while( running ) {

        double now = glfwGetTime();
        double elapsed = now - last;
        last = now;

        inputter.process();

        world.update( elapsed );

        renderer.begin_frame();
        renderer.render( world );
        renderer.end_frame();

        if( renderer.should_close() )
            running = false;
    }
}

void Engine::shutdown()
{
    inputter.shutdown();
    renderer.shutdown();
    world.cleanup();
}
