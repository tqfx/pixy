#!/usr/bin/env python
from pypixy import Pixy
import cv2

o = Pixy()
o.setLamp(0x0100)
o.setLED(0xFF, 0xFF, 0xFF)
o.setCameraBrightness(0xFF)
while True:
    cv2.imshow("test", o.read())
    if cv2.waitKey(30) == 113:
        break
