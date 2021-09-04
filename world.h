#pragma once
#include "main_entity.h"

class world : public singleton <world>
{
public:

	void on_fsn_state ( ClientFrameStage_t stage );
	void on_paint_traverse();

};