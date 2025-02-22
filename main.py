import pygame
import socket
import time


ARDUINO_IP = "192.168.1.50"   # static ip set to match the network
PORT = 8080                   # Port number on which the arduino server is listening

#  pygame and the controller
pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

def get_motor_commands():
    # update joystick events
    pygame.event.pump()
    
    # horizontal control:
    # left stick Y-axis for throttle (forward/backward) - invert so forward is positive.
    throttle = -joystick.get_axis(1)
    # right stick X-axis for turning (left/right)
    turn = joystick.get_axis(3)
    
    # vertical control using triggers:
    # right trigger for up; Left trigger for down.
    up_thrust = joystick.get_axis(5)    
    down_thrust = joystick.get_axis(2)  
    

    left_motor = int((throttle - turn) * 255)
    right_motor = int((throttle + turn) * 255)
    
    # scale triggers to 0 to 255 for vertical control. maybe this works?
    up_motor = int(up_thrust * 255)
    down_motor = int(down_thrust * 255)
    
    return left_motor, right_motor, up_motor, down_motor

while True:
    left, right, up, down = get_motor_commands()
    
    # format the command string: "L:<left>,R:<right>,U:<up>,D:<down>\n"
    command = f"L:{left},R:{right},U:{up},D:{down}\n"
    

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((ARDUINO_IP, PORT))
            sock.sendall(command.encode())
    except Exception as e:
        print("Connection error:", e)
    
    print("Sent:", command.strip())
    time.sleep(0.1)  
