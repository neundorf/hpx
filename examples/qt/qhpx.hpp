#ifndef QHPX_HPP
#define QHPX_HPP

#include <QObject>

#include <hpx/lcos/local/condition_variable.hpp>
#include <hpx/lcos/local/mutex.hpp>


class QHpx : public QObject
{
  Q_OBJECT
  public:
    static QHpx* self();

    void setArgcArgv(int _argc, char** _argv);

    void start();

    void stop();

    void run(int _numThreads);

    virtual bool event(QEvent* e);

    void hpxMainLoop();

  signals:
    void runFinished();
    void runThreadFinished(std::size_t index, double t);

  private:
//     void runHPX(int _numThreads);

    QHpx();
    
    static QHpx* s_qhpx;

    hpx::lcos::local::condition_variable mainLoopCV;
    hpx::lcos::local::mutex mainLoopMutex;

    int argc;
    char** argv;
};

#endif
