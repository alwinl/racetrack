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

#include <fstream>

#include "nlohmann/json.hpp"

#include "systems/render_system.h"
#include "systems/physics_system.h"
#include "systems/input_system.h"
#include "systems/resource_system.h"

#include "components/component_registry.h"

void Engine::init()
{
    systems.push_back( std::make_unique<RenderSystem>( this ) );
    systems.push_back( std::make_unique<InputSystem>( this ) );
    systems.push_back( std::make_unique<ResourceSystem>( this ) );
    systems.push_back( std::make_unique<PhysicsSystem>( this ) );

    for( auto& system : systems )
        system->init();

    force_component_registration();
}

void Engine::run()
{
    double last = glfwGetTime();

    while( running ) {

        double now = glfwGetTime();
        double elapsed = now - last;
        last = now;

        for( auto& system : systems )
            system->update( world, elapsed );

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

