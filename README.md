# pasend : Pc to Android audio transmission

### Requirements :

pulseaudio

***Make sure both are on the same Network!!***

### Usage :

`pasend start|stop`

### Installation :

# On Pc:

```
git clone https://github.com/DopeWiz/pasend.git
cd pasend
make
./pasend start|stop
```

# On Android:

Download and install PulseDroid app from this repository:
- [PulseDroid.apk](https://github.com/dront78/PulseDroid/tree/master/bin)
- [SimpleProtocolPlayer](https://play.google.com/store/apps/details?id=com.kaytat.simpleprotocolplayer&hl=en)(lower latency)

Input the Local IP address of your Pc and put 8000 (default) as port number
