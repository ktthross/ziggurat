#!/bin/bash
mcs -t:library zig_random.cs
mcs -r:zig_random.dll -t:library ziggurat.cs
mcs -r:ziggurat.dll zig_handler.cs
