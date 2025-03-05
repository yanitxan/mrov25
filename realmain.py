import pygame
import socket
import time

# config
ARDUINO_IP = "192.168.1.50"  # must match Arduino IP
PORT = 8080

# pygame and the controller
pygame.init()
if pygame.joystick.get_count() == 0:
    print("no joystick found")
    exit(1)
joystick = pygame.joystick.Joystick(0)
joystick.init()
print("joystick initialized.")

def get_motor_commands():
    pygame.event.pump()
    
    throttle = -joystick.get_axis(1)  
    turn = joystick.get_axis(2)       
    up_thrust = joystick.get_axis(5)    
    down_thrust = -joystick.get_axis(4)  
    
    left_motor = int((throttle - turn) * 255)
    right_motor = int((throttle + turn) * 255)
    up_motor = int(((up_thrust + 1) / 2) * 255)
    down_motor = int(((1 - down_thrust) / 2) * 255)
    
    return left_motor, right_motor, up_motor, down_motor

def connect_to_arduino():
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ARDUINO_IP, PORT))
        print(f" Connected to Arduino at {ARDUINO_IP}:{PORT}")
        return sock
    except Exception as e:
        print(" Connection error:", e)
        return None

client_socket = None
while True:
    if client_socket is None:
        print(" Attempting to connect to Arduino...")
        client_socket = connect_to_arduino()
        if client_socket is None:
            print(" Retrying connection in 5 seconds...")
            time.sleep(2)
            continue
    
    left, right, up, down = get_motor_commands()
    command = f"L:{left},R:{right},U:{up},D:{down}\n"
    
    try:
        client_socket.sendall(command.encode())
        print(" Command sent successfully:", command.strip())
    except Exception as e:
        print(" Error during send:", e)
        print("Closing socket and attempting to reconnect.")
        client_socket.close()
        client_socket = None
        time.sleep(2)
        continue  # Go back and retry connection
    
    time.sleep(0.5)  # Slower sending interval to reduce reconnection issues
