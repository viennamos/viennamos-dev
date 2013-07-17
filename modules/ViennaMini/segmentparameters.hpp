#ifndef SEGMENTPARAMETERS_H
#define SEGMENTPARAMETERS_H

#include <QString>

struct SegmentParameters
{
    QString name;
    QString material;

    bool isContact;

    bool isContactSingle;
    double contact;
    bool isContactRange;
    double contactFrom;
    double contactTo;
    double workfunction;

    bool isOxide;

    bool isSemiconductor;
    double donors;
    double acceptors;

    SegmentParameters()
    {
        isContact = false;
        isOxide = false;
        isSemiconductor = false;

        name = "";
        material = "";

        isContactSingle = false;
        isContactRange = false;

        contact = 0.0;
        contactFrom = 0.0;
        contactTo = 0.0;
        workfunction = 0.0;
        donors = 0.0;
        acceptors = 0.0;
    }
};


#endif // SEGMENTPARAMETERS_H
