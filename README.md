# Arduino Operation Game

![](/[photos]/IMG_20181223_010038.jpg)

When I was a boy, I had an old "Operation" game (here, in Italy, was called "L'allegro chirurgo"). It, basically, was just composed by a simple circuit based on two C sized batteries, a little incandescent bulb (for the nose), and an electromechanic buzzer.

After 35 years the game was still in perfect conditions, so in a day of december, I decided to take it out of the box to amuse my daughters. But it was in 2018: almost nobody uses C sized battery, nowadays, the mechanical buzzer was failing, and the incandescent lamp seemed like found in a museum. So, I thought "no problem, let's modernize it!"

So I built a simple arduino circuit with a little speaker (probably taken from an old cordless phone) and a red led, and wrote a simple sketch that plays a wav sample and blinks the led when contact between the tweezers and the metal plate is detected.

Play of sample is done via PCM library by damellis: [https://github.com/damellis/PCM/](https://github.com/damellis/PCM/).
Audio sample data was resampled to 8KHz using [SoX](http://sox.sourceforge.net/):
`sox sample.wav -r 8000 -b 8 sample_8khz.wav`  
then converted to C include file using **xxd**:
`xxd -i -s +44 sample_8khz.wav > sample_data.h`

In this repository you can find the arduino code, complete schematic, some photos of the build, and 3d models of the enclosure and the red nose (quickly sketched on tinkercad).
![](/[3d_models]/arduino_nano_with_speaker_enclosure.png)
![](/[3d_models]/operation_game_led_nose.png)

