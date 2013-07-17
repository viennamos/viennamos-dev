#include "xmlstreamio.h"
#include <QFile>


XMLStreamIO::XMLStreamIO(QTreeWidget* other) : tree(other)
{
}

void XMLStreamIO::load(QString const& filename)
{

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
//        std::cerr << "Error: Cannot read file " << qPrintable(filename)
//                  << ": " << qPrintable(file.errorString())
//                  << std::endl;

        return;
    }

    reader.clear();
    reader.setDevice(&file);

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "materials") {
                readMaterials();
            } else {
                reader.raiseError(QObject::tr("Not a materials file"));
            }
        } else {
            reader.readNext();
        }
    }
}

void XMLStreamIO::readMaterials()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "material") {
                this->readMaterial(tree->invisibleRootItem());
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void XMLStreamIO::readMaterial(QTreeWidgetItem *parent)
{
    reader.readNext();
    while (!reader.atEnd())  {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "material") {
                this->readMaterial(parent);
            }
            else if (reader.name() == "id")  {
                this->readID(parent);
            }
            else {
                skipUnknownElement();
            }
        } else   {
            reader.readNext();
        }
    }
}

void XMLStreamIO::readID(QTreeWidgetItem *parent)
{

    QString id_str   = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    QTreeWidgetItem *id = new QTreeWidgetItem(parent);
    id->setText(0, id_str);

    reader.readNext();
    while (!reader.atEnd())  {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
//            if (reader.name() == "name") {
//                this->readName(id);
//            }
            if (reader.name() == "parameters") {
                this->readParameters(id);
            }
            else {
                skipUnknownElement();
            }
        } else   {
            reader.readNext();
        }
    }
}


void XMLStreamIO::readName(QTreeWidgetItem *parent)
{

    QString str   = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    QTreeWidgetItem *id = new QTreeWidgetItem(parent);
    id->setText(0, str);
}

void XMLStreamIO::readParameters(QTreeWidgetItem *parent)
{
    reader.readNext();
    while (!reader.atEnd())  {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "parameter") {
                this->readParameter(parent);
            }
            else {
                skipUnknownElement();
            }
        } else   {
            reader.readNext();
        }
    }
}

void XMLStreamIO::readParameter(QTreeWidgetItem *parent)
{
    QTreeWidgetItem *parameter = new QTreeWidgetItem(parent);

    reader.readNext();
    while (!reader.atEnd())  {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "name") {
                this->readParameterName(parameter);
            }
            else if (reader.name() == "value") {
                this->readParameterValue(parameter);
            }
            else if (reader.name() == "unit") {
                this->readParameterUnit(parameter);
            }
            else {
                skipUnknownElement();
            }
        } else   {
            reader.readNext();
        }
    }

}

void XMLStreamIO::readParameterName(QTreeWidgetItem *parent)
{
    QString str = reader.readElementText();
    parent->setText(1, str);
    if (reader.isEndElement())
        reader.readNext();
}

void XMLStreamIO::readParameterValue(QTreeWidgetItem *parent)
{
    QString str = reader.readElementText();
    parent->setText(2, str);
    if (reader.isEndElement())
        reader.readNext();
}

void XMLStreamIO::readParameterUnit(QTreeWidgetItem *parent)
{
    QString str = reader.readElementText();
    parent->setText(3, str);
    if (reader.isEndElement())
        reader.readNext();
}

void XMLStreamIO::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}
