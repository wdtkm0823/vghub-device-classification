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
        print(f"Product: {product}, Manufacturer: {manufacturer}")
    finally:
        handle.release()

def main():
    all_devices = get_all_usb_devices()
    for device in all_devices:
        get_device_info(device)

if __name__ == '__main__':
    main()
