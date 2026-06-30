import serial
import struct
import time

# --- CONFIGURATION ---
SERIAL_PORT = 'COM3'  # <=== MAKE SURE this matches your USB-to-TTL adapter COM port!
BAUD_RATE = 9600

# Flight Path Parameters (Geographic Coordinate Benchmarks)
MUMBAI_LAT = 19124400    # 19.1244° N represented as a scaled int32
MUMBAI_LON = 72851500    # 72.8515° E represented as a scaled int32

TOKYO_LAT = 35777500     # 35.7775° N
TOKYO_LON = 140392100    # 140.3921° E

FLIGHT_DURATION_SECS = 60  # Total time for the simulated flight (1 minute test)
VEHICLE_NAME = b"AI-101"   # 6-byte aircraft callsign identifier

def scale_and_pack(lat, lon, status, name):
    """
    Packages data into the precise 15-byte hardware footprint:
    - lat : int32_t (4 bytes, Little-Endian '<i')
    - lon : int32_t (4 bytes, Little-Endian '<i')
    - status : uint8_t (1 byte, 'B')
    - name : char[6] (6 bytes, '6s')
    """
    # Ensure callsign is exactly 6 bytes by truncating or padding with spaces
    padded_name = name[:6].ljust(6, b' ')
    
    # Pack parameters into a continuous binary structure
    payload = struct.pack('<iiB6s', lat, lon, status, padded_name)
    return payload

def main():
    print(f"Connecting to USB-to-TTL module on {SERIAL_PORT}...")
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    except serial.SerialException as e:
        print(f"Error opening port: {e}\nCheck connection or update COM port string.")
        return

    print("Flight Simulation Started: Mumbai (BOM) -> Tokyo (NRT)")
    time.sleep(2) # Stabilization grace period for serial link

    start_time = time.time()
    
    try:
        while True:
            elapsed = time.time() - start_time
            if elapsed > FLIGHT_DURATION_SECS:
                break
            
            # Calculate completion fraction (0.0 to 1.0)
            fraction = elapsed / FLIGHT_DURATION_SECS
            
            # Linear interpolation (LERP) between current location values
            current_lat = int(MUMBAI_LAT + (TOKYO_LAT - MUMBAI_LAT) * fraction)
            current_lon = int(MUMBAI_LON + (TOKYO_LON - MUMBAI_LON) * fraction)

            # Build the payload configuration block
            status_flag = 1 # 1 = In flight Active
            payload = scale_and_pack(current_lat, current_lon, status_flag, VEHICLE_NAME)
            
            # Transmit the packet: Start Character '#' + 15 Payload Bytes
            ser.write(b'#')      
            ser.write(payload)   
            
            print(f"Transmitting -> Lat: {current_lat}, Lon: {current_lon} | Progress: {fraction*100:.1f}%")
            
            time.sleep(1.0) # Send updates at 1 Hz
            
        print("\nFlight has safely landed at Tokyo Narita International Airport.")
        
    except KeyboardInterrupt:
        print("\nSimulation aborted by user.")
    finally:
        ser.close()
        print("Serial interface safely unlinked.")

if __name__ == "__main__":
    main()