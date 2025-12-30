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

#include "../world.h"

#include "../components/geometry_component.h"
#include "../components/mesh_component.h"

void GeometrySystem::update( World &world, double dt )
{
    // auto& geometries = world.storage<GeometryComponent>();

    // for( auto& [entity, geometry] : geometries.all() ) {

    //     if( ! geometry.generated ) {

    //         Entity event = world.create_entity();

    //         MeshComponent mesh;

    //         float a = glm::max(geometry.axis_a, geometry.axis_b);
    //         float b = glm::min(geometry.axis_a, geometry.axis_b);

    //         for( int i = 0; i < geometry.segments; i++ ) {

    //             float t = (float)i / geometry.segments * glm::two_pi<float>();

    //             mesh.outline.push_back( glm::vec3( a * cos(t), b * sin(t), 0.0f ) );
    //         }

    //         mesh.colour = geometry.colour;
    //         mesh.filled = geometry.filled;

    //         world.add_component( event, mesh );

    //         geometry.generated = true;
    //     }
    // }
}
