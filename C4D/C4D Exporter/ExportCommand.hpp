#ifndef EXPORT_COMMAND_H__
#define EXPORT_COMMAND_H__

#include "c4d.h"
#include "c4d_commanddata.h"
#include "c4d_basedocument.h"

class ExportCommand : public CommandData
{

public:

	static const Int32 id;

	Bool Execute (BaseDocument * document) override;

	static Bool registerPlugin();

};

#endif