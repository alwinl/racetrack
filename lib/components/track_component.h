/*
 * track_component.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "../core/registry.h"

struct TrackComponent
{
    std::vector<glm::vec2> centreline;
    float width = 1.0f;
    bool closed = false;
    glm::vec3 colour = {1.0, 1.0, 1.0};

    bool dirty = false;

    void from_json( const nlohmann::json& json )
    {
        if( !json.contains("points") )
            throw std::runtime_error("JSON error: missing required key 'points'");

        if( !json["points"].is_array() )
            throw std::runtime_error("JSON error: 'points' must be an array");

        width = json["width"];
        closed = json["closed"];
        colour = {json["colour"][0], json["colour"][1],json["colour"][2] };

        centreline.clear();
        for( auto& p : json["points"] )
            centreline.emplace_back( glm::vec2( p[0], p[1]) );

        dirty = true;
    }

    static ComponentRegistrar<TrackComponent> registrar;
};
