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

#include "systems/system_renderer.h"
#include "systems/system_physics.h"
#include "systems/system_input.h"

#include "components/point.h"
#include "components/transform.h"
#include "components/velocity.h"
#include "components/triangle.h"

void Engine::init()
{
    systems.push_back( std::make_unique<RenderSystem>() );
    systems.push_back( std::make_unique<InputSystem>() );
    systems.push_back( std::make_unique<PhysicsSystem>() );

    for( auto& system : systems )
        system->init( *this );
}

void Engine::load( const std::string &filename )
{
    auto read_vec3 = [](const nlohmann::json& j){ return glm::vec3(j[0], j[1], j[2]); };

    std::ifstream datafile( filename );

    if( !datafile.is_open() )
        return;

    nlohmann::json data;

    datafile >> data;

    for( auto& ent : data["entities"] ) {

        Entity e = world.create_entity();

        auto& comps = ent["components"];

        if( comps.contains("Point") ) {
            world.add_component<PointComponent>(e, PointComponent {
                read_vec3( comps["Point"]["colour"] )
            });
        }

        if( comps.contains("Triangle") ) {
            world.add_component<TriangleComponent>(e, TriangleComponent {
                {
                    read_vec3( comps["Triangle"]["v1"] ),
                    read_vec3( comps["Triangle"]["v2"] ),
                    read_vec3( comps["Triangle"]["v3"] ),
                },
                read_vec3( comps["Triangle"]["colour"] )
            });
        }

        if( comps.contains("Transform") ) {
            world.add_component<Transform>(e, Transform {
                read_vec3( comps["Transform"]["translation"] )
            });
        }

        if( comps.contains("Velocity") ) {
            world.add_component<Velocity>(e, Velocity {
                read_vec3( comps["Velocity"]["speed"] )
            });
        }
    }

}

void Engine::run()
{
    auto renderer = get_system<RenderSystem>();
    if( !renderer )
        return;

    double last = glfwGetTime();

    while( running ) {

        double now = glfwGetTime();
        double elapsed = now - last;
        last = now;

        for( auto& system : systems )
            system->update( world, elapsed );

        for( auto& system : systems )
            system->draw( world );

        if( renderer->should_close() )
            running = false;
    }
}

void Engine::shutdown()
{
    for( auto& system : systems )
        system->shutdown();
}

