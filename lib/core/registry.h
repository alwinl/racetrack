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

#include "nlohmann/json.hpp"

struct Funcs
{
	std::function<void(World&, Entity, const nlohmann::json&)> create;
	std::function<void(World&, Entity)> remove;
	std::function<void(World&)> flush;
};

using TypeLookupMap = std::unordered_map<std::string, std::type_index>;
using FunctionMap = std::unordered_map<std::type_index, Funcs>;
using TypeListMap = std::unordered_map<Entity, std::vector<std::type_index>>;

class ComponentRegistry
{
public:
    static void register_funcs( const std::string& name, std::type_index t, Funcs funcs );

	static Entity create_entity( World& world );
	static void remove_entity( World& world, Entity e );

    static bool create_component( World& world, Entity e, const std::string& name, const nlohmann::json& data);
    static bool remove_component( World& world, Entity e, const std::string& name );

    static bool flush( World& world );
    static void clear_all( World& world );

private:
    TypeLookupMap type_lookup;
	FunctionMap func_map;
    TypeListMap entity_typelist;

    static ComponentRegistry& get()
    {
        static ComponentRegistry instance;
        return instance;
    }

	bool remove_component( World& world, Entity e, std::type_index type );
};

template<typename T>
struct ComponentRegistrar
{
    ComponentRegistrar( const std::string& name )
    {
		Funcs funcs = {
			.create = []( World& world, Entity e, const nlohmann::json& json ) { T component; component.from_json(json); world.add_component<T>(e, component); },
			.remove = []( World& world, Entity e ) { world.remove_component<T>( e ); },
			.flush = []( World& world ) { world.flush_components<T>(); }
		};

        ComponentRegistry::register_funcs( name, typeid(T), funcs );
    }
};
