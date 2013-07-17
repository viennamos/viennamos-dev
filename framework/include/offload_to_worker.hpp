#ifndef OFFLOAD_TO_WORKER_HPP
#define OFFLOAD_TO_WORKER_HPP

#include <QThread>
#include <QObject>

namespace viennamos {

template<typename Worker, typename Messenger, typename Module>
void offload(Worker* worker, Messenger* messenger, const char* signal_finished, Module* module, const char* slot_onfinish)
{
    QThread* thread = new QThread;
    worker->moveToThread(thread);

    // signals/slot communication between threads need to use the
    // queuedconnection mechanism! otherwise, it's unstable
    //
    QObject::connect(worker, SIGNAL(message(QString const&)),
                     messenger->getPlainTextEditWidget(), SLOT(appendPlainText(QString const&)),
                     Qt::QueuedConnection);
    QObject::connect(worker, signal_finished,
                     module, slot_onfinish,
                     Qt::QueuedConnection);
    QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
    QObject::connect(worker, signal_finished, thread, SLOT(quit()));
    QObject::connect(worker, signal_finished, worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(terminated()), thread, SLOT(deleteLater()));

    thread->start();
}




//template<typename ReturnType, typename Worker, typename Messenger, typename Module>
//void offload2(Worker* worker, Messenger* messenger, Module* module)
//{
//    qRegisterMetaType<ReturnType>("ViennaMiniWorker::SimulatorPtr"); // TODO


//    QThread* thread = new QThread;
//    worker->moveToThread(thread);

//    // signals/slot communication between threads need to use the
//    // queuedconnection mechanism! otherwise, it's unstable
//    //
//    QObject::connect(worker, SIGNAL(message(QString const&)),
//                     messenger->getPlainTextEditWidget(), SLOT(appendPlainText(QString const&)),
//                     Qt::QueuedConnection);
//    QObject::connect(worker, SIGNAL(finished(ReturnType)),
//                     module, SLOT(transferResult(ReturnType)),
//                     Qt::QueuedConnection);
//    QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
//    QObject::connect(worker, SIGNAL(finished(ReturnType)), thread, SLOT(quit()));
//    QObject::connect(worker, SIGNAL(finished(ReturnType)), worker, SLOT(deleteLater()));
//    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    QObject::connect(thread, SIGNAL(terminated()), thread, SLOT(deleteLater()));

//    thread->start();
//}





} // viennamos

#endif // OFFLOAD_TO_WORKER_HPP
