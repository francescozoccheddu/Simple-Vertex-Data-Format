#include "c4d.h"
#include "ExportCommand.hpp"
#include "ProviderTag.hpp"

Bool PluginStart (void)
{
	if (!ExportCommand::registerPlugin ())
	{
		return false;
	}
	
	if (!ProviderTag::registerPlugin ())
	{
		return false;
	}

	return true;
}

void PluginEnd (void)
{}

Bool PluginMessage (int id, void *data)
{
	switch (id)
	{

		case C4DPL_INIT_SYS:
			return resource.Init ();

		default:
			return false;
				
	}
}