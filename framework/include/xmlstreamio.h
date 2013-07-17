#ifndef XMLSTREAMIO_H
#define XMLSTREAMIO_H

#include <QTreeWidget>
#include <QString>
#include <QtXml>

class XMLStreamIO
{
public:
    XMLStreamIO(QTreeWidget* other);
    void load(QString const& filename);

private:
    void readMaterials();
    void readMaterial(QTreeWidgetItem *parent);
    void readID(QTreeWidgetItem *parent);
    void readName(QTreeWidgetItem *parent);
    void readParameters(QTreeWidgetItem *parent);
    void readParameter(QTreeWidgetItem *parent);
    void readParameterName(QTreeWidgetItem *parent);
    void readParameterValue(QTreeWidgetItem *parent);
    void readParameterUnit(QTreeWidgetItem *parent);
    void skipUnknownElement();

    QTreeWidget* tree;
    QXmlStreamReader reader;
};

#endif // XMLSTREAMIO_H
