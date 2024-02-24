#pragma once

#include "ofxUndoState.h"

namespace ofx {
	namespace undo {
		template<typename Data>
		class RefManager : public ofxUndoState<Data>
		{
		public:
			RefManager(Data& data) :ofxUndoState<Data>(), data_(data) {}
			void loadUndo(const Data& data) { data_ = data; }
		private:
			Data createUndo() const { return data_; }
			Data& data_;
		};
	}
}

template<typename Data>
using ofxUndoRef = ofx::undo::RefManager<Data>;