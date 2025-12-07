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

#include "components/component_registry.h"

void Engine::init()
{
    systems.push_back( std::make_unique<RenderSystem>() );
    systems.push_back( std::make_unique<InputSystem>() );
    systems.push_back( std::make_unique<PhysicsSystem>() );

    for( auto& system : systems )
        system->init( *this );

    force_component_registration();
}

void Engine::load( const std::string &filename )
{
    std::ifstream datafile( filename );

    if( !datafile.is_open() )
        return;

    nlohmann::json data;
    datafile >> data;

    for( auto& ent : data["entities"] ) {

        Entity e = world.create_entity();

        for( auto [name, component_data] : ent["components"].items() )
            ComponentRegistry::create( name, world, e, component_data );
    }
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
    }
}

void Engine::shutdown()
{
    for( auto& system : systems )
        system->shutdown();
}

