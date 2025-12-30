/*
 * track_system.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "track_system.h"

#include "../components/track_component.h"
#include "../components/mesh_component.h"

void TrackSystem::update( World &world, double dt )
{
    auto& tracks = world.storage<TrackComponent>();

    for( auto& [entity, track] : tracks.all() ) {

        if( !track.dirty )
            continue;

        regenerate_mesh( world, entity, track );

        track.dirty = false;
    }
}

void TrackSystem::regenerate_mesh( World &world, Entity ent, TrackComponent &track )
{
    MeshComponent mesh;
    
    mesh.filled = false;
    mesh.topology = MeshComponent::Topology::TRIANGLES;

    float half_width = track.width * 0.5f;

    for( size_t i = 0; i+1 < track.centreline.size(); ++i ) {

        glm::vec2 current = track.centreline[i];
        glm::vec2 next = track.centreline[i+1];

        glm::vec2 dir = glm::normalize( next - current );
        glm::vec2 normal = glm::normalize( glm::vec2(-dir.y, dir.x) );

        glm::vec2 left  = current + normal * half_width;
        glm::vec2 right = current - normal * half_width;

        mesh.vertices.push_back( glm::vec3(left, 0.0) );
        mesh.colours.push_back( track.colour );

        mesh.vertices.push_back( glm::vec3(right, 0.0) );
        mesh.colours.push_back( track.colour );
    }

    glm::vec2 current = track.centreline[track.centreline.size() - 1];
    glm::vec2 next = track.centreline[track.centreline.size() - 2];

    glm::vec2 dir = glm::normalize( current - next );
    glm::vec2 normal = glm::normalize( glm::vec2(-dir.y, dir.x) );

    glm::vec2 left  = current + normal * half_width;
    glm::vec2 right = current - normal * half_width;

    mesh.vertices.push_back( glm::vec3(left, 0.0) );
    mesh.colours.push_back( track.colour );

    mesh.vertices.push_back( glm::vec3(right, 0.0) );
    mesh.colours.push_back( track.colour );

    // each pair of vertices is a left and right vertex of the track, thus two pairs create a quad, which is two triangles
    // the triangle vertices of each are 0, 1, 3 and 1, 2, 3
    for( size_t i = 0; i < track.centreline.size() - 1; ++i )
    {
        mesh.indices.push_back( 0 + 2 * i );
        mesh.indices.push_back( 1 + 2 * i );
        mesh.indices.push_back( 3 + 2 * i );

        mesh.indices.push_back( 0 + 2 * i );
        mesh.indices.push_back( 2 + 2 * i );
        mesh.indices.push_back( 3 + 2 * i );
    }

    world.add_component<MeshComponent>( ent, mesh );
}
