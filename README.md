# Snowman Enroll

Custom wake word creation for [snowboy](https://github.com/Kitt-AI/snowboy) using [snowman](https://github.com/Thalhammer/snowman/)


## Building

``` sh
make
```


## Enrolling

Record 3 samples of your wake word:

``` sh
arecord -r 16000 -c 1 -f S16_LE -t wav -d 3 sample1.wav
...
```

Create the wake word:

``` sh
build/apps/enroll \
    --language en \
    --output my_wake_word.pmdl \
    --recording sample1.wav \
    --recording sample2.wav \
    --recording sample3.wav
```


## Detecting

``` sh
arecord -r 16000 -c 1 -f S16_LE -t raw - | \
    build/apps/detect -m /path/to/my_wake_word.pmdl
```
