savedcmd_/home/sherlock/tests/softlockup/busyloop.mod := printf '%s\n'   busyloop.o | awk '!x[$$0]++ { print("/home/sherlock/tests/softlockup/"$$0) }' > /home/sherlock/tests/softlockup/busyloop.mod
