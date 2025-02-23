import pygame
import tkinter as tk
from tkinter import ttk

# Initialize Pygame and the PS4 controller
pygame.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Create the GUI window
root = tk.Tk()
root.title("PS4 Controller Test")
root.geometry("400x300")

# Labels for progress bars
tk.Label(root, text="Left Motor").grid(row=0, column=0, padx=10, pady=5)
tk.Label(root, text="Right Motor").grid(row=1, column=0, padx=10, pady=5)
tk.Label(root, text="Up Thrust (R2)").grid(row=2, column=0, padx=10, pady=5)
tk.Label(root, text="Down Thrust (L2)").grid(row=3, column=0, padx=10, pady=5)

# Progress Bars
left_motor_bar = ttk.Progressbar(root, length=200, mode="determinate")
right_motor_bar = ttk.Progressbar(root, length=200, mode="determinate")
up_thrust_bar = ttk.Progressbar(root, length=200, mode="determinate")
down_thrust_bar = ttk.Progressbar(root, length=200, mode="determinate")

left_motor_bar.grid(row=0, column=1, padx=10, pady=5)
right_motor_bar.grid(row=1, column=1, padx=10, pady=5)
up_thrust_bar.grid(row=2, column=1, padx=10, pady=5)
down_thrust_bar.grid(row=3, column=1, padx=10, pady=5)

# Label for displaying values
status_label = tk.Label(root, text="Waiting for input...", font=("Arial", 12))
status_label.grid(row=4, column=0, columnspan=2, pady=10)

def get_motor_commands():
    pygame.event.pump()

    # Left stick Y-axis (axis 1) for forward/backward throttle (invert so that forward is positive)
    throttle = -joystick.get_axis(1)
    
    # Right stick horizontal (now using axis 2) for turning (left/right)
    turn = joystick.get_axis(2)
    
    # Use triggers for vertical control:
    # R2 (axis 5) for up thrust; L2 (axis 4) for down thrust.
    # Normalize raw values from [-1, 1] to [0, 255]
    up_raw = joystick.get_axis(5)   # R2 trigger
    down_raw = joystick.get_axis(4) # L2 trigger
    up_thrust = ((up_raw + 1) / 2) * 255
    down_thrust = ((down_raw + 1) / 2) * 255

    # Differential drive calculation for horizontal movement:
    # left motor = throttle - turn, right motor = throttle + turn
    left_motor = throttle - turn
    right_motor = throttle + turn

    # Clamp horizontal motor values to -1.0 to 1.0
    left_motor = max(-1.0, min(1.0, left_motor))
    right_motor = max(-1.0, min(1.0, right_motor))

    return left_motor, right_motor, up_thrust, down_thrust

def update_gui():
    left, right, up, down = get_motor_commands()

    # Convert horizontal motor values (-1.0 to 1.0) to a percentage (0-100)
    left_motor_bar["value"] = (left + 1) * 50  # Maps -1->0, 0->50, 1->100
    right_motor_bar["value"] = (right + 1) * 50
    
    # Vertical thrust bars now directly display values from 0 to 255
    up_thrust_bar["value"] = up
    down_thrust_bar["value"] = down

    status_label.config(text=f"Left: {left:.2f}, Right: {right:.2f}, Up: {up:.0f}, Down: {down:.0f}")

    root.after(50, update_gui)  # Update every 50ms

# Start the GUI update loop
root.after(50, update_gui)
root.mainloop()
