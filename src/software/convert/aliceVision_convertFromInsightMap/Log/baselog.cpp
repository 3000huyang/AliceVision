#include "BaseLog.h"
#include <iostream>
#include <glog/logging.h>



INSIGHT_NAME_SPACE


Progress::Progress()
{

}

Progress::~Progress()
{

}

void Progress::setInfinite(bool val)
{
	if (_progImp){
		_prog_msg_mutex.lock();
		_progImp->setInfinite(val);
		_prog_msg_mutex.unlock();
	}
	else{
		std::cout << "set infinite " << val << std::endl;
	}
}

void Progress::msg(const char *m)
{
	if (_progImp){
		_prog_msg_mutex.lock();
		_progImp->msg(m);
		_prog_msg_mutex.unlock();
	}
	else{
		std::cout << m;
	}
}

void Progress::msg(const std::string &s)
{
	msg(s.c_str());
}

void Progress::title(const char *m)
{
	if (_progImp){
		_prog_msg_mutex.lock();
		_progImp->title(m);
		_prog_msg_mutex.unlock();
	}
	else{
		std::cout << m;
	}
}

void Progress::title(const std::string &s)
{
	title(s.c_str());
}

void Progress::end()
{
	prog.msg(prog.prog_log_ss.str()); prog.prog_log_ss.clear(); prog.prog_log_ss.str("");
	if (!_progImp){
		std::cout << std::endl;
	}
}

bool Progress::IsStopped()
{
	std::unique_lock<std::mutex> lock(mutex_);
	return stopped_;
}

bool Progress::IsStarted()
{
	std::unique_lock<std::mutex> lock(mutex_);
	return started_;
}

void Progress::Resume()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (paused_) {
		paused_ = false;
		pause_condition_.notify_all();
	}
}

void Progress::Finish()
{
	std::unique_lock<std::mutex> lock(mutex_);
	finished_ = true;
	started_ = false;
	if (_progImp){
		_prog_msg_mutex.lock();
		_progImp->finished();
		_prog_msg_mutex.unlock();
	}
	percent(0);
	msg("");
}

void Progress::Pause()
{
	std::unique_lock<std::mutex> lock(mutex_);
	paused_ = true;
}

void Progress::Start()
{
	std::unique_lock<std::mutex> lock(mutex_);
	CHECK(!started_ || finished_);
	timer_.reset();
	started_ = true;
	stopped_ = false;
	paused_ = false;
	pausing_ = false;
	finished_ = false;
	if (_progImp){
		_prog_msg_mutex.lock();
		_progImp->starting();
		_prog_msg_mutex.unlock();
	}
}

void Progress::Stop()
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		stopped_ = true;
	}
	Resume();
}

bool Progress::IsPaused()
{
	std::unique_lock<std::mutex> lock(mutex_);
	return paused_;
}

bool Progress::IsRunning()
{
	std::unique_lock<std::mutex> lock(mutex_);
	return started_ && !pausing_ && !finished_;
}

bool Progress::IsFinished()
{
	std::unique_lock<std::mutex> lock(mutex_);
	return finished_;
}

void Progress::BlockIfPaused()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (paused_) {
		pausing_ = true;
		timer_.pause();
		pause_condition_.wait(lock);
		pausing_ = false;
		timer_.resume();
	}
}

void Progress::setProgress(IProgress *progr)
{
	_prog_msg_mutex.lock();
	_progImp = progr;
	_prog_msg_mutex.unlock();
}

void Progress::percent(float p)
{
	if (_progImp){
		_prog_msg_mutex.lock();
		_progImp->percent(p);
		_prog_msg_mutex.unlock();

	}
	else{
		std::cout << "--------------" << p * 100 << "%--------------\n";
	}
}

LOG_EXPORT Progress prog;



INSIGHT_NAME_SPACE_END