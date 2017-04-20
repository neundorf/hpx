#include "qhpx.hpp"


#include <hpx/hpx.hpp>
#include <hpx/hpx_start.hpp>
#include <hpx/include/thread_executors.hpp>
#include <hpx/lcos/future_wait.hpp>
#include <hpx/util/bind.hpp>
#include <hpx/apply.hpp>
#include <hpx/util/find_prefix.hpp>

#include <QEvent>
#include <QCoreApplication>


QHpx* QHpx::s_qhpx = nullptr;

QHpx::QHpx()
:QObject()
,mainLoopCV()
,mainLoopMutex()
,argc(0)
,argv(nullptr)
{
}


QHpx* QHpx::self()
{
    if (!s_qhpx)
    {
        s_qhpx = new QHpx();
    }

    return s_qhpx;
}


void QHpx::setArgcArgv(int _argc, char** _argv)
{
    argc = _argc;
    argv = _argv;
}


void QHpx::start()
{
    hpx::start(argc, argv);
    hpx::register_thread(hpx::get_runtime_ptr(), "main");
}


void QHpx::stop()
{
    mainLoopCV.notify_one();
}


void QHpx::hpxMainLoop()
{
    std::unique_lock<hpx::lcos::local::mutex> mut(mainLoopMutex);
    mainLoopCV.wait(mut);
    hpx::finalize();
}


int hpx_main(int argc, char ** argv)
{
    QHpx::self()->hpxMainLoop();
    return 0;
}


// application specific code

#define MY_EVENT_RUN (QEvent::User+1)
#define MY_EVENT_RUN_THREAD_FINISHED (QEvent::User+2)

class ThreadFinishedEvent : public QEvent
{
    public:
        ThreadFinishedEvent(std::size_t _index, double _t):QEvent((QEvent::Type)MY_EVENT_RUN_THREAD_FINISHED), index(_index), t(_t) {}

        std::size_t getIndex() const { return index; }
        double getT() const { return t; }

    private:
        std::size_t index;
        double t;
};


bool QHpx::event(QEvent* e)
{
    switch (e->type())
    {
      case MY_EVENT_RUN:
      {
          emit runFinished();
          return true;
      }
      case MY_EVENT_RUN_THREAD_FINISHED:
      {
          const ThreadFinishedEvent* tf = dynamic_cast<ThreadFinishedEvent*>(e);
          emit runThreadFinished(tf->getIndex(), tf->getT());
          return true;
      }
      default:
          break;
    }

    return QObject::event(e);
}


double runner(double now)
{
    hpx::util::high_resolution_timer t(now);
    double sum = 0;
    for(int i=0; i<10000000; i++)
    {
        sum += cos(i);
    }
    // do something cool here
    std::cout << "sum: "<< sum << std::endl;
    return t.elapsed();
}

HPX_PLAIN_ACTION(runner, runner_action)


void runHPX(int num_threads)
{
    std::vector<hpx::lcos::future<double> > futures(num_threads);

    for(int i = 0; i < num_threads; ++i)
    {
        runner_action a;
        futures[i] = hpx::async(a, hpx::find_here(), hpx::util::high_resolution_timer::now());
    }

    hpx::lcos::wait(futures, [](std::size_t i, double t){
          QCoreApplication::postEvent(QHpx::self(), new ThreadFinishedEvent(i, t)); });

    QEvent* e = new QEvent((QEvent::Type)MY_EVENT_RUN);
    QCoreApplication::postEvent(QHpx::self(), e);
}


void QHpx::run(int num_threads)
{
    hpx::apply(hpx::util::bind(runHPX, num_threads));
}

