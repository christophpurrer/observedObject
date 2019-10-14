//
//  ObservedObject.hpp
//  observedObject
//
//  Created by Christoph Purrer on 10/13/19.
//  Copyright © 2019 Christoph Purrer. All rights reserved.
//

#ifndef ObservedObject_h
#define ObservedObject_h

#include <memory>

template<class T>
class ObservedObject;

template<class T>
class Observer {
 public:
  // static getter
  static T* get(Observer<T>* observer) {
    return static_cast<T*>(observer->get());
  }
  // cstr/dstr
  Observer(T* observed) : observed_(observed), alive_(true) {}
  ~Observer() = default;
  // disable copy
  Observer(const Observer&) = delete;
  Observer& operator=(const Observer&) = delete;
  // enable move
  Observer(Observer&& other) {
    observed_ = other.observed_;
    alive_ = std::exchange(other.alive_, false);
    other.observed_ = nullptr;
  };
  Observer& operator=(Observer&& other) {
    observed_ = other.observed_;
    alive_ = std::exchange(other.alive_, false);
    other.observed_ = nullptr;
    return *this;
  };
  friend class ObservedObject<T>;

 private:
  T* observed_;
  bool alive_;
  // return observed
  T* const get() const {
    return alive_ ? observed_ : nullptr;
  };
};

template<class T>
class ObservedObject {
 public:
  ObservedObject() : observer_(std::make_shared<Observer<T>>(static_cast<T*>(this))) {}
  ~ObservedObject() noexcept {
    this->observer_->alive_ = false;
  }

 protected:
  std::shared_ptr<Observer<T>> observer_;
};

#endif /* ObservedObject_h */
