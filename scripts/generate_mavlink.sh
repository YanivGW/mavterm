#!/bin/bash
git submodule update --init --recursive
cd mavlink
python -m pymavlink.tools.mavgen --lang=C --wire-protocol=1.0 --output=../src/mavlink/autogen message_definitions/v1.0/common.xml
cd ..