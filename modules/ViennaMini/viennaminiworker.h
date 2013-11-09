#ifndef VIENNAMINIWORKER_H
#define VIENNAMINIWORKER_H

#include <QObject>

#include "deviceparameters.hpp"

#include "copy.hpp"
#include "common.hpp"
#include "device.hpp"
#include "materialmanager.h"
#include "quantity.h"

#include "viennamini/simulator.hpp"

#include "viennagrid/algorithm/quantity_transfer.hpp"
#include "viennautils/average.hpp"

#include "utils.hpp"

class ViennaMiniWorker : public QObject
{
  Q_OBJECT

public:
//    typedef viennamini::simulator<Device, MaterialManager::Library>     Simulator;
//    typedef boost::shared_ptr<Simulator>                                SimulatorPtr;

  ViennaMiniWorker(viennamos::Device2u* vmos_device, MaterialManager::Library& matlib, DeviceParameters& parameters,
                   Quantity& target_pot_quan_vertex, Quantity& target_n_quan_vertex, Quantity& target_p_quan_vertex,
                   Quantity& target_pot_quan_cell, Quantity& target_n_quan_cell, Quantity& target_p_quan_cell);
  ViennaMiniWorker(viennamos::Device3u* vmos_device, MaterialManager::Library& matlib, DeviceParameters& parameters,
                   Quantity& target_pot_quan_vertex, Quantity& target_n_quan_vertex, Quantity& target_p_quan_vertex,
                   Quantity& target_pot_quan_cell, Quantity& target_n_quan_cell, Quantity& target_p_quan_cell);
  ~ViennaMiniWorker();

public slots:
  void process();

private:
  template<typename DeviceT>
  void process_impl(DeviceT& device)
  {
    typedef typename DeviceT::CellComplex                           Domain;
    typedef typename DeviceT::Segmentation                          Segmentation;
    typedef typename DeviceT::QuantityComplex                       QuanComplex;
    typedef viennamini::device<Domain, Segmentation, QuanComplex>   VMiniDevice;
    typedef typename MaterialManager::Library                       MatLib;

    VMiniDevice vmini_device(device.getCellComplex(), device.getSegmentation(), device.getQuantityComplex());
    viennamini::config & config = parameters_.config();

    for(typename DeviceParameters::iterator siter = parameters_.begin();
        siter != parameters_.end(); siter++)
    {
        std::size_t si = siter->first;
        SegmentParameters& segpara = siter->second;
        vmini_device.assign_name(si, segpara.name.toStdString());
        vmini_device.assign_material(si, segpara.material.toStdString());

        if(segpara.isContact)
        {
            vmini_device.assign_contact(si);
            config.assign_contact(si, segpara.contact, segpara.workfunction);
        }
        else
        if(segpara.isOxide)
        {
            vmini_device.assign_oxide(si);
        }
        else
        if(segpara.isSemiconductor)
        {
            vmini_device.assign_semiconductor(si, segpara.donors, segpara.acceptors);
        }
        else {
        }
    }

    // create a ViennaMini simulator object
    //
    typedef viennamini::simulator<VMiniDevice, MatLib>     Simulator;
    typedef typename Simulator::VectorType                 ResultVector;
    Simulator simulator(vmini_device, matlib_, config);

    // run the simulation
    //
    simulator();

    //simulator.write_result();


    typedef typename viennagrid::result_of::cell_tag<Domain>::type                          CellTag;
    typedef typename viennagrid::result_of::element<Domain, CellTag>::type                  CellType;
    typedef typename viennagrid::result_of::element<Domain, viennagrid::vertex_tag>::type   VertexType;
    typedef viennamini::result_accessor<CellType, QuanComplex, ResultVector>                ResultAccessor;

    ResultAccessor source_pot_acc(vmini_device.storage(), simulator.result(), simulator.quantity_potential().id());
    ResultAccessor source_n_acc  (vmini_device.storage(), simulator.result(), simulator.quantity_electron_density().id());
    ResultAccessor source_p_acc  (vmini_device.storage(), simulator.result(), simulator.quantity_hole_density().id());

    typedef typename viennadata::result_of::accessor<QuanComplex, Quantity, double, VertexType>::type TargetVertexAccessor;
    TargetVertexAccessor target_pot_vertex_acc = viennadata::make_accessor(device.getQuantityComplex(), target_pot_quan_vertex_);
    TargetVertexAccessor target_n_vertex_acc   = viennadata::make_accessor(device.getQuantityComplex(), target_n_quan_vertex_);
    TargetVertexAccessor target_p_vertex_acc   = viennadata::make_accessor(device.getQuantityComplex(), target_p_quan_vertex_);

    typedef QuantityTransferSetter<TargetVertexAccessor>  QuantityTransferSetter;
    QuantityTransferSetter pot_setter (target_pot_vertex_acc);
    QuantityTransferSetter n_setter   (target_n_vertex_acc);
    QuantityTransferSetter p_setter   (target_p_vertex_acc);

    // transfer the cell-based ViennaMini results to the vertex-based ViennaMOS
    // device using the ViennaMOS quantity accessor
    //
    viennagrid::quantity_transfer<CellType, VertexType>(device.getCellComplex(),
                                        source_pot_acc, pot_setter,
                                        viennautils::arithmetic_averaging(),
                                        any_filter(), any_filter());

    viennagrid::quantity_transfer<CellType, VertexType>(device.getCellComplex(),
                                        source_n_acc, n_setter,
                                        viennautils::arithmetic_averaging(),
                                        any_filter(), any_filter());

    viennagrid::quantity_transfer<CellType, VertexType>(device.getCellComplex(),
                                        source_p_acc, p_setter,
                                        viennautils::arithmetic_averaging(),
                                        any_filter(), any_filter());

    typedef typename viennadata::result_of::accessor<QuanComplex, Quantity, double, CellType>::type TargetCellAccessor;
    TargetCellAccessor target_pot_cell_acc = viennadata::make_accessor(device.getQuantityComplex(), target_pot_quan_cell_);
    TargetCellAccessor target_n_cell_acc   = viennadata::make_accessor(device.getQuantityComplex(), target_n_quan_cell_);
    TargetCellAccessor target_p_cell_acc   = viennadata::make_accessor(device.getQuantityComplex(), target_p_quan_cell_);

    // transfer the cell-based ViennaMini results to the cell-based ViennaMOS
    // device using the ViennaMOS quantity accessor
    //
    viennamos::copy(device, source_pot_acc, target_pot_cell_acc);
    viennamos::copy(device, source_n_acc,   target_n_cell_acc);
    viennamos::copy(device, source_p_acc,   target_p_cell_acc);

  }

private slots:
  void forward_message(QString const& msg);

signals:
  void finished();
  void message(QString const& msg);

private:
  viennamos::Device2u*            vmos_device2u_;
  viennamos::Device3u*            vmos_device3u_;
  MaterialManager::Library      & matlib_;
  DeviceParameters              & parameters_;
  Quantity                      & target_pot_quan_vertex_;
  Quantity                      & target_n_quan_vertex_;
  Quantity                      & target_p_quan_vertex_;
  Quantity                      & target_pot_quan_cell_;
  Quantity                      & target_n_quan_cell_;
  Quantity                      & target_p_quan_cell_;

};

#endif // VIENNAMINIWORKER_H
