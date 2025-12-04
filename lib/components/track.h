/*
 * track.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "component_registry.h"

struct TrackComponent
{
    std::vector<glm::vec3> centreline_points;

    void from_json( const nlohmann::json& json )
    {
        centreline_points.clear();

        if( !json.contains("points") )
            throw std::runtime_error("JSON error: missing required key 'points'");

        if( !json["points"].is_array() )
            throw std::runtime_error("JSON error: 'points' must be an array");

        for( auto& p : json["points"] )
            centreline_points.emplace_back( glm::vec3( p[0], p[1], p[2]) );
    }

    glm::vec3 lerp( float t ) const
    {
        if( centreline_points.empty() )
            return glm::vec3(0);

        t = glm::clamp( t, 0.0f, 1.0f );

        float scaled = t * (centreline_points.size() - 1);
        int i = (int)scaled;                    // get the start point

        if( i >= (int)centreline_points.size() - 1 )
            return centreline_points.back();

        float f = scaled - i;                   // get the fraction between start and next point

        return glm::mix(centreline_points[i], centreline_points[i+1], f);        
    }

    static ComponentRegistrar<TrackComponent> registrar;
};