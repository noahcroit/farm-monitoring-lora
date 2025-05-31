import sys
import time
import logging
import queue
import argparse
import json
import asyncio
import async_timeout
import aiomqtt
import serial



SERIAL_PORT = '/dev/ttyS0'
BAUD_RATE = 9600  # Must match the baud rate of the sending device



async def main():
    global cfg
    # create task(s)
    t1 = asyncio.create_task(task_serial())
    t2 = asyncio.create_task(task_mqtt())
    # main loop
    while True:
        logger.info('Checking the status of all tasks')
        await asyncio.sleep(1)
        # check tasks
        if t1.done():
            logger.info('Restart a task serial')
            t1 = asyncio.create_task(task_serial())
        if t2.done():
            logger.info('Restart a task mqtt')
            t2 = asyncio.create_task(task_mqtt())




async def task_mqtt():
    global cfg
    while True:
        await asyncio.sleep(1)




async def task_serial():
    ser = serial.Serial(
        port=SERIAL_PORT,
        baudrate=BAUD_RATE,
        parity=serial.PARITY_NONE,    # No parity bit
        stopbits=serial.STOPBITS_ONE, # One stop bit
        bytesize=serial.EIGHTBITS,    # 8 data bits
        timeout=1                     # Read timeout in seconds
    )
    while True:
        # find the SOF first
        data = ser.read()
        if data == b'\xaa':
            # go through the message until EOF
            msg = ""
            completed_flag = False
            while not completed_flag:
                data = ser.read()
                if data != b'\xbb':
                    msg = msg + data.decode('utf-8')
                else:
                    completed_flag = True
                    print(msg)
        else:
            await asyncio.sleep(1)




if __name__ == '__main__':
    # Initialize parser
    parser = argparse.ArgumentParser()
    # Adding optional argument
    # Read arguments from command line
    parser.add_argument("-c", "--cfg", help="JSON file for the configuration file of all devices", default='config.json')
    args = parser.parse_args()

    # Extract config data from .json
    try:
        f = open(args.cfg, 'rb')
        cfg = json.load(f)
        f.close()
    except OSError:
        logger.error('Configuration file does not exist!')
        sys.exit()

    # setup logging system
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    console_handler = logging.StreamHandler()
    console_handler.setLevel(logging.INFO)
    console_handler.setFormatter(formatter)
    logger.addHandler(console_handler)

    # run main program
    logger.info('Starting a smartfarm worker')
    asyncio.run(main())
