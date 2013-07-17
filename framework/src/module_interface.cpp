
#include "module_interface.h"

ModuleInterface::ModuleInterface(ModuleInterface* child) : child(child)
{
    module_control_widget = new ModuleControl;

    QObject::connect(module_control_widget->getButtonSave(),  SIGNAL(clicked()),
                     this, SLOT(on_pushButtonSave_clicked()));

    QObject::connect(module_control_widget->getButtonLoad(),  SIGNAL(clicked()),
                     this, SLOT(on_pushButtonLoad_clicked()));

    QObject::connect(module_control_widget->getButtonRun(),   SIGNAL(clicked()),
                     this, SLOT(pre_execute()));

    QObject::connect(this, SIGNAL(base_data_ready()),
                     child, SLOT(preprocess()));

    QObject::connect(this,      SIGNAL(finished()),
                     this,      SLOT(post_execute()));

    QObject::connect(this,      SIGNAL(abort()),
                     this,      SLOT(report_error()));

    database         = NULL;
    multiview        = NULL;
    messenger        = NULL;
    material_manager = NULL;
}

ModuleInterface::~ModuleInterface()
{
    delete module_control_widget->getWidget();
    delete module_control_widget;
}

void ModuleInterface::setup(DataBase* central_db,         MultiView* central_multiview,
           Messenger* central_messenger, MaterialManager* central_material_manager)
{
    database         = central_db;
    multiview        = central_multiview;
    messenger        = central_messenger;
    material_manager = central_material_manager;
    emit base_data_ready();
}

void ModuleInterface::pre_execute()
{
    module_control_widget->deactivate();
    emit module_begin(child->name());
    child->execute();
}

void ModuleInterface::post_execute()
{
    emit module_end(child->name());
    module_control_widget->activate();
}

void ModuleInterface::report_error()
{
    emit module_end_error(this->name());
    module_control_widget->activate();
}

QWidget* ModuleInterface::get_widget()
{
    return module_control_widget;
}

void ModuleInterface::register_module_widget(QWidget* widget)
{
    module_control_widget->addModuleParameterWidget(widget);
    widget->resize(widget->sizeHint());

    QObject::connect(this,                               SIGNAL(state_loaded(QSettings&)),
                     module_control_widget->getWidget(), SLOT(loadState(QSettings&)));

    QObject::connect(this,                               SIGNAL(state_saved(QSettings&)),
                     module_control_widget->getWidget(), SLOT(saveState(QSettings&)));
}

//Quantity::TupleContainer& ModuleInterface::getQuantityTuples()
//{
//    return quantityTuples;
//}

//void ModuleInterface::register_quantity(Quantity& quan)
//{
//    quantityTuples.push_back(quan.acc_tuple());
//}

QuantitySet& ModuleInterface::get_quantity_set()
{
    return quantity_set;
}

void  ModuleInterface::register_quantity(Quantity& quan)
{
    quantity_set.push_back(quan);
}

void ModuleInterface::on_pushButtonSave_clicked()
{
    QString filename = QFileDialog::getSaveFileName(NULL, tr("Save State to File"), QDir::currentPath(), key::state_file_filter);
    if(filename == QString("")) return; // if the cancel button has been clicked ..
    else {
        QString suffix = QFileInfo(filename).suffix();
        if(suffix != key::state_suffix) filename += "." + key::state_suffix;
        QSettings           settings(filename, QSettings::IniFormat);
        emit state_saved(settings);
    }
}

void ModuleInterface::on_pushButtonLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(NULL, tr("Open State from File"), QDir::currentPath(), key::state_file_filter);
    if(filename == QString("")) return; // if the cancel button has been clicked ..
    else {
        QSettings           settings(filename, QSettings::IniFormat);
        emit state_loaded(settings);
    }
}
