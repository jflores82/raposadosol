call clean.bat
sdcc -c -mz80 main.c
sdcc -c -mz80 vdp.c
sdcc -c -mz80 sram.c
sdcc -c -mz80 fxsample.c
sdcc -c -mz80 --constseg BANK2 bank2.c
sdcc -c -mz80 --constseg BANK3 bank3.c
sdcc -c -mz80 --constseg BANK4 bank4.c
sdcc -o main.ihx -mz80 --no-std-crt0 --data-loc 0xC000 -Wl-b_BANK2=0x28000 -Wl-b_BANK3=0x38000 -Wl-b_BANK4=0x48000 crt0\crt0_sms.rel main.rel vdp.rel sram.rel fxsample.rel SMSlib.lib PSGlib\PSGlib.rel bank2.rel bank3.rel bank4.rel
makesms -pm main.ihx game.sms
c:\smsdev\emulicious\emulicious game.sms
