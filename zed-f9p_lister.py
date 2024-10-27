import serial

def main():
    rtk_reciever = serial.Serial('/dev/ttyACM0', 115200, timeout=5)

    while True:
        string = rtk_reciever.read(100)
        
        # Check if the data starts with the NMEA identifier '$'
        if string.startswith(b'$'):
            # Try to decode NMEA messages
            try:
                print(string.decode('utf-8'))
            except UnicodeDecodeError:
                print("Received malformed NMEA message")
        
        # Check if the data starts with UBX sync characters
        elif string.startswith(b'\xb5\x62'):
            print("Received UBX binary data:", string.hex())

        else:
            print("Unknown data format received")

if __name__ == "__main__":
    main()
