#!/bin/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/app/tars/tarsnode/data/TestApp.HelloServer/bin/:/usr/local/app/tars/tarsnode/data/lib/

./LobbyServer --config=./conf/TestApp.LobbyServer.config.conf  &
