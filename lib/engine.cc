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

#include "systems/render_system.h"
#include "systems/time_system.h"
#include "systems/input_system.h"
#include "systems/resource_system.h"
#include "systems/physics_system.h"
#include "systems/geometry_system.h"

void Engine::init()
{
    make_systems();

    for( auto& system : systems )
        system->init();
}

void Engine::run()
{
    TimeSystem * time_system = get_system<TimeSystem>();

    while( running ) {

        for( auto& system : systems )
            system->update( world, time_system->delta() );

        for( auto& system : systems )
            system->draw( world );

        for( auto& system : systems )
            system->flush( world );
    }
}

void Engine::shutdown()
{
    for( auto& system : systems )
        system->shutdown();
}

void Engine::make_systems()
{
    // push systems in the order you need
    systems.push_back( std::make_unique<RenderSystem>( this ) );
    systems.push_back( std::make_unique<TimeSystem>( this ) );
    systems.push_back( std::make_unique<InputSystem>( this ) );
    systems.push_back( std::make_unique<ResourceSystem>( this ) );
    systems.push_back( std::make_unique<PhysicsSystem>( this ) );
    systems.push_back( std::make_unique<GeometrySystem>( this ) );
}
