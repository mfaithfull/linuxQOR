// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

// This test needs C++ exceptions thrown by qor::tef exceptions to work.
// These exceptions are turned off in release builds that define NDEBUG.
#undef NDEBUG

#include "src/configuration/configuration.h"

#include <iostream>
#include <string>
#include <vector>

#include "src/qor/tdd/tdd.h"
#include "src/qor/function/type_erased_object.h"

using namespace qor;
using namespace qor::tef;

namespace {

// Cannot use gmock here for various reasons.
// 1. Mocked objects are not copyable, and copying them as undefined behavior.
// 2. We want to keep track of the number of calls across the object including
//    copies, moves and destructions. The latter is not possible with gmock.
class Object {
 public:
  Object(size_t* copy_constructor_calls = nullptr,
         size_t* move_constructor_calls = nullptr,
         size_t* destructor_calls = nullptr)
      : copy_constructor_calls_(copy_constructor_calls),
        move_constructor_calls_(move_constructor_calls),
        destructor_calls_(destructor_calls) {
    if (copy_constructor_calls_)
      *copy_constructor_calls_ = 0;

    if (move_constructor_calls_)
      *move_constructor_calls_ = 0;

    if (destructor_calls_)
      *destructor_calls = 0;
  }

  Object(const Object& dummy)
      : copy_constructor_calls_(dummy.copy_constructor_calls_),
        move_constructor_calls_(dummy.move_constructor_calls_),
        destructor_calls_(dummy.destructor_calls_) {
    if (copy_constructor_calls_)
      ++(*copy_constructor_calls_);
  }

  Object(Object&& dummy)
      : copy_constructor_calls_(dummy.copy_constructor_calls_),
        move_constructor_calls_(dummy.move_constructor_calls_),
        destructor_calls_(dummy.destructor_calls_) {
    if (move_constructor_calls_)
      ++(*move_constructor_calls_);
  }

  ~Object() {
    if (destructor_calls_)
      ++(*destructor_calls_);
  }

 private:
  size_t* copy_constructor_calls_;
  size_t* move_constructor_calls_;
  size_t* destructor_calls_;
};

// Class that is not copy constructible.
class NonCopyable {
 public:
  NonCopyable() {}
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable(NonCopyable&&) = default;
};

// Double-check that copy constructability detection works.
static_assert(!std::is_copy_constructible<NonCopyable>::value,
              "The NonCopyable class must not be copy constructible.");

}  // anonymous namespace


struct TypeErasedObjectTestSuite{};

qor_pp_test_suite_case(TypeErasedObjectTestSuite, TestEmpty) 
{
    TypeErasedObject test;
    qor_pp_assert_that(!test.HasStoredObject());
    qor_pp_assert_that(!test);
    qor_pp_assert_that(test.GetObject()).isNull();
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, StorePointer) 
{
    TypeErasedObject test;

    size_t copied, moved, destroyed;
    Object object(&copied, &moved, &destroyed);

    test.StorePointer(&object);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    qor_pp_assert_that(!test.HasStoredObject());
    qor_pp_assert_that(!!test).isTrue();
    qor_pp_assert_that(test.GetObject()).isEqualTo(&object);
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, StoreObject) 
{
    TypeErasedObject test;
    size_t copied, moved, destroyed;
    Object object(&copied, &moved, &destroyed);

    // Copy the object.
    test.StoreObject(object);
    qor_pp_assert_that(copied).isEqualTo(1);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    qor_pp_assert_that(test.HasStoredObject());
    qor_pp_assert_that(!!test);
    qor_pp_assert_that(test.GetObject()).isNotEqualTo(&object);

    // Then we move the object. It should destroy the previous copy first.
    test.StoreObject(std::move(object));
    qor_pp_assert_that(copied).isEqualTo(1);
    qor_pp_assert_that(moved).isEqualTo(1);
    qor_pp_assert_that(destroyed).isEqualTo(1);
    qor_pp_assert_that(test.HasStoredObject());
    qor_pp_assert_that(!!test);

    // Make the type-erased object release anything it has.
    test.Reset();
    qor_pp_assert_that(copied).isEqualTo(1);
    qor_pp_assert_that(moved).isEqualTo(1);
    qor_pp_assert_that(destroyed).isEqualTo(2);
    
    qor_pp_assert_that(!test.HasStoredObject());    
    qor_pp_assert_that(!test);
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, StoreObjectSharedPtr) 
{
    TypeErasedObject test;
    size_t copied, moved, destroyed;
    auto object = std::make_shared<Object>(&copied, &moved, &destroyed);

    // Copy the object shared pointer. The object itself is not copied.
    test.StoreObject(object);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);
    qor_pp_assert_that(test.HasStoredObject());
    qor_pp_assert_that(!!test);
    qor_pp_assert_that(test.GetObject()).isEqualTo(object.get());
    
    // Reset the type-erased object. Since we still have a reference to the
    // stored object it should not have been destroyed.
    test.Reset();
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);
    qor_pp_assert_that(!test.HasStoredObject());    
    qor_pp_assert_that(!test);

    // Reset the last reference. The object should now be destroyed.
    object.reset();
    qor_pp_assert_that(destroyed).isEqualTo(1);
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, StoreObjectSharedPtrConst) {
  TypeErasedObject test;

  size_t copied, moved, destroyed;
  auto object = std::make_shared<const Object>(&copied, &moved, &destroyed);

  // Copy the object shared pointer. The object itself is not copied.
  test.StoreObject(object);
  qor_pp_assert_that(copied).isEqualTo(0);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(0);  
  qor_pp_assert_that(test.HasStoredObject());
  qor_pp_assert_that(!!test);
  qor_pp_assert_that(test.GetObject()).isEqualTo((void*)(object.get()));
  
  // Reset the type-erased object. Since we still have a reference to the
  // stored object it should not have been destroyed.
  test.Reset();
  qor_pp_assert_that(copied).isEqualTo(0);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(0);
  
  qor_pp_assert_that(!test.HasStoredObject());
  qor_pp_assert_that(!test);

  // Reset the last reference. The object should now be destroyed.
  object.reset();
  qor_pp_assert_that(destroyed).isEqualTo(1);  
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, StoreObjectsOfDifferentTypes) 
{
    TypeErasedObject test;
    size_t object_copied, object_moved, object_destroyed;
    Object object(&object_copied, &object_moved, &object_destroyed);

    size_t shared_copied, shared_moved, shared_destroyed;
    std::shared_ptr<Object> shared_object = std::make_shared<Object>(
        &shared_copied, &shared_moved, &shared_destroyed);

    // Store an object.
    test.StoreObject(object);
    qor_pp_assert_that(object_copied).isEqualTo(1);
    qor_pp_assert_that(object_moved).isEqualTo(0);
    qor_pp_assert_that(object_destroyed).isEqualTo(0);
    qor_pp_assert_that(shared_copied).isEqualTo(0);
    qor_pp_assert_that(shared_moved).isEqualTo(0);
    qor_pp_assert_that(shared_destroyed).isEqualTo(0);

    // Store a shared pointer to an object.
    test.StoreObject(shared_object);
    qor_pp_assert_that(object_copied).isEqualTo(1);
    qor_pp_assert_that(object_moved).isEqualTo(0);
    qor_pp_assert_that(object_destroyed).isEqualTo(1);
    qor_pp_assert_that(shared_copied).isEqualTo(0);
    qor_pp_assert_that(shared_moved).isEqualTo(0);
    qor_pp_assert_that(shared_destroyed).isEqualTo(0);    

    // Reset the local reference to the shared object.
    shared_object.reset();

    // Store an object pointer. Should destroy the stored object.
    test.StorePointer(&object);
    qor_pp_assert_that(object_copied).isEqualTo(1);
    qor_pp_assert_that(object_moved).isEqualTo(0);
    qor_pp_assert_that(object_destroyed).isEqualTo(1);
    qor_pp_assert_that(shared_copied).isEqualTo(0);
    qor_pp_assert_that(shared_moved).isEqualTo(0);
    qor_pp_assert_that(shared_destroyed).isEqualTo(1);    

    // Move the first object back.
    test.StoreObject(std::move(object));
    qor_pp_assert_that(object_copied).isEqualTo(1);
    qor_pp_assert_that(object_moved).isEqualTo(1);
    qor_pp_assert_that(object_destroyed).isEqualTo(1);
    qor_pp_assert_that(shared_copied).isEqualTo(0);
    qor_pp_assert_that(shared_moved).isEqualTo(0);
    qor_pp_assert_that(shared_destroyed).isEqualTo(1);
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, CopyTypeErasedObjects) {
  TypeErasedObject test;
  size_t copied, moved, destroyed;
  Object object(&copied, &moved, &destroyed);

  // Object is copied inside the type-erased object.
  test.StoreObject(object);
  qor_pp_assert_that(copied).isEqualTo(1);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(0);

  // Nothing should happen either if we try to self-assign.
  test = test;
  qor_pp_assert_that(copied).isEqualTo(1);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(0);

  // Same for move-assignment.
  test = std::move(test);
  qor_pp_assert_that(copied).isEqualTo(1);  
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(0);  

  // Type-erased object is copied. The object inside it is copied too.
  TypeErasedObject test_copy = test;
  qor_pp_assert_that(copied).isEqualTo(2);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(0);

  // Same should happen with if the assignment operator is used.
  // The previous object should be destroyed.
  test_copy = test;
  qor_pp_assert_that(copied).isEqualTo(3);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(1);

  // No object is destroyed if nothing was contained.
  TypeErasedObject test_copy_empty;
  test_copy_empty = test;
  qor_pp_assert_that(copied).isEqualTo(4);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(1);

  // The type-erased object is moved, but only the unique_ptr owning the object
  // in the heap is moved. The object itself is not.
  TypeErasedObject test_move = std::move(test_copy);
  qor_pp_assert_that(copied).isEqualTo(4);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(1);
  
  // Same should happen with if the move-assignment operator is used.
  // The previous stored object is destroyed.
  test = std::move(test_move);
  qor_pp_assert_that(copied).isEqualTo(4);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(2);  

  // No object is destroyed if moving into an empty object.
  TypeErasedObject test_move_empty;
  test_move_empty = std::move(test_copy_empty);
  qor_pp_assert_that(copied).isEqualTo(4);
  qor_pp_assert_that(moved).isEqualTo(0);
  qor_pp_assert_that(destroyed).isEqualTo(2);
  
  // Move the object into the type-erased object.
  // The previously stored object is destroyed.
  test.StoreObject(std::move(object));
  qor_pp_assert_that(copied).isEqualTo(4);
  qor_pp_assert_that(moved).isEqualTo(1);
  qor_pp_assert_that(destroyed).isEqualTo(3);
  
  // Reset the copies we have of the original type-erased object.
  test.Reset();
  test_move_empty.Reset();
  qor_pp_assert_that(copied).isEqualTo(4);
  qor_pp_assert_that(moved).isEqualTo(1);
  qor_pp_assert_that(destroyed).isEqualTo(5);
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, CopyTypeErasedObjectsShared) 
{
    TypeErasedObject test;
    size_t copied, moved, destroyed;
    auto object = std::make_shared<Object>(&copied, &moved, &destroyed);

    // Store the shared pointer to the object. The object itself is not copied.
    test.StoreObject(object);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // Nothing should happen either if we try to self-assign.
    // This is tested when there's only one reference, as if the self-assign check
    // failed it would destroy the object.
    test = test;
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // Same for move-assignment.
    test = std::move(test);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);


    // Type-erased object is copied. The object inside is not copied, but its
    // shared pointer is.
    TypeErasedObject test_copy = test;
    qor_pp_assert_that(copied).isEqualTo(0);  
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);  

    // Same should happen with if the assignment operator is used.
    // The previous object should not be destroyed because it's still referenced.
    test_copy = test;
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // No object is destroyed either if nothing was contained.
    TypeErasedObject test_copy_empty;
    test_copy_empty = test;
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // Type-erased object is moved. The object inside it's not moved, but again
    // only its shared pointer is.
    TypeErasedObject test_move = std::move(test_copy);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // Same should happen with if the move-assignment operator is used.
    // The previous stored object is destroyed.
    test = std::move(test_move);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // No object is destroyed if moving into an empty object.
    TypeErasedObject test_move_empty;
    test_move_empty = std::move(test_copy_empty);
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // Release the copy of the object we have.
    object.reset();

    // Destroy the original type-erased object. Since test_move_empty is still
    // around the object is not destroyed yet.
    test.Reset();
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(0);

    // Destroy test_move, which should be the last reference to the object.
    // It should now be destroyed.
    test_move_empty.Reset();
    qor_pp_assert_that(copied).isEqualTo(0);
    qor_pp_assert_that(moved).isEqualTo(0);
    qor_pp_assert_that(destroyed).isEqualTo(1);
}

qor_pp_test_suite_case(TypeErasedObjectTestSuite, NonCopyableObject) 
{
    TypeErasedObject test;
    test.StoreObject(NonCopyable());

    try 
    {
        // Moving should work.
        TypeErasedObject f1 = std::move(test);
        //SUCCEED();

        // Copying should not.
        TypeErasedObject f2 = f1;
        //FAIL();

    } 
    catch (const Serious& error) 
    {
        qor_pp_assert_that(error.what().Content() == "Object not copyable. Type-erased object is not copy-constructible.").isTrue();
    }
}
