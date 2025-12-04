/*
 * lake.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <glm/glm.hpp>

#include "component_registry.h"
#include <glm/gtc/constants.hpp>

struct LakeComponent
{
    std::vector<glm::vec2> lake_outline;
    std::vector<glm::vec2> island_outline;

    std::array<float,2> lake_axis_length = { 1.0f, 1.0f };
    float lake_freq = 4.0f;        // jaggedness frequency
    float lake_amp  = 0.15f;       // jaggedness amount (15%)    

    float island_radius = 6.0f;
    float island_freq = 3.0f;
    float island_amp  = 0.25f;

    int segments = 200;

    float smooth_noise( float t, float frequency, float amplitude )
    {
        float v1 = sin(t * frequency * 1.3f);
        float v2 = sin(t * frequency * 0.7f + 2.0f);
        float v3 = sin(t * frequency * 2.1f + 1.3f);
        return (v1 + v2 + v3) * 0.33f * amplitude;
    }
    
    void generate_lake()
    {
        lake_outline.clear();
        island_outline.clear();

        float a = glm::max(lake_axis_length[0], lake_axis_length[1]);
        float b = glm::min(lake_axis_length[0], lake_axis_length[1]);
        float c = sqrt(glm::max(0.0f, a*a - b*b)) / 2.0f;

        for (int i = 0; i < segments; i++) {

            float t = (float)i / segments * glm::two_pi<float>();

            float lake_r = 1.0f + smooth_noise( t, lake_freq, lake_amp );
            glm::vec2 lake_point( a * cos(t) * lake_r, b * sin(t) * lake_r );

            lake_outline.push_back( lake_point );

            float island_r = island_radius * (1.0f + smooth_noise( t, island_freq, island_amp ));
            glm::vec2 island_point( glm::vec2( c, 0.0f ) + glm::vec2( cos(t), sin(t) ) * island_r );

            island_outline.push_back( island_point );
        }
    }

    void from_json( const nlohmann::json& json )
    {
        lake_axis_length = { json.value( "lake_axis_length", std::array<float,2> {15.0f, 25.0f} ) };
        lake_freq = json.value("lake_freq", 4.0f );
        lake_amp  = json.value("lake_amp", 0.15f );
        island_radius = json.value("island_radius", 6.0f );
        island_freq = json.value("island_freq", 3.0f );
        island_amp  = json.value("island_amp", 0.25f );
        segments = json.value("segments", 200);

        generate_lake();
    }

    static ComponentRegistrar<LakeComponent> registrar;
};
