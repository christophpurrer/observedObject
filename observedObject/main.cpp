//
//  main.cpp
//  Object
//
//  Created by Christoph Purrer on 10/13/19.
//  Copyright © 2019 Christoph Purrer. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "ObservedObject.hpp"

typedef void* CRawPointer;

class MyObject : public ObservedObject<MyObject> {
 public:
  MyObject() : value_("Answer"), anotherValue_(42) {}
  std::string value_;
  int anotherValue_;
  std::shared_ptr<Observer<MyObject>> observer() {
    return this->observer_;
  }
};

#define EXPECT_EQ(actual, expected) assert(actual == expected)

static void accessObservedObject_whileStillAlive() {
  // Create object and register callbacks
  std::vector<std::shared_ptr<Observer<MyObject>>> registeredCallbacks{};
  auto myObject = std::make_unique<MyObject>();
  registeredCallbacks.push_back(myObject->observer());
  registeredCallbacks.push_back(myObject->observer());

  // Get object from callback pointer
  CRawPointer rawPointer = registeredCallbacks.back().get();
  auto observer = (Observer<MyObject>*)rawPointer;
  EXPECT_EQ(observer != nullptr, true);
  auto instance = Observer<MyObject>::get(observer);
  EXPECT_EQ(instance != nullptr, true);
  EXPECT_EQ(instance->value_, "Answer");
  EXPECT_EQ(instance->anotherValue_, 42);
}

static void accessObservedObject_afterDestruction() {
  // Create object and register callbacks
  std::vector<std::shared_ptr<Observer<MyObject>>> registeredCallbacks{};
  if (true) {
    auto myObject = std::make_unique<MyObject>();
    registeredCallbacks.push_back(myObject->observer());
    registeredCallbacks.push_back(myObject->observer());
  }

  while (registeredCallbacks.size() > 0) {
    CRawPointer rawPointer = registeredCallbacks.back().get();
    auto observer = (Observer<MyObject>*)rawPointer;
    EXPECT_EQ(observer != nullptr, true);
    auto instance = Observer<MyObject>::get(observer);
    EXPECT_EQ(instance != nullptr, false);
    registeredCallbacks.pop_back();
  }
}

static void modifyObservedObject_whileStillAlive() {
  // Create object and register callbacks
  std::vector<std::shared_ptr<Observer<MyObject>>> registeredCallbacks{};
  auto myObject = std::make_unique<MyObject>();
  registeredCallbacks.push_back(myObject->observer());
  registeredCallbacks.push_back(myObject->observer());

  // Call and modify member
  CRawPointer rawPointer = registeredCallbacks.back().get();
  auto myObjectAgain = Observer<MyObject>::get((Observer<MyObject>*)rawPointer);
  myObjectAgain->value_ = "What was the question again?";
  EXPECT_EQ(myObject->value_, "What was the question again?");
}

int main(int argc, const char * argv[]) {
    std::cout << "Mixing C with C++";
    accessObservedObject_whileStillAlive();
    accessObservedObject_afterDestruction();
    modifyObservedObject_whileStillAlive();
    return 0;
}
