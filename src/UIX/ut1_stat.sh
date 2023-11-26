#! /bin/bash
nm -o $1 | grep " T " | awk '{print $NF}' >> /tmp/t2
sort /tmp/t2 > /tmp/t3
uniq /tmp/t3 > /tmp/t2
# eof
