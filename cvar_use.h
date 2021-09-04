#pragma once

#include "main_entity.h"

class cvar : public singleton <cvar>
{
public:

	void on_create_move();
	void on_fsn();

};