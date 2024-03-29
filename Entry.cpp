#include <iostream>
#include <chrono>
#include <functional>
#include <future>
#include <thread>

template<typename Interval>
class Timer {
public:
	Timer() : expired_(true) {}

	void Start(int interval, const _STD function<void()>& func) {
		expired_ = false;
		interval_ = Interval(interval);
		future_ = _STD async(_STD launch::async, [this, func]() {
            while (!expired_) {
                try
                {
                    _STD this_thread::sleep_for(interval_);
                    func();
                }
                catch (const _STD exception& e)
                {
                    _STD cout << e.what() << _STD endl;
                }
            }
			});
	}

	void Stop() {
		expired_ = true;
		future_.wait();
	}

	bool IsRunning() const
	{
		return !expired_;
	}

private:
	_STD future<void> future_;
	Interval interval_;
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
	timer_microseconds.Start(1000000, TimerMicrosecondsDoEvent);

	Timer<_STD chrono::milliseconds> timer_milliseconds;
	timer_milliseconds.Start(1000, TimerMillisecondsDoEvent);

	Timer<_STD chrono::seconds> timer_seconds;
	timer_seconds.Start(1, TimerSecondsDoEvent);

	for (;; _STD this_thread::sleep_for(_STD chrono::seconds(1)))
		_STD cout << "Main Thread ..." << '\n';

	return EXIT_SUCCESS;
}
