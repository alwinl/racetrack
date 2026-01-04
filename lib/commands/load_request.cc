/*
 * load_request.cc Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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

#include "load_request.h"

#include "../core/engine.h"
#include "../core/world.h"
#include "../core/registry.h"

#include <fstream>

#include "../components/geometry_component.h"
#include "../components/lake_component.h"
#include "../components/mesh_component.h"
#include "../components/point_component.h"
#include "../components/track_component.h"
#include "../components/transform_component.h"
#include "../components/triangle_component.h"
#include "../components/velocity_component.h"

void load_from_json( PointComponent& comp, const nlohmann::json& json )
{
	auto [r,g,b] = json.value( "colour", std::array<float,3> {1.0f, 0.0f, 0.0f} );
	comp.colour = glm::vec3( r, g, b );
}

void load_from_json( GeometryComponent& comp, const nlohmann::json& json )
{
	comp.axis_a = json.value( "axis_a", 1.0f );
	comp.axis_b = json.value( "axis_b", 1.0f );
	comp.segments = json.value( "segments", 4.0f );
	comp.closed = json.value( "closed", true );
	comp.filled = json.value( "filled", true );

	auto [r,g,b] = json.value( "colour", std::array<float,3> {1.0f, 0.0f, 0.0f} );
	comp.colour = glm::vec3( r, g, b );

	comp.dirty = true;
}

void load_from_json( LakeComponent& comp, const nlohmann::json& json )
{
	comp.lake_axis_length = { json.value( "lake_axis_length", std::array<float,2> {15.0f, 25.0f} ) };
	comp.lake_freq = json.value("lake_freq", 4.0f );
	comp.lake_amp  = json.value("lake_amp", 0.15f );
	comp.island_radius = json.value("island_radius", 6.0f );
	comp.island_freq = json.value("island_freq", 3.0f );
	comp.island_amp  = json.value("island_amp", 0.25f );
	comp.segments = json.value("segments", 200);

	comp.generate_lake();
}

void load_from_json( MeshComponent& comp, const nlohmann::json& json )
{

}

void load_from_json( TrackComponent& track, const nlohmann::json& json )
{
	if( !json.contains("points") )
		throw std::runtime_error("JSON error: missing required key 'points'");

	if( !json["points"].is_array() )
		throw std::runtime_error("JSON error: 'points' must be an array");

	track.width = json["width"];
	track.closed = json["closed"];
	track.colour = {json["colour"][0], json["colour"][1],json["colour"][2] };

	track.centreline.clear();
	for( auto& p : json["points"] )
		track.centreline.emplace_back( glm::vec2( p[0], p[1]) );

	track.dirty = true;
}
void load_from_json( TransformComponent& comp, const nlohmann::json& json )
{
	auto [tx,ty,tz] = json.value( "translation", std::array<float, 3>{0.0f, 0.0f, 0.0f} );
	comp.translation = glm::vec3( tx, ty, tz );

	auto [rx,ry,rz] = json.value( "rotation", std::array<float, 3>{0.0f, 0.0f, 0.0f} );
	comp.rotation = glm::vec3( rx, ry, rz );

	auto [sx,sy,sz] = json.value( "scale", std::array<float, 3>{1.0f, 1.0f, 1.0f} );
	comp.scale = glm::vec3( sx, sy, sz );
}

void load_from_json( TriangleComponent& comp, const nlohmann::json& json )
{
	std::array< std::array<float,3>, 3> defaults = {{
		{{-1.0f, 0.0f, 0.0f}},
		{{0.0f, 1.0f, 0.0f}},
		{{1.0f, 0.0f, 0.0f}}
	}};

	for( int i=0; i < 3; ++i ) {
		auto [vx,vy,vz] = json.value( std::string( "v" + std::to_string(i+1) ).c_str(), defaults[i] );
		comp.vertices[i] = glm::vec3( vx,vy,vz );
	}

	auto [r,g,b] = json.value( "colour", std::array<float,3> {1.0f, 0.0f, 0.0f} );
	comp.colour = glm::vec3( r, g, b );
}

void load_from_json( VelocityComponent& comp, const nlohmann::json& json )
{
	auto [vx,vy,vz] = json.value( "speed", std::array<float, 3>{0.0f, 0.0f, 0.0f} );
	comp.speed = glm::vec3( vx, vy, vz );
}

using JsonLoaderFn = void(*)(void*, const nlohmann::json&);

#define X(Name) \
    { #Name, [](void* ptr, const nlohmann::json& j) { \
        load_from_json(*static_cast<Name##Component*>(ptr), j); \
    } },


const std::unordered_map<std::string, JsonLoaderFn> json_loaders =
{
	#include "../components/components.def"
};

#undef X

#define STR(x) #x
#define XSTR(x) STR(x)
#define CAT(a,b) a##b

#define X(Name) { STR(Name), XSTR(CAT(Name,Component)) },

const std::unordered_map<std::string, std::string> name_to_component =
{
	#include "../components/components.def"
};

#undef X

void LoadRequest::execute( Engine &engine )
{
	auto& registry = engine.get_registry();

    std::ifstream datafile( filename );

    if( !datafile.is_open() )
        return;

    nlohmann::json data;
    datafile >> data;

	registry.clear();

    for( auto& ent : data["entities"] ) {

        Entity e = registry.create_entity();

        for( auto [name, component_data] : ent["components"].items() )
		{
			auto it = name_to_component.find( name );
			auto it2 = json_loaders.find( name );

			if( (it == name_to_component.end()) || ( it2 == json_loaders.end() ) )
				continue;

			auto type_name = it->second;
			auto loader = it2->second;

            registry.create_component( e, type_name );
			registry.with_component( e, type_name, [&](void *ptr)
			{
				if( !ptr )		// worthy of an exception
					return;

				loader( ptr, component_data );
			} );
		}
    }
}
