/*
 * world.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

// #include <any>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <memory>

using Entity = std::uint32_t;
constexpr uint32_t InvalidEntity = (uint32_t)-1;

template<typename... Ts> class View;

class World
{
private:
	struct IStore
	{
        virtual ~IStore() = default;
        virtual void clear() = 0;
    };
    template< typename T>
    class Store : public IStore
    {
    public:
        using ComponentMap = std::unordered_map<Entity, T>;
		using RemovalList = std::vector<Entity>;

        T* add( Entity e, const T& component ) { components[e] = component; return &(components[e]); }
        void remove( Entity e ) { pending_removals.push_back(e); }
        bool has(Entity e) const { return components.count(e); }
		void flush( ) { for( Entity e : pending_removals ) components.erase(e); pending_removals.clear(); }

        T* get( Entity e ) { auto it = components.find(e); return it != components.end() ? &it->second : nullptr; }
        const T* get( Entity e ) const { auto it = components.find(e); return it != components.end() ? &it->second : nullptr; }

		template<typename Fn> void for_each( Fn&& fn ) const { for( const auto& [e, c] : components ) fn(e, c); };
		template<typename Fn> void for_each( Fn&& fn ) { for( auto& [e, c] : components ) fn(e, c); };

		void clear() override { components.clear(); pending_removals.clear(); }

    private:
        ComponentMap components;
		RemovalList pending_removals;
    };

public:
    Entity create_entity() { return next_id++; }
	void remove_entity( Entity e ) {};
	template<typename T, typename Fn> void for_each_entity( Fn&& fn ) const { component_store<T>().for_each( [&]( Entity e, const T& ) { fn(e); } ); };

    template<typename T> T& add_component( Entity e, const T& component ) { return *component_store<T>().add(e, component ); }
    template<typename T> T* get_component( Entity e ) { return component_store<T>().get(e ); }
    template<typename T> T* get_component( Entity e ) const { return component_store<T>().get(e ); }
    template<typename T> void remove_component( Entity e ) { component_store<T>().remove(e); }
    template<typename T> void flush_components() { component_store<T>().flush(); };

	template<typename... Ts> View<Ts...> view();					// defined in view.h
	template<typename... Ts> View<const Ts...> view() const;		// defined in view.h

	void clear() { next_id = 0; for( auto& [_,store] : stores) if( store ) store->clear(); };

private:
    Entity next_id = 0;
	mutable std::unordered_map<std::type_index, std::unique_ptr<IStore>> stores;

    template<typename T>
    Store<T>& component_store() const
    {
		auto type = std::type_index( typeid(T) );

		auto& store_ptr = stores[type];
		if( !store_ptr )
			store_ptr = std::make_unique<Store<T>>();

        return static_cast<Store<T>&>(*store_ptr);
    }
};
