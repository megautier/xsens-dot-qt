#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <qbluetoothlocaldevice.h>
#include <QBluetoothDeviceDiscoveryAgent>

#include "xsensdot.h"

//Debug
#include <QFile>

QT_FORWARD_DECLARE_CLASS (QBluetoothDeviceInfo)

class Scanner: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
public:
    Scanner();
    ~Scanner();
    QVariant getDevices();

public slots:
    void startScan();

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void addDevice(const QBluetoothDeviceInfo&);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error);
    void scanFinished();
    void sensorValuesUpdated(const QString& macAddress, const EulerReading&);

Q_SIGNALS:
    void devicesUpdated();

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList<QObject *> devices;

    //Debug
//    QFile data_dbg;
};

#endif // SCANNER_H
