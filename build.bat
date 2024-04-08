call clean.bat
sdcc -c -mz80 main.c
sdcc -c -mz80 vdp.c
sdcc -o main.ihx -mz80 --no-std-crt0 --data-loc 0xC000 -Wl-b_BANK2=0x8000 crt0\crt0_sms.rel main.rel vdp.rel SMSlib.lib PSGlib\PSGlib.rel bank2.rel
makesms -pm main.ihx game.sms
c:\smsdev\emulicious\emulicious game.sms
