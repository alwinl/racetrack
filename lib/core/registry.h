/*
 * registry.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <unordered_map>
#include <string>
#include <functional>
#include <typeindex>

#include "world.h"

// #include "nlohmann/json.hpp"

struct Funcs
{
	std::function<void(World&, Entity)> create;
	std::function<void(World&, Entity)> remove;
	std::function<void(World&, Entity, std::function<void(void*)>)> dispatch;
	std::function<void(World&)> flush;
};

using TypeLookupMap = std::unordered_map<std::string, std::type_index>;
using FunctionMap = std::unordered_map<std::type_index, Funcs>;
using TypeListMap = std::unordered_map<Entity, std::vector<std::type_index>>;

class Registry
{
public:
	Registry( World& world ) : world(world) {}

    template<typename T> void register_component( const std::string& component_name );

	Entity create_entity();
	void remove_entity( Entity e );

    bool create_component( Entity e, const std::string& name );
	bool with_component( Entity e, const std::string& name, std::function<void(void*)>&& fn );
    bool remove_component( Entity e, const std::string& name );

    bool flush();
    void clear();

private:
	World& world;
    TypeLookupMap type_lookup;
	FunctionMap func_map;
    TypeListMap entity_typelist;

	bool remove_component( Entity e, std::type_index type );
};

template <typename T>
void Registry::register_component( const std::string &component_name )
{
	Funcs funcs = {
		.create = []( World& world, Entity e ) { world.add_component<T>(e, T());},
		.remove = []( World& world, Entity e ) { world.remove_component<T>( e ); },
		.dispatch = []( World& world, Entity e, std::function<void(void*)> fn ) { if( auto * c= world.get_component<T>(e) ) fn( static_cast<void*>(c)); },
		.flush = []( World& world ) { world.flush_components<T>(); }
	};

	type_lookup.insert( {component_name, typeid(T)} );
	func_map.insert( {typeid(T), funcs} );
}
