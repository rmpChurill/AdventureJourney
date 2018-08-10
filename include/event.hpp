#ifndef EVENT_HPP
#define EVENT_HPP

#include <vector>

namespace util
{
template <class T>
class i_event_listener
{
  public:
    virtual void handle_event() = 0;
};

template <class T>
class listener_event
{
  private:
    std::vector<i_event_listener<T> *> listeners;

  public:
    void operator+=(i_event_listener<T> *listener)
    {
        if(this.listeners.find(listener) == std::vector<i_event_listener<T>>::npos)
        {
            this.listeners.push_back(listener);
        }
    }

    void operator-=(i_event_listener<T> listener)
    {
        auto pos = this.listeners.find(listener);

        if(pos != std::vector<i_event_listener<T>>::npos)
        {
            this.listeners.remove(pos);
        }
    }
};

template <class T>
class function_event
{
  public:
    typedef void (*event_function)(void *, T);

    void operator+=(std::pair<void*, event_function> data)
    {
    }

    void operator-=(std::pair<void*, event_function> data)
    {
    }

    void operator()(const T &args) const
    {
        for(const auto &item : this.targets)
        {
            (item.first->*item.second)(args);
        }
    }

  private:
    std::vector<std::pair<void*, event_function>> targets;
};
} // namespace util

#endif /*EVENT_HPP*/