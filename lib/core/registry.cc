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

#include "registry.h"
#include "world.h"
#include <algorithm>

Entity ComponentRegistry::create_entity()
{
	Entity e = world.create_entity();

	entity_typelist.insert( {e, std::vector<std::type_index>()} );

	return e;
}

void ComponentRegistry::remove_entity( Entity e )
{
	auto it = entity_typelist.find( e );
	if( it == entity_typelist.end() )
		return;

	auto typelist = it->second;

	for( auto& type : typelist )
		remove_component( e, type );

	world.remove_entity( e );

	entity_typelist.erase( e );
}

bool ComponentRegistry::create_component( Entity e, const std::string &name )
{
    auto it = type_lookup.find( name );
    if( it == type_lookup.end() )
        return false;

	auto type = it->second;

	auto it2 = func_map.find( type );
    if( it2 == func_map.end() )
        return false;

	auto funcs = it2->second;

	funcs.create(world, e );

	auto& typelist = entity_typelist.at(e);

	if( !std::count( typelist.begin(), typelist.end(), type ) )
		typelist.push_back( type );

    return true;
}

bool ComponentRegistry::with_component( Entity e, const std::string &name, std::function<void( void * )> &&fn )
{
    auto it = type_lookup.find( name );
    if( it == type_lookup.end() )
        return false;

	auto type = it->second;

	auto it2 = func_map.find( type );
    if( it2 == func_map.end() )
        return false;

	auto funcs = it2->second;

	funcs.dispatch( world, e, fn);

	return false;
}

bool ComponentRegistry::remove_component( Entity e, const std::string& name )
{
    auto it = type_lookup.find( name );
    if( it == type_lookup.end() )
        return false;

	auto type = it->second;

	auto it2 = func_map.find( type );
    if( it2 == func_map.end() )
        return false;

	auto funcs = it2->second;

	funcs.remove( world, e );

	auto& typelist_map = entity_typelist;

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

bool ComponentRegistry::flush()
{
    for( auto [_, funcs] : func_map )
        funcs.flush( world );

    return true;
}

void ComponentRegistry::clear()
{
	entity_typelist.clear();
	world.clear();
}

bool ComponentRegistry::remove_component( Entity e, std::type_index type )
{
	auto component_funcs_it = func_map.find( type );
    auto entity_types_it = entity_typelist.find(e);

    if( (component_funcs_it == func_map.end()) || (entity_types_it == entity_typelist.end()) )		// if neither component type nor entity are registered
        return false;

	auto funcs = component_funcs_it->second;
	auto& typelist = entity_types_it->second;

	funcs.remove( world, e );		// removes this component from this entity

	auto it4 = std::find(typelist.begin(), typelist.end(), type );	// deregisters this component from this entity
    if (it4 != typelist.end()) {
		*it4 = typelist.back();
		typelist.pop_back();
	}

	return true;
}