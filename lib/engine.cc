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

#include "systems/system_renderer.h"
#include "systems/system_physics.h"
#include "systems/system_input.h"

#include <GLFW/glfw3.h>

void Engine::init()
{
    auto renderer = std::make_unique<RenderSystem>();
    auto inputter = std::make_unique<InputSystem>();

    running = renderer->init();
    if( !running )
        return;

    inputter->init( renderer->get_window() );

    systems.push_back( std::move( renderer ) );
    systems.push_back( std::move( inputter ) );
    systems.push_back( std::make_unique<PhysicsSystem>() );

    world.init();

    // --- Create a sample entity ---
    Entity e = world.create_entity();
    world.add_component<PointComponent>(e, PointComponent{ glm::vec3(1,1,0) } );
    world.add_component<Transform>(e, Transform{ glm::vec3(0.f, 0.f, 0.f) } );
    world.add_component<Velocity>(e, Velocity{ glm::vec3(1.f, 0.f, 0.f) } );

    // --- Create a second entity ---
    e = world.create_entity();
    auto& triangle = world.add_component<TriangleComponent>(e, TriangleComponent {
        {
            glm::vec3(-1.f, 0.f, 0.f ),
            glm::vec3( 1.f, 0.f, 0.f ),
            glm::vec3( 0.f, 2.f, 0.f )
        },
        glm::vec3( 0.0f,  1.f, 0.0f )
    } );
    world.add_component<Transform>(e, Transform{ glm::vec3(0.5f, 1.f, 0.f) } );
    world.add_component<Velocity>(e, Velocity{ glm::vec3(0.5f, 1.f, 0.f) } );
}

void Engine::run()
{
    double last = glfwGetTime();

    while( running ) {

        double now = glfwGetTime();
        double elapsed = now - last;
        last = now;

        world.update( elapsed );

        for( auto& system : systems )
            system->update( world, elapsed );

        for( auto& system : systems )
            system->draw( world );

        for( auto& system : systems ) {
            if( auto renderer = dynamic_cast<RenderSystem*>(system.get()) )
                if( renderer->should_close() )
                    running = false;
        }
    }
}

void Engine::shutdown()
{
    for( auto& system : systems )
        system->shutdown();

    world.cleanup();
}
