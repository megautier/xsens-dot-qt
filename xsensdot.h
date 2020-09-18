#ifndef XSENSDOT_H
#define XSENSDOT_H

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

//debug
#include <QFile>

class EulerReading{
public:
    qint32 timestamp;
    float x;
    float y;
    float z;
};

class XsensDot : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString macAddress READ getMacAddress NOTIFY deviceChanged)
    Q_PROPERTY(QString deviceName READ getName NOTIFY deviceChanged)
    Q_PROPERTY(uint battery READ getBattery NOTIFY batteryUpdated)
    Q_PROPERTY(bool charging READ isCharging NOTIFY batteryUpdated)
    Q_PROPERTY(EulerReading euler READ getEuler NOTIFY eulerUpdated)

public:
    XsensDot(const QBluetoothDeviceInfo &sensor);
    ~XsensDot();

    QString getMacAddress() const;
    QString getName() const;
    uint getBattery() const{ return battery;}
    bool isCharging() const{ return charging;}
    EulerReading& getEuler() { return euler;}

private slots:
    // QLowEnergyController related
    void deviceConnected();
    void deviceDisconnected();
    void errorReceived(QLowEnergyController::Error);
    void addService(const QBluetoothUuid &uuid);
    void serviceScanDone();
    // Battery related
    void batteryServiceStateChanged(QLowEnergyService::ServiceState);
    void batteryCharacteristicChanged(QLowEnergyCharacteristic,QByteArray);
    // Measurement related
    void measurementServiceStateChanged(QLowEnergyService::ServiceState);
    void measurementDataCharacteristicChanged(QLowEnergyCharacteristic,QByteArray);
    void measurementServiceResetOrientation(QLowEnergyDescriptor, QByteArray);

Q_SIGNALS:
//    void connected();
//    void disconnected();
//    void error(QString msg);
    void deviceChanged();
    void servicesUpdated();
//int
    void batteryUpdated();
    void eulerUpdated(const QString& macAddress, const EulerReading& euler);

private:
    void convertDeviceData(QByteArray &data);
    void setMeasurementMode(const QString &mode);
    void startMeasuring();
    bool resetHeading();

    QBluetoothDeviceInfo bleDot;

    QLowEnergyController *controller = nullptr;
    bool connected;
    // Battery management
    QLowEnergyService* batteryService;
    uint battery = 0;
    bool charging = false;
    // Measurement management
    QLowEnergyService *measurementService;
    bool resetOrientation = false;
    int count = 0;
    EulerReading euler;
};

#endif // XSENSDOT_H
