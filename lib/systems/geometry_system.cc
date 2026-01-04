/*
 * geometry_system.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "geometry_system.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../core/world.h"
#include "../core/engine.h"
#include "../core/view.h"

#include "../components/geometry_component.h"
#include "../components/mesh_component.h"

void GeometrySystem::update( double elapsed )
{
	auto& world = engine->get_world();

	for( auto [entity, geometry] : world.view<GeometryComponent>() )
		if( geometry.dirty ) {
			regenerate_mesh(  world, entity, geometry );
			geometry.dirty = false;
		}
}

void GeometrySystem::regenerate_mesh( World &world, Entity ent, GeometryComponent &geometry )
{
	auto * mesh = world.get_component<MeshComponent>(ent);

	if( !mesh ) {
		engine->get_registry().create_component(ent, "MeshComponent" );
		mesh = world.get_component<MeshComponent>(ent);
	}

    mesh->topology = geometry.filled ? MeshComponent::Topology::TRIANGLE_FAN : MeshComponent::Topology::LINE_LOOP;
	mesh->vertices.clear();
	mesh->colours.clear();
	mesh->indices.clear();

	float a = glm::max(geometry.axis_a, geometry.axis_b);
	float b = glm::min(geometry.axis_a, geometry.axis_b);

	std::vector<glm::vec3> points;
	for( int i = 0; i < geometry.segments; i++ ) {

		float t = (float)i / geometry.segments * glm::two_pi<float>();
		points.push_back( glm::vec3( a * cos(t), b * sin(t), 0.0f ) );
	}

	if( geometry.filled ) {

		glm::vec3 center(0.0f, 0.0f, 0.0f);
        mesh->vertices.push_back(center);
        mesh->colours.push_back(geometry.colour);

		for (int i = 0; i < geometry.segments; i++)
		{
            int next = (i + 1) % geometry.segments;

            mesh->vertices.push_back(points[i]);
            mesh->colours.push_back(geometry.colour);

            mesh->vertices.push_back(points[next]);
            mesh->colours.push_back(geometry.colour);
        }
	} else {

        for( auto &p : points ) {

            mesh->vertices.push_back(p);
            mesh->colours.push_back(geometry.colour);
        }

        mesh->vertices.push_back(points[0]);
        mesh->colours.push_back(geometry.colour);
    }
}
