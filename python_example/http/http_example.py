import requests
import json

ESP32_IP = "192.168.0.105"
PORT = 80
URL = f"http://{ESP32_IP}:{PORT}/api/cmd"

def send_json_command(payload: dict):
    """send JSON data to ESP32"""
    headers = {"Content-Type": "application/json"}
    try:
        response = requests.post(URL, headers=headers, data=json.dumps(payload), timeout=2)
        print(f"status: {response.status_code}")
        print(f"feedback: {response.text}")
    except requests.exceptions.RequestException as e:
        print(f"exception: {e}")

if __name__ == "__main__":
    # example
    data = {"T":202,
            "line":1,
            "text":"http cmd test",
            "update":1}
    send_json_command(data)
