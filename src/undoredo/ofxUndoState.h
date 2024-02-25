#pragma once

#include "ofxUndoManager.h"

namespace ofx {
	namespace undo {
		template<typename Data>
		class State : public Manager<Data>
		{
		public:
			int getRedoLength() const { return Manager<Data>::history_.size() - Manager<Data>::current_index_ - 1; }
			void clearRedo() {
				switch (Manager<Data>::last_action_) {
				case Manager<Data>::UNDO:
				case Manager<Data>::REDO:
					++Manager<Data>::current_index_;
					break;
				}
				Manager<Data>::history_.resize(Manager<Data>::current_index_);
				Manager<Data>::last_action_ = Manager<Data>::OTHER;
			}
		protected:
			Data& getDataForUndo(int index) { return Manager<Data>::history_[index]; }
			Data& getDataForRedo(int index) { return Manager<Data>::history_[index]; }
		};
	}
}

template<typename Data>
using ofxUndoState = ofx::undo::State<Data>;