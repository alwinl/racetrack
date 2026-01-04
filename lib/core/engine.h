/*
 * engine.h Copyright 2025 Alwin Leerling dna.leerling@gmail.com
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

#include <memory>
#include <vector>

#include "timing.h"
#include "commandqueue.h"
#include "inputqueue.h"
#include "registry.h"

#include "../systems/base_system.h"

class World;

class Engine
{
public:
    Engine();

    void init();
    void run();
    void shutdown();

    void stop_running() { running = false; }

	World& get_world() { return world; }
	Registry& get_registry() { return registry; }

	CommandQueue& command_list() { return commands; }
	InputQueue& input() { return input_queue; }

private:
    bool running = true;
    World world;
	Registry registry {world};
    std::vector<std::unique_ptr<ISystem>> systems;
	Timing timer;
	CommandQueue commands;
	InputQueue input_queue;

	void process_commands();
};
