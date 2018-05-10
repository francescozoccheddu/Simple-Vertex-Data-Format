#include "ExportCommand.hpp"

#include "c4d.h"
#include "c4d_basedocument.h"
#include "c4d_commanddata.h"
#include "c4d_gui.h"
#include "c4d_symbols.h"
#include "c4d_plugin.h"

const Int32 ExportCommand::id{ 900000109 };

Bool ExportCommand::Execute (BaseDocument * document)
{
	MessageDialog (String{ "Execute ExportCommand" });
	return true;
}

Bool ExportCommand::registerPlugin ()
{
	return RegisterCommandPlugin (ExportCommand::id, GeLoadString(IDS_EXPORT_COMMAND_TITLE), 0, AutoBitmap ("icon.tif"), GeLoadString(IDS_EXPORT_COMMAND_HELP), NewObj (ExportCommand));
}
