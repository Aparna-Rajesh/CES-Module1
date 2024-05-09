**Demonstration & Installation**

[![YouTube video player](https://img.youtube.com/vi/vUEu3yvQ8Dk/0.jpg)](https://www.youtube.com/watch?v=vUEu3yvQ8Dk)

[![YouTube video player](https://img.youtube.com/vi/-QTiDZOsdoQ/0.jpg)](https://www.youtube.com/watch?v=-QTiDZOsdoQ)


***ESP Setup***
1. Install the Arduino IDE from the official website.
2. Install the ESP32 board support package in the Arduino IDE. Follow the instructions here.
3. Install the TFT_eSPI library by Bodmer. Go to Sketch > Include Library > Manage Libraries..., search for "TFT_eSPI" and install it.
4. Connect your ESP32 board to your computer via USB.
5. Open the code provided in the Arduino IDE.
6. Upload the code to your ESP32 board.
7. After uploading the code, the generative art display will start running immediately. The display alternates between soothing messages, displayed in random colors, and anxiety-inducing phrases, displayed in black and white towards the bottom right corner of the screen.

The display operates within a 24-second loop, with 60% of the time dedicated to soothing messages, 20% to anxiety-inducing phrases, and 10% to deep sleep mode to conserve power.

***Building the Display***
1. Use heat shrink to build a battery setup (with a 130mAh battery) that will be compatible with the board. Remember that black wires should never be connected with red wires!

2. Connect the battery to the board.

3. Design an envelope with a rectangular hole in the center for the ESP display. Write calming messages in different colors horizontally across the envelope; in smaller letters, add anxious messages in black letters vertically towards the bottom.

4. Poke a small hole in a popsicle stick and string a thread through the hole. Tie one end of the string to the popsicle stick. Wrap the string around the width of the popsicle stick and tie it again. The majority of the string should hang from the popsicle stick.

5. Poke a small hole in the envelope and pull the string through the hole and tie it. The envelope should be hanging from the popsicle stick.

6. Use tape to fasten the ESP (the display should poke out a bit through the rectangular hole in the paper) and the battery inside the envelope and seal it shut.

7. Place the popsicle stick between beams underneath the air vent on the 5th floor lounge in Milstein.


See the full README  [here](https://aparna-rajesh.github.io/)
