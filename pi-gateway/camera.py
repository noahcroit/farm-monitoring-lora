import os
import base64
import argparse
import json
import cv2
import aiomqtt
import asyncio
import async_timeout



camera="/dev/v4l/by-path/platform-3f801000.csi-video-index0"

async def main():
    global cfg
    # Camera capture
    stream = cv2.VideoCapture(camera)
    cnt = 0
    while stream.isOpened() and cnt < 20:
        ret, frame = stream.read()
        cnt += 1
    stream.release()
    
    # Convert frame into base64
    ret, buf = cv2.imencode('.jpg', frame)
    jpg_byte = base64.b64encode(buf)
    jpg_str = jpg_byte.decode('UTF-8')

    # Publish base64
    if jpg_str:
        async with aiomqtt.Client(cfg['mqtt_broker'], int(cfg['mqtt_port'])) as client:
            await client.publish(cfg['mqtt_topics']['image64'], payload=jpg_str)



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
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
    
    asyncio.run(main())
