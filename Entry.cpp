#include <iostream>
#include <chrono>
#include <functional>
#include <future>
#include <thread>


template<typename Interval>
class Timer {
public:
    Timer() : interval_(0), event_function_(nullptr), expired_(true) {}
    Timer (const _STD function<void()>& event_function, unsigned int interval) : interval_(interval), event_function_(event_function), expired_(true) {}

    void Start() {
        if (!event_function_)
            throw _STD runtime_error("Event function must be set before starting the timer");

        expired_ = false;
        future_ = _STD async(_STD launch::async, [this] {
            while (!expired_) {
                try {
                    _STD this_thread::sleep_for(interval_);
                    event_function_();
                }
                catch (const _STD exception& e) {
                    _STD cout << e.what() << '\n';
                }
            }
        });
    }

    void Stop() {
        expired_ = true;
        future_.wait();
    }

    bool IsRunning() const {
        return !expired_;
    }

    void SetInterval(unsigned int interval)
    {
        interval_ = Interval(interval);
    }

    void SetEvent(const _STD function<void()>& func)
    {
        event_function_ = func;
    }

private:
    _STD future<void> future_;
    Interval interval_;
    _STD function<void()> event_function_;
    bool expired_;
};


void TimerMicrosecondsDoEvent()
{
    _STD cout << "TimerMicrosecondsDoEvent..." << '\n';
}

void TimerMillisecondsDoEvent()
{
    _STD cout << "TimerMillisecondsDoEvent..." << '\n';
}

void TimerSecondsDoEvent()
{
    _STD cout << "TimerSecondsDoEvent..." << '\n';
}

int main() {

    Timer<_STD chrono::microseconds> timer_microseconds;
    timer_microseconds.SetEvent(TimerMicrosecondsDoEvent);
    timer_microseconds.SetInterval(1000000);

    Timer<_STD chrono::milliseconds> timer_milliseconds;
    timer_milliseconds.SetEvent(TimerMillisecondsDoEvent);
    timer_milliseconds.SetInterval(1000);


    Timer<_STD chrono::seconds> timer_seconds;
    timer_seconds.SetEvent(TimerSecondsDoEvent);
    timer_seconds.SetInterval(1);


    timer_microseconds.Start();
    timer_milliseconds.Start();
    timer_seconds.Start();

    for (;; _STD this_thread::sleep_for(_STD chrono::seconds(1)))
    _STD cout << "Main Thread ..." << '\n';

    return EXIT_SUCCESS;
}
