#!/bin/bash
 
# run
run() {
    nohup "$*" >/dev/null 2>&1 &
}
