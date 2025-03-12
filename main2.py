import pygame
import socket
import time

# arduino connection details
arduino_ip = "192.168.1.50"
port = 8080

# joystick dead zone threshold
DEAD_ZONE = 0.1  

pygame.init()
if pygame.joystick.get_count() == 0: # looks for joystick and if not found exit python
    print("no joystick found")
    exit()
joystick = pygame.joystick.Joystick(0)
joystick.init()
print("joystick started")

def apply_dead_zone(value, threshold=DEAD_ZONE):
    """returns 0 if the value is within the dead zone, otherwise returns the value."""
    return 0 if abs(value) < threshold else value

def get_motor_commands():
    pygame.event.pump()

    leftStick = apply_dead_zone(joystick.get_axis(1))
    rightStick = apply_dead_zone(joystick.get_axis(3))
    up_thrust = apply_dead_zone(joystick.get_axis(5))
    down_thrust = apply_dead_zone(joystick.get_axis(4))

    leftMotor = round(leftStick * 255) # turns -1 to 1 range to -255 to 255
    rightMotor = round(rightStick * 255)
    upMotor = round(up_thrust * 255)
    downMotor = round(down_thrust * 255)

    return leftMotor, rightMotor, upMotor, downMotor

def connect_to_arduino():
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((arduino_ip, port))
        print(f"Connected to Arduino at {arduino_ip}:{port}")
        return sock
    except Exception as e:
        print("Connection error:", e)
        return None

client_socket = None
while True:
    if client_socket is None:
        print("Attempting to connect to Arduino...")
        client_socket = connect_to_arduino()
        if client_socket is None:
            print("Retrying connection in 3 seconds...")
            time.sleep(3)
            continue
    
    left, right, up, down = get_motor_commands()
    command = f"L:{left},R:{right},U:{up},D:{down}\n"

    try:
        client_socket.sendall(command.encode())
        print("Command sent successfully:", command.strip())
    except Exception as e:
        print("Error during send:", e)
        print("Closing socket and attempting to reconnect.")
        client_socket.close()
        client_socket = None
        time.sleep(2)
        continue  # go back and retry the connection
    
    time.sleep(0.5)  # reduce reconnection issue
