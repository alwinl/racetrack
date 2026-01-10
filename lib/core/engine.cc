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

#include "world.h"
#include "engine.h"
#include "platform.h"

#include "../systems/render_system.h"
#include "../systems/resource_system.h"
#include "../systems/physics_system.h"
#include "../systems/geometry_system.h"
#include "../systems/track_system.h"

#include "../components/components.h"

#define STR(x) #x
#define XSTR(x) STR(x)
#define CAT(a,b) a##b

Engine::Engine( IPlatform& platform ) : platform(platform)
{
#define X(Name) systems.push_back( std::make_unique<CAT(Name,System)>( this ) );
	#include "../systems/systems.def"
#undef X

#define X(Name) registry.register_component<CAT(Name,Component)>(XSTR(CAT(Name,Component)));
	#include "../components/components.def"
#undef X
}

Engine::~Engine()		// needs to be in implementation file for the compiler to know the size of ISystem
{}

void Engine::init()
{
	if( ! platform.create_window( input_queue ) )
		running = false;

    for( auto& system : systems )
        system->init();
}

void Engine::run()
{
	double last_time = platform.get_time();

    while( running ) {

		double now = platform.get_time();
		double elapsed = now - last_time;
		last_time = now;

        for( auto& system : systems )
            system->input();

		platform.poll_events();

		running = !platform.should_close();

		while( auto event = input_queue.pop() )
			event->process( *this );

		while( auto command = command_queue.pop() )
			command->execute( *this );

        for( auto& system : systems )
            system->update( elapsed );

		platform.begin_render();

        for( auto& system : systems )
            system->draw();

		platform.present_frame();

		registry.flush();
    }
}

void Engine::shutdown()
{
    for( auto& system : systems )
        system->shutdown();

	platform.destroy_window();
}

