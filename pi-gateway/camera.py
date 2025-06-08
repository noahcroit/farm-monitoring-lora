import os
import base64
import redis
import cv2




if __name__ == "__main__":
    # Camera capture
    stream = cv2.VideoCapture(0)
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
        redis_client = redis.StrictRedis(host="localhost", port=6379, db=0)
        channel = "image64"
        redis_client.publish(channel, jpg_str)
