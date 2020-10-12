# xsens-dot-qt
to remove "qt.bluetooth.bluez: Missing CAP_NET_ADMIN permission. Cannot determine whether a found address is of random or public type." warning use the following command :
sudo setcap 'cap_net_raw,cap_net_admin+eip' myapplication
with myapplication, your executable name

firmware for Xsens Dot : 1.4.0 (if lower you need to change some LittleEndian to BigEndian
