#pragma once


#include "log_global.h"
#include <memory>
#include <mutex>
#include <condition_variable>
#include "common/timer.hpp"
#include <atomic>
#include <sstream>

INSIGHT_NAME_SPACE

struct IProgress
{
	virtual void percent(float p) = 0;
	virtual void msg(const char *m) = 0;
	virtual void title(const char *title) = 0;
	virtual void setInfinite(bool val) = 0;
	virtual void starting() = 0;
	virtual void finished() = 0;
};

class LOG_EXPORT Progress
{
public:
	std::mutex prog_mutex;
	std::stringstream prog_log_ss;
	Progress();
	~Progress();
	virtual void percent(float p);//0-1
	virtual void msg(const char *m);
	virtual void title(const char *m);
	virtual void setInfinite(bool val);
	void msg(const std::string &s);
	void title(const std::string &s);

	void end();
	void Start();

	void Stop();

	void Pause();

	void Resume();

	void Finish();

	bool IsStarted();

	bool IsStopped();

	bool IsPaused();

	bool IsRunning();

	bool IsFinished();

	void BlockIfPaused();

	const Timer& GetTimer() const { return timer_; }
	void setProgress(IProgress *prog);
private:
	IProgress * _progImp;
	std::mutex mutex_;
	std::mutex _prog_msg_mutex;
	std::condition_variable pause_condition_;

	Timer timer_;

	std::atomic<bool> started_;
	std::atomic<bool> stopped_;
	std::atomic<bool> paused_;
	std::atomic<bool> pausing_;
	std::atomic<bool> finished_;
};


LOG_EXPORT extern	 Progress prog;

#define ILOG(INFO) \
	{ std::lock_guard<std::mutex> gard(prog.prog_mutex); prog.prog_log_ss << #INFO ":"
//prog.prog_mutex.lock();

#if 0
#define IEND \
	".";prog.msg(prog.prog_log_ss.str());prog.prog_log_ss.clear();prog.prog_log_ss.str(""); } //prog.prog_mutex.unlock()
#else
#define IEND \
	".";prog.end(); } 
#endif
#define ITITLE(T) \
	prog.title(T)

INSIGHT_NAME_SPACE_END

