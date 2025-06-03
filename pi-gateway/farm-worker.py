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



# UART properties of LoRa receiver
UART_SOF = b'\xaa'
UART_EOF = b'\xbb'
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
    global q2mqtt
    while True:
        try:
            if not q2mqtt.empty():
                #data = q2mqtt.get().decode('utf-8')
                msg = q2mqtt.get()
                dev_id, moisture, temp, RH = lora_message_decode(msg)
                async with aiomqtt.Client(cfg['mqtt_broker'], int(cfg['mqtt_port'])) as client:
                    await client.publish(str(dev_id) + cfg['mqtt_topics']['moisture'], payload=str(moisture))
                    await client.publish(str(dev_id) + cfg['mqtt_topics']['temp'], payload=str(temp))
                    await client.publish(str(dev_id) + cfg['mqtt_topics']['RH'], payload=str(RH))
        except Exception as e:
            logger.warning("task: mqtt publish will be stop")
            logger.error(e)
            break
        await asyncio.sleep(1)




async def task_serial():
    global cfg
    global q2mqtt
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
        if data == UART_SOF:
            # go through the message until EOF
            msg = ""
            completed_flag = False
            while not completed_flag:
                data = ser.read()
                if data != UART_EOF:
                    msg = msg + data.decode('utf-8')
                else:
                    completed_flag = True
                    q2mqtt.put(msg)
                    print(msg)
        else:
            await asyncio.sleep(1)



def lora_message_decode(msg):
    data = msg.split(",")
    dev_id = int((data[0].split("="))[1])
    moisture = int((data[1].split("="))[1])
    temp = float((data[2].split("="))[1])
    RH = float((data[3].split("="))[1])
    return dev_id, moisture, temp, RH




if __name__ == '__main__':
    # Queues
    q2mqtt = queue.Queue(maxsize=256)

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
