import tkinter as tk
import socket

# Define the Quadcopter IP address
UDP_IP = "ESP8266_IP_ADDRESS"
UDP_PORT = 12345
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_command(command):
    sock.sendto(command.encode(), (UDP_IP, UDP_PORT))

def on_key_press(event):
    # Map arrow keys to respective directions and send commands
    key = event.keysym.lower()
    if key == "up":
        send_command("FORWARD")
    elif key == "down":
        send_command("BACKWARD")
    elif key == "right":
        send_command("RIGHT")
    elif key == "left":
        send_command("LEFT")

def on_w_key(event):
    send_command("UP")

def on_s_key(event):
    send_command("DOWN")

def is_connected():
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.settimeout(2)  # Set a timeout for the response
        sock.sendto(b'ping', (UDP_IP, UDP_PORT))
        data, addr = sock.recvfrom(1024)  # Wait for a response
        if data.decode() == 'pong':
            return True
        else:
            return False
    except socket.timeout:
        return False
    except Exception as e:
        print(f"Error: {e}")
        return False
    finally:
        sock.close()
        
app = tk.Tk()
app.title("Quadcopter Control")

# Create frames for layout
throttle_buttons_frame = tk.Frame(app)
direction_buttons_frame = tk.Frame(app)
connection_status_frame = tk.Frame(app)

# Create U buttons for control
up_button = tk.Button(throttle_buttons_frame, text="Up")
down_button = tk.Button(direction_buttons_frame, text="Down")

# Create arrow buttons for control
forward_button = tk.Button(direction_buttons_frame, text="Up")
backward_button = tk.Button(direction_buttons_frame, text="Down")
left_button = tk.Button(direction_buttons_frame, text="Left")
right_button = tk.Button(direction_buttons_frame, text="Right")

# Create label for connection status
connection_status = tk.StringVar(value="Connection Status: NOT CONNECTED - No MPU found")
connection_label = tk.Label(connection_status_frame, textvariable=connection_status, foreground='red')

up_button.grid(row=0, column=1, padx=10, pady=5)
down_button.grid(row=2, column=1, padx=10, pady=5)
left_button.grid(row=1, column=0, padx=10, pady=5)
right_button.grid(row=1, column=2, padx=10, pady=5)

connection_label.pack(side=tk.LEFT, padx=10)

# Pack the frames using .grid()
throttle_buttons_frame.grid(row=0, column=0)
direction_buttons_frame.grid(row=0, column=1)
connection_status_frame.grid(row=2, column=0, columnspan=2)

# Update connection status text and color
if is_connected():
    connection_status.set("Connection Status: CONNECTED - MPU found")
    connection_label.configure(foreground='green')
else:
    connection_status.set("Connection Status: NOT CONNECTED - No MPU found")
    connection_label.configure(foreground='red')

# Bind arrow key events to the key press function
app.bind("<Up>", on_key_press)
app.bind("<Down>", on_key_press)
app.bind("<Right>", on_key_press)
app.bind("<Left>", on_key_press)
app.bind("w", on_w_key)
app.bind("s", on_s_key)

app.mainloop()