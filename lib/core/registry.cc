/*
 * registry.cc Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include "../components/point_component.h"
#include "../components/transform_component.h"
#include "../components/triangle_component.h"
#include "../components/velocity_component.h"
#include "../components/track_component.h"
#include "../components/lake_component.h"
#include "../components/load_request_component.h"
#include "../components/mesh_component.h"
#include "../components/geometry_component.h"
#include "registry.h"

ComponentRegistrar<PointComponent> PointComponent::registrar("Point");
ComponentRegistrar<TriangleComponent> TriangleComponent::registrar("Triangle");
ComponentRegistrar<TrackComponent> TrackComponent::registrar("Track");
ComponentRegistrar<LakeComponent> LakeComponent::registrar("Lake");
ComponentRegistrar<TransformComponent> TransformComponent::registrar("Transform");
ComponentRegistrar<VelocityComponent> VelocityComponent::registrar("Velocity");
ComponentRegistrar<LoadRequestComponent> LoadRequestComponent::registrar("Level");
ComponentRegistrar<MeshComponent> MeshComponent::registrar("mesh");
ComponentRegistrar<GeometryComponent> GeometryComponent::registrar("polygon");

void ComponentRegistry::register_funcs( const std::string& name, std::type_index type, Funcs funcs )
{
	get().type_lookup.insert( {name, type} );
	get().func_map.insert( {type, funcs} );
}

Entity ComponentRegistry::create_entity( World& world )
{
	Entity e = world.create_entity();

	get().entity_typelist.insert( {e, std::vector<std::type_index>()} );

	return e;
}

void ComponentRegistry::remove_entity( World& world, Entity e )
{
	auto it = get().entity_typelist.find( e );
	if( it == get().entity_typelist.end() )
		return;

	auto typelist = it->second;

	for( auto& type : typelist )
		get().remove_component( world, e, type );

	world.remove_entity( e );

	get().entity_typelist.erase( e );
}

bool ComponentRegistry::create_component( World &world, Entity e, const std::string &name, const nlohmann::json &data )
{
    auto it = get().type_lookup.find( name );
    if( it == get().type_lookup.end() )
        return false;

	auto type = it->second;

	auto it2 = get().func_map.find( type );
    if( it2 == get().func_map.end() )
        return false;

	auto funcs = it2->second;

	funcs.create(world, e, data );

	auto& typelist = get().entity_typelist.at(e);

	if( !std::count( typelist.begin(), typelist.end(), type ) )
		typelist.push_back( type );

    return true;
}

bool ComponentRegistry::remove_component( World& world, Entity e, const std::string& name )
{
    auto it = get().type_lookup.find( name );
    if( it == get().type_lookup.end() )
        return false;

	auto type = it->second;

	auto it2 = get().func_map.find( type );
    if( it2 == get().func_map.end() )
        return false;

	auto funcs = it2->second;

	funcs.remove( world, e );

	auto& typelist_map = get().entity_typelist;

    auto it3 = typelist_map.find(e);
    if( it3 == typelist_map.end() )
        return false;

	auto& typelist = it3->second;

	auto it4 = std::find(typelist.begin(), typelist.end(), type );
    if (it4 != typelist.end()) {
		*it4 = typelist.back();
		typelist.pop_back();
	}

    return true;
}

bool ComponentRegistry::flush( World& world )
{
    for( auto [_, funcs] : get().func_map )
        funcs.flush( world );

    return true;
}

void ComponentRegistry::clear_all( World& world )
{
    auto& typelist_map = get().entity_typelist;

    // We copy keys because destroy() mutates the typelist map
    std::vector<Entity> to_delete;
    to_delete.reserve( typelist_map.size() );

    for( auto& [entity, types] : typelist_map ) {
        to_delete.push_back(entity);

		// remove all components of this entity
		for( auto type : types )
			get().remove_component( world, entity, type );
	}

    for( Entity e : to_delete ) {
        get().remove_entity( world, e );
	}

    // get().flush(world);
}

bool ComponentRegistry::remove_component( World& world, Entity e, std::type_index type )
{
	auto it2 = get().func_map.find( type );
    if( it2 == get().func_map.end() )
        return false;

	auto funcs = it2->second;

	funcs.remove( world, e );

	auto& typelist_map = get().entity_typelist;

    auto it3 = typelist_map.find(e);
    if( it3 == typelist_map.end() )
        return false;

	auto& typelist = it3->second;

	auto it4 = std::find(typelist.begin(), typelist.end(), type );
    if (it4 != typelist.end()) {
		*it4 = typelist.back();
		typelist.pop_back();
	}

	return true;
}