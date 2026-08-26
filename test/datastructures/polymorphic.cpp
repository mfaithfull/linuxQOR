// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "src/qor/tdd/tdd.h"
#include "src/qor/memory/reference/newref.h"
#include "src/qor/essentials/datastructures/polymorphic.h"

using namespace qor;
using namespace qor::test;

namespace test_polymorphic {

    class Base 
    {
    public:
        virtual ~Base() = default;
        virtual int value() const = 0;
        virtual void set_value(int) = 0;
    };

    class Derived : public Base 
    {
    private:
        int value_;

        public:
        Derived(int v) : value_(v) {}

        Derived() : Derived(0) {}

        int value() const override { return value_; }

        void set_value(int v) override { value_ = v; }
    };    

    class Item
    {
    public:

        virtual ~Item() {};
        template< typename U > constexpr bool Is() 
        { 
            U* test = dynamic_cast<U*>(this);
            return test != nullptr; 
        }        

        template< typename U > U* As()
        {
            U* as = dynamic_cast<U*>(this);
            return as;
        }

        virtual void make_base_abstract() = 0;
    };

    class Compound;

    class Contained : public virtual Item
    {
    public:

        Contained() = default;
        virtual ~Contained() = default;

        //clang 18 issue
        /*constexpr*/ bool Is(const Contained*) const;
        Compound* Parent() const;
        void SetParent(Compound*);
        int Index() const;
        void Detach();

    private:

        Compound* m_parent{nullptr};
    };

    class Compound : public virtual Item
    {
    public:

        ref_of<Contained>::type ChildAt(size_t i)
        {
            assert(i < ChildCount());
            return m_children[i];
        }

        size_t ChildCount() const
        {
            return m_children.size();
        }

        int IndexOf(const Contained* contained) const
        {
            int index = 0;
            for(ref_of<Contained>::type child : m_children) 
            {
                if (child == contained) 
                {
                    return index;
                }
                index++;
            }
            return -1;
        }

        void Add(ref_of<Contained>::type child)
        {
            if(child.IsNotNull())
            {
                child->Detach();
                child->SetParent(this);
                m_children.push_back(child);
            }
        }

        void Detach(Contained* contained)
        {
            auto it = std::find_if(std::begin(m_children), std::end(m_children), [contained](const ref_of<Contained>::type& that) 
                {
                    return contained == that;
                }
            );
            if(it != m_children.end())
            {
                m_children.erase(it);
            }
        }

        void DetachAllChildren()
        {
            while (!m_children.empty()) 
            {
                m_children[0]->Detach();
            }
        }

        template< class R, typename func >
        void ApplyIf( func&& apply )
        {
            for( typename ref_of<Contained>::type child : m_children) 
            {                
                R* t = child->As<R>();
                if(t)
                {
                    apply(t);
                }
            }
        }

    protected:

        std::vector< ref_of<Contained>::type > m_children;

    };

    class Scene;

    class Renderer
    {
    public:

        Renderer() = default;
        virtual ~Renderer() = default;

        void Frame(Scene* scene)
        {
            //Work out how much space there is
            //Layout the scene in the space
            //Render it
            //Handle interaction and events/sizing, Ctrl+C, signals etc
        };
    };

    class Renderable : public virtual Item
    {
    public:
        Renderable() = default;
        virtual ~Renderable() = default;
        
        virtual void Render(Renderer* renderer)
        {
            auto compound = Item::As<Compound>();
            if(compound)
            {                
                compound->ApplyIf<Renderable>([renderer](Renderable* child)
                {
                    child->Render(renderer);
                });
            }
        }        
    };

    class Scene : public Compound, public virtual Renderable
    {
    public:

        Scene() = default;
        ~Scene() = default;

    };

    struct Requirement 
    {
        // The required size to fully draw the element.
        int min_x = 0;
        int min_y = 0;

        // How much flexibility is given to the component.
        int flex_grow_x = 0;
        int flex_grow_y = 0;
        int flex_shrink_x = 0;
        int flex_shrink_y = 0;
    };

    struct Box
    {
    public:

        int x_min = 0;
        int x_max = 0;
        int y_min = 0;
        int y_max = 0;

        //static auto Intersection(Box a, Box b) -> Box;
        //static auto Union(Box a, Box b) -> Box;
        //void Shift(int x, int y);
        //bool Contain(int x, int y) const;
        //bool IsEmpty() const;
        //bool operator==(const Box& other) const;
        //bool operator!=(const Box& other) const;
    };

    
    class Arrangeable : public virtual Item
    {
    public:
        
        struct Status
        {
            int iteration = 0;
            bool need_iteration = false;
        };

        Arrangeable() = default;
        virtual ~Arrangeable() = default;        

        virtual void ComputeRequirement()
        {
            //Propagate ComputeRequirment to children
            if(Item::Is<Compound>())
            {
                Compound* compound = dynamic_cast<Compound*>(this);
                compound->ApplyIf<Arrangeable>([](Arrangeable* child)
                {
                    child->ComputeRequirement();
                });

                auto firstChild = compound->ChildAt(0)->As<Arrangeable>();
                if(firstChild)
                {
                    m_requirement = firstChild->GetRequirement();
                }
            }
            
            // By default, the requirement is the one of the first child.
        }

        Requirement GetRequirement()
        {
            return m_requirement;
        }

        virtual void SetBox(Box box)
        {
            m_box = box;
        }

        virtual void Check(Status* status)
        {
            //Propagate Check to children
            if(Item::Is<Compound>())
            {
                Compound* compound = dynamic_cast<Compound*>(this);
                compound->ApplyIf<Arrangeable>([status](Arrangeable* child)
                {
                    child->Check(status);
                });
            }
            status->need_iteration |= (status->iteration == 0);
        }

        virtual void make_base_abstract(){ };

    protected:
        
        Requirement m_requirement;
        Box m_box;
    };

    class Widget : public Compound, public virtual Contained, public virtual Arrangeable, public virtual Renderable
    {
    public:

        Widget() = default;
        virtual ~Widget() = default;

        virtual void make_base_abstract(){ };        
    };

    qor_pp_test_case (canStackPolymorphic)
    {
        polymorphic<Derived> i;
        qor_pp_assert_that(&i).isNotEqualTo(nullptr);
    }

    qor_pp_test_case (canStackinitialisePolymorphic)
    {
        Derived i(42);
        polymorphic<Derived> d(i);
        qor_pp_assert_that(d->value() == 42);
    }

    qor_pp_test_case (PolymorphicComparison)
    {
        polymorphic<Derived> i(Derived{42});
        qor_pp_assert_that(i->value() != 0);
        qor_pp_assert_that(i->value() == 42);
    }

    qor_pp_test_case(PolymorphicWidget)
    {
        polymorphic<Widget> widget;

        std::vector<Renderable*> renderables;
        renderables.emplace_back(widget.operator->());

        Renderer renderer;
        for(auto renderable : renderables)
        {
            renderable->Render(&renderer);
        }

        std::vector<Arrangeable> arrangables;
        arrangables.push_back(*widget);
        for(auto arrangeable : arrangables)
        {
            arrangeable.ComputeRequirement();
        }

        Widget* exampleWdiget = new Widget();
        polymorphic<Item> item(std::move(*exampleWdiget));
       
        renderables.emplace_back(item->As<Renderable>());

        for(auto renderable : renderables)
        {
            renderable->Render(&renderer);
        }

        arrangables.push_back(*(item->As<Arrangeable>()));
        for(auto arrangeable : arrangables)
        {
            arrangeable.ComputeRequirement();
        }
        
    }

}//test_polymorphic
