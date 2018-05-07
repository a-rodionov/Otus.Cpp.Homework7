#pragma once

#include "IOutput.h"
#include "Observable.h"

class OutputObservable : public Observable<IOutput>
{

protected:

  void Output(const std::size_t timestamp, const std::list<std::string>& data) {
    for (const auto& subscriber : subscribers) {
      auto subscriber_locked = subscriber.lock();
      if(subscriber_locked)
        subscriber_locked->Output(timestamp, data);
    }
  }

};
