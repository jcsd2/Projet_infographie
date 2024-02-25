#pragma once

#include "ofxUndoState.h"

namespace ofx {
	namespace undo {
		template<typename T>
		class Json : public State<ofJson>, public T
		{
		public:
#ifndef OFXUNDOJSON_NOT_USE_OFXJSONUTILS
			ofJson createUndo() const { return ofxJsonUtils::convert((T&)*this); }
			void loadUndo(const ofJson& json) { ofxJsonUtils::parse(json, (T&)*this); }
#else
			ofJson createUndo() const { return ((T&)*this).toJson(); }
			void loadUndo(const ofJson& json) { ((T&)*this).loadJson(json); }
#endif
		};
	}
}

template<typename T>
using ofxUndoJson = ofx::undo::Json<T>;