# rpicam-apps
This is a small suite of libcamera-based applications to drive the cameras on a Raspberry Pi platform.

Build
-----

```commandline
sudo apt install -y libcamera-dev libepoxy-dev libjpeg-dev libtiff5-dev libpng-dev libdrm-dev libexif-dev cppzmq-dev
sudo apt install -y git meson ninja-build cmake
sudo apt install -y libboost-program-options-dev  # NOTE: This command will take 8 hours on a RPi Zero
```

```commandline
git clone https://github.com/oneleggedredcow/rpicam-apps
cd rpicam-apps
meson setup build -Denable_libav=false -Denable_drm=true -Denable_egl=false -Denable_qt=false -Denable_opencv=false -Denable_tflite=false -Denable_zmq=true
meson compile -C build -j 1
```

License
-------

The source code is made available under the simplified [BSD 2-Clause license](https://spdx.org/licenses/BSD-2-Clause.html).

Status
------

[![ToT libcamera build/run test](https://github.com/raspberrypi/rpicam-apps/actions/workflows/rpicam-test.yml/badge.svg)](https://github.com/raspberrypi/rpicam-apps/actions/workflows/rpicam-test.yml)
