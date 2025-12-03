#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <winrt/Windows.Gaming.Input.h>

using namespace winrt;
using namespace Windows::Gaming::Input;

int main() {
    winrt::init_apartment();

    // Esperar a que se conecte un mando
    Gamepad pad = nullptr;
    while (pad == nullptr) {
        auto gamepads = Gamepad::Gamepads();
        if (gamepads.Size() > 0) {
            pad = gamepads.GetAt(0);
            std::cout << "Mando detectado.\n";
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // Bucle principal
    while (true) {
        auto reading = pad.GetCurrentReading();

        // Valor del gatillo derecho (0.0 a 1.0)
        double rightTrigger = reading.RightTrigger();

        if (rightTrigger > 0.5) { // presionado más de la mitad
            GamepadVibration vib{};
            vib.RightTrigger = 0.3; // intensidad ligera
            pad.Vibration(vib);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Apagar vibración
            vib.RightTrigger = 0.0;
            pad.Vibration(vib);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
