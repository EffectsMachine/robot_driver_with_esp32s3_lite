import json
import websocket

ESP32_IP = "192.168.0.104"
PORT = 80
PATH = "/ws"
WS_URL = f"ws://{ESP32_IP}:{PORT}{PATH}"

def on_message(ws, message):
    """callback: message recv"""
    print(f"recv: {message}")


def on_error(ws, error):
    """callback: error"""
    print(f"WebSocket error: {error}")


def on_close(ws, close_status_code, close_msg):
    """callback: connection closed"""
    print("connection closed")


def on_open(ws):
    """callback: new connection"""
    print("connected to ESP32 WebSocket")

    # create JSON cmd here
    command = {
        "T": 202,
        "line": 1,
        "text": "webSocket msg",
        "update":1
    }

    # send cmd
    ws.send(json.dumps(command))
    print(f"send: {command}")


def close_ws():
    ws.close()


if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp(
        WS_URL,
        on_message=on_message,
        on_error=on_error,
        on_close=on_close,
        on_open=on_open
    )

    ws.run_forever()