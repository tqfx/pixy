#!/usr/bin/env python
from sys import version_info
from Cython.Build import cythonize
from distutils.core import setup, Extension

ext_modules = [
    Extension(
        name="pypixy",
        language="cpp",
        sources=[
            "pixy.cpp",
            "pypixy.pyx",
            "src/common/src/chirp.cpp",
            "src/host/libpixyusb2/src/util.cpp",
            "src/host/libpixyusb2/src/usblink.cpp",
            "src/host/libpixyusb2/src/libpixyusb2.cpp",
        ],
        define_macros=[("__LINUX__", None)],
        include_dirs=[
            "/usr/include/opencv4",
            "/usr/include/libusb-1.0",
            "src/common/inc",
            "src/host/libpixyusb2/include",
            "src/host/arduino/libraries/Pixy2",
        ],
        libraries=[
            "opencv_core",
            "usb-1.0",
        ],
    )
]


setup(
    ext_modules=cythonize(
        ext_modules,
        language_level=version_info[0],
        quiet=True,
    )
)
