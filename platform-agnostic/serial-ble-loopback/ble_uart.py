import asyncio
import sys
from typing import Iterator
import serial
import traceback

from bleak import BleakClient, BleakScanner
from bleak.backends.characteristic import BleakGATTCharacteristic
from bleak.backends.device import BLEDevice
from bleak.backends.scanner import AdvertisementData

UART_SERVICE_UUID = "4c590933-c4a5-4996-a345-dca2150dcbc2"
UART_CHAR_UUID = "02505aae-5c3d-40ab-a86e-7840971cc85d"

# DEBUG Port
PORT = 'COM3'
BAUDRATE = 115200
TIMEOUT = 0.1
TRIALS = 10
N = 10
A = 'ABCDEFGHIJKLMNOP'*N

total_errors = 0
total_sent = 0

total_rev_errors = 0
total_rev = 0
received_buf = []


def open_serial_port():
    s = serial.Serial(port=PORT,baudrate=BAUDRATE,timeout=TIMEOUT)
    if s is None:
        print("no serial port")
        sys.exit(1)

    s.flushInput()
    s.flushOutput()
    return s

def send_serial(s : serial.Serial):
    N0 = len(A)
    b_str = bytearray()
    b_str.extend(map(ord, A))
    N1 = s.write(b_str)
    if N0 != N1: raise Exception('sent %d of %d octets' % (N1, N0))
    return A

async def send_bt_receive_serial(s : serial.Serial, uart_char :BleakGATTCharacteristic, client :BleakClient):
    global total_sent
    global total_errors
    N1 = len(A)
    b_str = bytearray()
    b_str.extend(map(ord, A))
    await client.write_gatt_char(uart_char, b_str,True)
    B = s.read(N1)
    B = B.decode("utf-8")
    N2 = len(B)
    if N1 != N2: raise Exception('received(UART) %d of %d octets' % (N2, N1))
    for a, b in zip(A, B):
        if a != b: total_errors += 1
    total_sent += N1

async def uart_terminal():
    """This is a simple "terminal" program that uses the Nordic Semiconductor
    (nRF) UART service. It reads from stdin and sends each line of data to the
    remote device. Any data received from the device is printed to stdout.
    """
    def match_nus_uuid(device: BLEDevice, adv: AdvertisementData):
        # This assumes that the device includes the UART service UUID in the
        # advertising data. This test may need to be adjusted depending on the
        # actual advertising data supplied by the device.
        if UART_SERVICE_UUID.lower() in adv.service_uuids:
            return True

        return False

    def handle_disconnect(_: BleakClient):
        print("Device was disconnected, goodbye.")
        # cancelling all tasks effectively ends the program
        for task in asyncio.all_tasks():
            task.cancel()

    def handle_rx(_: BleakGATTCharacteristic, data: bytearray):
        global total_rev_errors
        global total_rev
        global received_buf
        # since we will not receive one indicator, since BLE will buffer from UART
        received_buf.extend(data.decode("utf-8"))
        #print(', '.join('{:02x}'.format(x) for x in data))
        N1 = len(sent_A)        
        if(len(received_buf) == N1):
            B = received_buf
            for a, b in zip(sent_A, B):
                if a != b: total_rev_errors += 1
            total_rev += N1
            ble_rev_event.set()

    device = await BleakScanner.find_device_by_filter(match_nus_uuid)

    if device is None:
        print("no matching device found, you may need to edit match_nus_uuid().")
        sys.exit(1)

    async with BleakClient(device, disconnected_callback=handle_disconnect) as client:
        global sent_A
        print("Connected, Bluetooth Device")

        await client.start_notify(UART_CHAR_UUID, handle_rx)
        ble_rev_event = asyncio.Event()
        print("Start notify monitor")

        s = open_serial_port()
        print("Opened Serial Port")

        nus = client.services.get_service(UART_SERVICE_UUID)
        rx_char = nus.get_characteristic(UART_CHAR_UUID)

        try:
            for i in range(TRIALS):
                await send_bt_receive_serial(s, rx_char, client)
                sent_A = send_serial(s)
                await ble_rev_event.wait()
                ble_rev_event.clear()
                print('Count:', i)
                received_buf.clear()

            ber = float(total_errors)/float(total_sent)
            print('%d/%d = %f Receive BER' % (total_errors,total_sent,ber))

            ber = float(total_rev_errors)/float(total_rev)
            print('%d/%d = %f Sent BER' % (total_rev_errors,total_rev,ber))

        except:
            traceback.print_exc(file=sys.stdout)
            print('failed on test %d of %d with N=%d' % (i+1, TRIALS, N))
        finally:
            s.close()
            await client.stop_notify(UART_CHAR_UUID)
            #client.disconnect()

if __name__ == "__main__":
    try:
        asyncio.run(uart_terminal())
    except asyncio.CancelledError:
        # task is cancelled on disconnect, so we ignore this error
        pass


