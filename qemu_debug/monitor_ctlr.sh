#!/bin/bash

ip=127.0.0.1
port=3333

nc $ip $port << EOF
q
EOF

