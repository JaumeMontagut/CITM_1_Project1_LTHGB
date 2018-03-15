#ifndef __DUMMYESC_H__
#define __DUMMYESC_H__


#include <conio.h>
#include "Module.h"
#include "Globals.h"

class ModuleDummyESC : public Module
{
	bool Init()
	{
		LOG("DummyESC Init!");
		return true;
	}

	update_status PreUpdate()
	{
		LOG("DummyESC PreUpdate!");
		return update_status::UPDATE_CONTINUE;
	}

	update_status Update()
	{
		LOG("DummyESC Update!");
		if(KeyPress())
		{
			return update_status::UPDATE_CONTINUE;
		}
		else
		{
			return update_status::UPDATE_STOP;
		}
		
	}

	update_status PostUpdate()
	{
		LOG("DummyESC PostUpdate!");
		return update_status::UPDATE_CONTINUE;
	}

	bool CleanUp()
	{
		LOG("DummyESC CleanUp!");
		return true;
	}

	bool KeyPress()
	{
		if (_kbhit())
		{
			return update_status::UPDATE_STOP;
		}
		else
		{
			return update_status::UPDATE_CONTINUE;
		}
	}
};

#endif // __DUMMYESC_H__