#!/usr/bin/env python3

import usb.core

def get_all_usb_devices():
    devices = []
    for bus in usb.busses():
        for device in bus.devices:
            devices.append(device)
    return devices

def get_device_info(device):
    try:
        handle = device.open()
        product = handle.getString(device.iProduct, 256)
        manufacturer = handle.getString(device.iManufacturer, 256)

        # Debugging: Print product and manufacturer before encoding
        print("Raw Product:", product)
        print("Raw Manufacturer:", manufacturer)

        # Check if product and manufacturer are not None before encoding
        if product is not None:
            product = product.encode('utf-8')
        else:
            product = b"Unknown Product"

        if manufacturer is not None:
            manufacturer = manufacturer.encode('utf-8')
        else:
            manufacturer = b"Unknown Manufacturer"

        print("Product: {}, Manufacturer: {}".format(product.decode('utf-8'), manufacturer.decode('utf-8')))
    finally:
        handle.releaseInterface()

def main():
    all_devices = get_all_usb_devices()
    for device in all_devices:
        try:
            get_device_info(device)
        except usb.core.USBError as e:
            if e.errno == 13:
                print("Access denied. Try running the script with sudo.")
            else:
                print("USBError: {}".format(e))

if __name__ == '__main__':
    main()
