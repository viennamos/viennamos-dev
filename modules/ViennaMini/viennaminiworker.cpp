
#include <vector>

#include "viennaminiworker.h"
#include "stream_emitter.h"

ViennaMiniWorker::ViennaMiniWorker(viennamos::Device2u* vmos_device, MaterialManager::Library& matlib, DeviceParameters& parameters,
                                   Quantity& target_pot_quan_vertex, Quantity& target_n_quan_vertex, Quantity& target_p_quan_vertex,
                                   Quantity& target_pot_quan_cell, Quantity& target_n_quan_cell, Quantity& target_p_quan_cell)
    : vmos_device2u_(vmos_device), matlib_(matlib), parameters_(parameters),
      target_pot_quan_vertex_(target_pot_quan_vertex), target_n_quan_vertex_(target_n_quan_vertex), target_p_quan_vertex_(target_p_quan_vertex),
      target_pot_quan_cell_(target_pot_quan_cell), target_n_quan_cell_(target_n_quan_cell), target_p_quan_cell_(target_p_quan_cell)
{
    vmos_device3u_ = NULL;
}

ViennaMiniWorker::ViennaMiniWorker(viennamos::Device3u* vmos_device, MaterialManager::Library& matlib, DeviceParameters& parameters,
                                   Quantity& target_pot_quan_vertex, Quantity& target_n_quan_vertex, Quantity& target_p_quan_vertex,
                                   Quantity& target_pot_quan_cell, Quantity& target_n_quan_cell, Quantity& target_p_quan_cell)
    : vmos_device3u_(vmos_device), matlib_(matlib), parameters_(parameters),
      target_pot_quan_vertex_(target_pot_quan_vertex), target_n_quan_vertex_(target_n_quan_vertex), target_p_quan_vertex_(target_p_quan_vertex),
      target_pot_quan_cell_(target_pot_quan_cell), target_n_quan_cell_(target_n_quan_cell), target_p_quan_cell_(target_p_quan_cell)
{
    vmos_device2u_ = NULL;
}

ViennaMiniWorker::~ViennaMiniWorker()
{

}

void ViennaMiniWorker::process()
{
    //
    // Reroute streams
    //
    StreamEmitter msg_cout(std::cout);
    StreamEmitter msg_cerr(std::cerr);
    connect(&msg_cout, SIGNAL(message(QString const&)), this, SLOT(forward_message(QString const&)));
    connect(&msg_cerr, SIGNAL(message(QString const&)), this, SLOT(forward_message(QString const&)));

    //
    // process a 22u domain
    //
    if (vmos_device2u_ != NULL)
        process_impl(*vmos_device2u_); // in viennaminiworker.h (due to the use of templates ..)
    else if (vmos_device3u_ != NULL)
        process_impl(*vmos_device3u_); // in viennaminiworker.h (due to the use of templates ..)
    else std::cerr << "ViennaMiniWorker::Error: Device type is not supported!" << std::endl;
    emit finished();
}

void ViennaMiniWorker::forward_message(QString const& msg)
{
    emit message(msg);
}
