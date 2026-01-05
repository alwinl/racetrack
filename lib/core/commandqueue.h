/*
 * commandqueue.h Copyright 2026 Alwin Leerling dna.leerling@gmail.com
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
#include <queue>

#include "command.h"

class CommandQueue
{
public:
	void push( std::unique_ptr<ICommand> cmd )
		{ commands.push( std::move(cmd) ); }

	void process( Engine& engine )
	{
		auto command = pop();

		while( command ) {
			command->execute( engine );
			command = pop();
		}
	}

private:
	std::queue<std::unique_ptr<ICommand>> commands;

	std::unique_ptr<ICommand> pop()
	{
		if( commands.empty() )
			return nullptr;

		auto cmd = std::move(commands.front());
		commands.pop();
		return cmd;
	}
};
