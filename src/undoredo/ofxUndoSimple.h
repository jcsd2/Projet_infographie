#pragma once

#include "ofxUndoState.h"

namespace ofx {
	namespace undo {
		template<typename Data>
		class Simple : public State<Data>
		{
		public:
			operator Data& () { return data_; }
			Data& operator=(const Data& data) { return data_ = data; }
		protected:
			Data createUndo() const { return data_; }
			void loadUndo(const Data& data) { data_ = data; }
		private:
			Data data_;
		};
	}
}

template<typename Data>
using ofxUndoSimple = ofx::undo::Simple<Data>;