#!/usr/bin/env python3

import usb.core

class UsbDevice:
    def __init__(self, vendor_id=None, product_id=None):
        self.device = self.find_device(vendor_id, product_id)
        self.configuration = self.device.configurations[0]
        self.interface = self.configuration.interfaces[0][0]
        self.endpoints = [endpoint for endpoint in self.interface.endpoints]
        self.pipes = [endpoint.address for endpoint in self.endpoints]

    def find_device(self, vendor_id, product_id):
        for bus in usb.busses():
            for device in bus.devices:
                if (device.idVendor, device.idProduct) == (vendor_id, product_id):
                    return device
        raise RuntimeError('Device not found')

    def open(self):
        if hasattr(self, 'handle'):
            raise RuntimeError('Device already opened')
        self.handle = self.device.open()
        product = self.handle.getString(self.device.iProduct, 20)
        manufacturer = self.handle.getString(self.device.iManufacturer, 20)
        print(product, manufacturer)

    def close(self):
        if hasattr(self, 'handle'):
            self.handle.releaseInterface()
            del self.handle
        else:
            raise RuntimeError('Device not opened')

def main():
    dev = UsbDevice(0x04fe, 0x0008)
    dev.open()
    dev.close()

if __name__ == '__main__':
    main()
