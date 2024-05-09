import socket
import tkinter as tk
import numpy as np
import time
from threading import Thread
import random

# Network settings for ESP connection
UDP_IP = "192.168.1.232"  # The IP that is printed in the serial monitor from the ESP32
SHARED_UDP_PORT = 4210
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP socket
sock.connect((UDP_IP, SHARED_UDP_PORT))

pot_val, button_val, joy_x, joy_y = 0, 0, 0, 0
button_presses = 0

# Function to handle UDP communication
def udp_communication():
    global pot_val
    global joy_x
    global joy_y
    global button_val
    global button_presses
    sock.send('Hello ESP32'.encode())
    while True:
        data = sock.recv(2048)
        pot_val, joy_x, joy_y, button_val, = data.decode().split("---")
        if button_val == "Pressed":
            button_presses += 1
        print("Received from ESP32:", "potentiometer:", pot_val, "joy x:", joy_x, "joy y:", joy_y, "button:", button_val, "# presses: ", button_presses)
        sock.send('Received and printed.'.encode())

# GUI settings for the lava lamp
width, height, colors = 720, 480, 255
ymax, xmax = 3.5, 5
frame, frames = 0, 250
color_list = [[[253, 231, 36], [78, 1, 94]], [[42, 106, 219], [227, 95, 50]], [[12, 201, 34], [33, 207, 204]], [[140, 33, 217], [23, 230, 209]], [[230, 51, 23], [237, 121, 198]], [[random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)], [random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)]] ]

def update_lava_lamp(root, image):
    global frame  # Declare frame as global to modify the external variable
    global pot_val
    global joy_y
    y, x = np.ogrid[ymax:-ymax:height*1j, -xmax:xmax:width*1j]
    cy, cx = np.cos(2 * y), np.cos(2 * x)
    ppmheader = b"P6\n%d %d\n%d\n" % (width, height, colors)
    
    neutral_position = 1854  # Midpoint value where joystick is untouched
    base_sleep_time = 0.06   # Base sleep time when joystick is at neutral
    speed_factor_right = 0.004  # Speed increase factor to the right
    speed_factor_left = 0.001  # Smaller factor for slowing down to the left

    while True:
        frame = (frame + 1) % frames  # Update and use the global frame variable
        phase = 2 * np.pi * frame / frames
        plane = cx * np.cos(y + phase) > cy * np.sin(y + phase)
        temp_arr = color_list[button_presses % len(color_list)]
        factor = int(pot_val) / 4095
        outer_color, inner_color = temp_arr[0], temp_arr[1]
        outer_color = [outer_color[0] * factor, outer_color[1] * factor, outer_color[2] * factor]
        inner_color = [inner_color[0] * factor, inner_color[1] * factor, inner_color[2] * factor]

        rgbcolors = np.uint8([outer_color, inner_color])
        rgbimg = rgbcolors[plane.astype(np.uint8)].tobytes()
        image.put(data=ppmheader + rgbimg)
        root.update()

        # Adjust sleep time based on joystick y-position
        joy_y_int = int(joy_y)  # Convert joystick y-position to integer
        if joy_y_int > neutral_position:
            speed_adjustment = (joy_y_int - neutral_position) * speed_factor_right
            sleep_time = max(0, base_sleep_time - speed_adjustment)
        else:
            speed_adjustment = (neutral_position - joy_y_int) * speed_factor_left
            sleep_time = min(0.1, base_sleep_time + speed_adjustment)  # Set a maximum slow down to 0.1 seconds

        time.sleep(sleep_time)



# Main function to setup GUI and start threads
def main():
    root = tk.Tk()
    image = tk.PhotoImage(master=root, width=width, height=height)
    tk.Label(master=root, image=image).pack()

    # Start the UDP communication in a separate thread
    udp_thread = Thread(target=udp_communication)
    udp_thread.start()

    # Start the lava lamp GUI update in the main thread
    update_lava_lamp(root, image)

if __name__ == "__main__":
    main()
