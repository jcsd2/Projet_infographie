#pragma once

#include "ofxUndoCommand.h"

namespace ofx {
	namespace undo {

		class Group : public CommandManager
		{
		public:
			template<typename T>
			void add(T& child) {
				ofAddListener(child.storeEvent(), this, &Group::onStore<T>);
			}
			template<typename T>
			void remove(T& child) {
				ofRemoveListener(child.storeEvent(), this, &Group::onStore<T>);
			}
		protected:
			template<typename T>
			void onStore(const void* sender, const typename T::DataType& data) {
				auto command = std::make_shared<Command>();
				auto* manager = const_cast<T*>(static_cast<const T*>(sender));
				command->undo = [manager]() {manager->undo(); };
				command->redo = [manager]() {manager->redo(); };
				store(command);
			}
		};
	}
}
using ofxUndoGroup = ofx::undo::Group;