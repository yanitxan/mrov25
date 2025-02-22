import pygame
import socket
import time
# 
# Arduino IP and port (Dont forget to change)
ARDUINO_IP = "192.168.1.50"
PORT = 5005

# Initialize pygame and joystick
pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

def send_motor_command(left_speed, right_speed, up_speed, down_speed):
    
   # Sends a command like 'L:100,R:-100,U:80,D:-80\n' 
   
    command = f"L:{left_speed},R:{right_speed},U:{up_speed},D:{down_speed}\n"
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((ARDUINO_IP, PORT))
            s.sendall(command.encode())  # Send command
            response = s.recv(1024).decode()  # Receive
            print(f"Sent: {command.strip()} | Received: {response}")
    except Exception as e:
        print(f"Error sending command: {e}")

while True:
    pygame.event.pump()  # Process joystick events

    # Read joystick
    throttle = -joystick.get_axis(1)  # Forward/Backward
    turn = joystick.get_axis(3)       # Left/Right turn

    # Read trigger values (0 to 1) i think
    down_thrust = joystick.get_axis(2)  # LT for DOWN
    up_thrust = joystick.get_axis(5)    # RT for UP

    # Convert triggers to motor speeds (0-255)
    up_speed = int(up_thrust * 255)
    down_speed = int(down_thrust * 255)

    # Convert joystick values to motor speeds (-255 to 255)
    left_motor_speed = int((throttle + turn) * 255)  
    right_motor_speed = int((throttle - turn) * 255)

    # Limit speed values between -255 and 255
    left_motor_speed = max(-255, min(255, left_motor_speed))
    right_motor_speed = max(-255, min(255, right_motor_speed))

    # Send data to Arduino
    send_motor_command(left_motor_speed, right_motor_speed, up_speed, -down_speed)

    time.sleep(0.1)
