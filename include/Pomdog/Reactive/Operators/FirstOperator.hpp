// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Reactive/Observable.hpp"
#include "Pomdog/Reactive/Observer.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include <functional>
#include <memory>
#include <utility>

namespace Pomdog {
namespace Reactive {
namespace Detail {

template <class T>
class FirstOperator final
    : public Observer<T>
    , public Observable<T> {
public:
    explicit FirstOperator()
        : isStopped(false)
    {
    }

    void Subscribe(const std::shared_ptr<Observer<T>>& observerIn) override
    {
        POMDOG_ASSERT(observerIn);
        observer = observerIn;
    }

    void OnNext(T value) override
    {
        if (isStopped) {
            return;
        }
        isStopped = true;
        if (observer) {
            observer->OnNext(std::move(value));
            observer->OnCompleted();
        }
    }

    void OnError() override
    {
        if (isStopped) {
            return;
        }
        isStopped = true;
        if (observer) {
            observer->OnError();
        }
    }

    void OnCompleted() override
    {
        if (isStopped) {
            return;
        }
        isStopped = true;
        if (observer) {
            observer->OnCompleted();
        }
    }

private:
    std::shared_ptr<Observer<T>> observer;
    bool isStopped;
};

} // namespace Detail
} // namespace Reactive
} // namespace Pomdog
