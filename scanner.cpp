#include "scanner.h"
#include "xsensdot.h"

#include <QDebug>
#include <QMetaEnum>

const QString SENSOR_NAME = "Xsens DOT";

Scanner::Scanner() //: data_dbg("./data/sensor_data.csv")
{
    //debug
//    if(!data_dbg.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Append))
//        exit(1);
//    QTextStream stream(&data_dbg);
//    stream << "Address;timestamp;Angle1;Angle2;Angle3\n";

    //! [the-devicediscovery-1]
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Scanner::addDevice);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error), this, &Scanner::scanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Scanner::scanFinished);

    // Stop after 5000 ms
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    //! [the-devicediscovery-1]
}
Scanner::~Scanner()
{
    delete discoveryAgent;
    qDeleteAll(devices);
    devices.clear();

    //Debug
//    data_dbg.close();
}

QVariant Scanner::getDevices()
{
    return QVariant::fromValue(devices);
}

void Scanner::startScan()
{
    qDeleteAll(devices);
    devices.clear();
    emit devicesUpdated();

    qDebug()<< "Scanning for devices";

    //! [les-devicediscovery-2]
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    //! [les-devicediscovery-2]

}

void Scanner::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration && info.name().compare(SENSOR_NAME)==0)
    {
        qDebug()<< "add sensor "<<info.address();
        XsensDot* dot = new XsensDot(info);
        devices.append(dot);
        emit devicesUpdated();

        // Debug
        connect(dot, &XsensDot::eulerUpdated, this, &Scanner::sensorValuesUpdated);
    }

}

void Scanner::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qDebug()<< "The Bluetooth adaptor is powered off, power it on before doing discovery.";
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        qDebug()<< "Writing or reading from the device resulted in an error.";
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
                    discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        qDebug()<< "Error: " << QLatin1String(qme.valueToKey(error));
    }
}


void Scanner::scanFinished()
{

    qDebug()<< "Scan done!";
}

void Scanner::sensorValuesUpdated(const QString& macAddress, const EulerReading& euler)
{
//    QTextStream stream(&data_dbg);
//    stream<<macAddress<<";"<<euler.timestamp<<";"<<euler.x<<";"<< euler.y <<";"<<euler.z<<"\n";
    qDebug()<<macAddress<<";"<<euler.timestamp<<";"<< euler.x<<";"<<euler.y<<";"<<euler.z;
}
