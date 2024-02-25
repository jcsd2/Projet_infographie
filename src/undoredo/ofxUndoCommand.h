#pragma once

#include "ofxUndoManager.h"
#include <functional>

namespace ofx {
	namespace undo {
		struct Command
		{
			std::function<void()> undo, redo;
		};

		class CommandManager : public Manager<std::shared_ptr<Command>>
		{
		public:
			void loadUndo(const std::shared_ptr<Command>& data) { if (data->undo)data->undo(); }
			void loadRedo(const std::shared_ptr<Command>& data) { if (data->redo)data->redo(); }
		protected:
			std::shared_ptr<Command>& getDataForUndo(int index) { return history_[index]; }
			std::shared_ptr<Command>& getDataForRedo(int index) { return history_[index - 1]; }
		};
	}
}

using ofxUndoCommand = ofx::undo::Command;
using ofxUndoCommandManager = ofx::undo::CommandManager;