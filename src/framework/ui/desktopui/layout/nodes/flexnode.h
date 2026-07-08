// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_FLEXNODE
#define QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_FLEXNODE

#include "../node.h"
#include "vbox.h"
#include "hbox.h"

namespace qor{ namespace ui{ namespace layout { 

	namespace 
	{
		using FlexFunction = void (*)(Requirement&);

		void function_flex_grow(Requirement& r) 
		{
			r.flex_grow_x = 1;
			r.flex_grow_y = 1;
		}

		void function_xflex_grow(Requirement& r) 
		{
			r.flex_grow_x = 1;
		}

		void function_yflex_grow(Requirement& r) 
		{
			r.flex_grow_y = 1;
		}

		void function_flex_shrink(Requirement& r) 
		{
			r.flex_shrink_x = 1;
			r.flex_shrink_y = 1;
		}

		void function_xflex_shrink(Requirement& r) 
		{
			r.flex_shrink_x = 1;
		}

		void function_yflex_shrink(Requirement& r) 
		{
			r.flex_shrink_y = 1;
		}

		void function_flex(Requirement& r) 
		{
			r.flex_grow_x = 1;
			r.flex_grow_y = 1;
			r.flex_shrink_x = 1;
			r.flex_shrink_y = 1;
		}

		void function_xflex(Requirement& r) 
		{
			r.flex_grow_x = 1;
			r.flex_shrink_x = 1;
		}

		void function_yflex(Requirement& r) 
		{
			r.flex_grow_y = 1;
			r.flex_shrink_y = 1;
		}

		void function_not_flex(Requirement& r) 
		{
			r.flex_grow_x = 0;
			r.flex_grow_y = 0;
			r.flex_shrink_x = 0;
			r.flex_shrink_y = 0;
		}

		class Flex : public Node 
		{
		public:
			explicit Flex(FlexFunction f) : f_(f) {}

			Flex(FlexFunction f, Element child) : Node(detail::unpack(std::move(child))), f_(f) {}

			void ComputeRequirement() override 
			{
				requirement_.min_x = 0;
				requirement_.min_y = 0;
				if (!children_.empty()) 
				{
					children_[0]->ComputeRequirement();
					requirement_ = children_[0]->requirement();
				}
				f_(requirement_);
			}

			void SetBox(Box box) override {
				Node::SetBox(box);
				if (children_.empty()) 
				{
					return;
				}
				children_[0]->SetBox(box);
			}

			FlexFunction f_;
		};

	}//

}}}//qor::ui::layout

#endif//QOR_PP_H_FRAMEWORK_DESKTOPUI_LAYOUT_DBOX

