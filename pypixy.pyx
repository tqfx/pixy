# distutils: language = c++
from libc.stddef cimport *
from libc.stdint cimport *

DEF PIXY2_RAW_FRAME_WIDTH = 316
DEF PIXY2_RAW_FRAME_HEIGHT = 208

cdef extern from "pixy.h":
    cdef cppclass pixy:
        int init()
        void exit()
        pixy() except +
        int readbgr(uint8_t *)
        int setLamp(uint8_t, uint8_t)
        int setCameraBrightness(uint8_t)
        int setLED(uint8_t, uint8_t, uint8_t)

import numpy as np
cimport numpy as np

cdef class Pixy:
    cdef pixy ctx
    cdef np.ndarray img
    def __cinit__(self):
        self.img = np.zeros([PIXY2_RAW_FRAME_HEIGHT, PIXY2_RAW_FRAME_WIDTH], dtype="3B")
    def read(self):
        self.ctx.readbgr(<uint8_t *>self.img.data)
        return self.img
    def setLamp(self, x):
        self.ctx.setLamp((x >> 2) & 0xFF, x & 0xFF)
    def setLED(self, r, g, b):
        self.ctx.setLED(r, g, b)
    def setCameraBrightness(self, x):
        self.ctx.setCameraBrightness(x & 0xFF)
