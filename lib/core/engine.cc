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

#include "../systems/render_system.h"
#include "../systems/input_system.h"
#include "../systems/resource_system.h"
#include "../systems/physics_system.h"
#include "../systems/geometry_system.h"
#include "../systems/track_system.h"

#include "../components/point_component.h"
#include "../components/transform_component.h"
#include "../components/triangle_component.h"
#include "../components/velocity_component.h"
#include "../components/track_component.h"
#include "../components/lake_component.h"
#include "../components/mesh_component.h"
#include "../components/geometry_component.h"



Engine::Engine()
{
    // push systems in the order you need
    systems.push_back( std::make_unique<RenderSystem>( this ) );
    systems.push_back( std::make_unique<InputSystem>( this ) );
    systems.push_back( std::make_unique<ResourceSystem>( this ) );
    systems.push_back( std::make_unique<PhysicsSystem>( this ) );
    systems.push_back( std::make_unique<GeometrySystem>( this ) );
    systems.push_back( std::make_unique<TrackSystem>( this ) );

	registry.register_component<PointComponent>("PointComponent");
	registry.register_component<TriangleComponent>("TriangleComponent");
	registry.register_component<TrackComponent>("TrackComponent");
	registry.register_component<LakeComponent>("LakeComponent");
	registry.register_component<TransformComponent>("TransformComponent");
	registry.register_component<VelocityComponent>("VelocityComponent");
	registry.register_component<MeshComponent>("MeshComponent");
	registry.register_component<GeometryComponent>("GeometryComponent");

}

void Engine::init()
{
    for( auto& system : systems )
        system->init();
}

void Engine::run()
{
    while( running ) {

		double elapsed = timer.elapsed();

        for( auto& system : systems )
            system->input();

		process_commands();

        for( auto& system : systems )
            system->update( elapsed );

        for( auto& system : systems )
            system->draw();

		registry.flush();
    }
}

void Engine::shutdown()
{
    for( auto& system : systems )
        system->shutdown();
}

void Engine::process_commands( )
{
	auto command = commands.pop();

	while( command ) {
		command->execute( *this );
		command = commands.pop();
	}
}
